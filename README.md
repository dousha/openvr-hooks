# openvr-hooks

## Building

### Prerequisites

You will need the following tools properly configured:

* [Visual Studio 2022 (or at least MSVC 17 toolchain)][vs]
* [CMake 3.25 or later][cmake]
* [vcpkg][vcpkg]

Using MSVC toolchian is recommended. GCC/Clang are not tested and are not guaranteed to even compile.

### Environment setup

You need to set the environment variable `VCPKG_ROOT` to the root path of your vcpkg installation. If you are using the bundled version provided by Visual Studio, the path is typically `X:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg` where `X` is the drive letter your Visual Studio is installed to.

### Compiling as a standalone project

Simply invoke CMake Presets to compile the project. (May take a while to download and compile dependencies.)

```sh
cmake --preset release-vs17
cmake --build --preset release-all
```

The library can be found at `./cmake-build-release-vs17/RelWithDebInfo/OpenVRHooks.lib`.

### Importing the project as a CMake sub-project

You need to either:

* Configure your system so that `openvr`, `minhook` and `spdlog` can be found by CMake with `find_package`, or
* Use vcpkg for your upstream projects, and add `openvr`, `minhook` and `spdlog` to your vcpkg dependencies.

Then you can import this project with `add_subdirectory` or `FetchContent`.

## Using the library

All functions and classes are available in the `vrhook` namespace.

### Initialize the hooking library

In your `vr::IServerTrackedDeviceProvider` implementation:

```cpp
class ServerDriver : public vr::IServerTrackedDeviceProvider {
    /* ... */

private:
    std::unique_ptr<HookEventConsumer> consumer; // see later sections
};

vr::EVRInitError ServerDriver::Init(vr::IVRDriverContext* ctx) {
    VR_INIT_SERVER_DRIVER_CONTEXT(ctx); 
    // all operations must be performed AFTER the context initialization

    auto hookLogger = ObtainNamedLogger("DaemonHooks");
    hookLogger->set_level(spdlog::level::info);
    vrhook::SetLogger(hookLogger);
    // to avoid log spamming / redirect hooking log to other facilities
    // you can set the logger used by the library

    if (!vrhook::InterfaceHooks::InitHooks()) {
        // critical failure, must abort here
        return vr::VRInitError_Driver_Failed;
    }

    consumer = std::make_unique<HookEventConsumer>(/* ... */);
    // see later sections about how to implement a HookEventConsumer
    vrhook::InterfaceHooks::SetEventConsumer(consumer.get());
    // set the consumer
    // note that each driver can only register 1 consumer
    // if there are multiple drivers all hooking into OpenVR runtime
    // some strange things may occur
    // see LIMITATIONS in the later sections

    auto driverContextHooks
        = vrhook::InterfaceHooks::HookInterface(ctx, "IVRDriverContext");
    // ^ hooking into the IVRDriverContext, this is critical for hooking into
    // other interfaces in OpenVR
    if (driverContextHooks == nullptr) {
        // if failed, the driver must abort
        return vr::VRInitError_Driver_Failed;
    }

    // do your rest of initialization here
    // ...
}
```

### Cleanup the hooking mechanisms

In your `vr::IServerTrackedDeviceProvider` implementation:

```cpp
void ServerDriver::Cleanup() {
    vrhook::InterfaceHooks::CleanUpHooks();
    consumer.reset();

    // do your rest of clean up here
    // ...

    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}
```

### Implementing a hook event consumer

Doxygen document is available for `vrhook::HookEventConsumer`. You will need a hook event consumer to obtain data and temper with (some of) them.

```cpp
class HookEventConsumer : public vrhook::HookEventConsumer { /* ... */ };
```

Here are some key methods:

* `OnTrackedDeviceAdded`: Called when an OpenVR device is added to the system. At this stage, the device is only declared to OpenVR and is not assigned an OpenVR ID. You may perform some early stage registration here.
  The `driver` parameter is the pointer to the actual driver class instance controlling this device. You may save this pointer to identify device instances later.
* `OnTrackedDeviceActivate`: Called when an OpenVR device is about to be activated. The `context` is the `driver` pointer provided in `OnTrackedDeviceAdded`. The same applies to all rest of methods.
* `OnTrackedDeviceActivated`: Called when an OpenVR device is successfully activated.
  You need to obtain its property container ID by using `vr::VRPropertiesRaw()->TrackedDeviceToPropertyContainer` as input/output components hooking functions are invoked with property container ID instead of OpenVR ID.
* `OnBooleanComponentCreated`: Called when a Boolean component is added to a device.
* `OnScalarComponentCreated`: Called when a Scalar component is added to a device.
* `OnHapticComponentCreated`: Called when a Haptic component is added to a device.
* `OnBooleanComponentUpdate`: Called when a Boolean component value is about to change. Returning `false` here would prevent the component from being updated.
* `OnScalarComponentUpdate`: Called when a Scalar component value is about to change. Returning `false` here would prevent the component from being updated.
* `OnTrackedDevicePoseUpdate`: Called when the pose of the device is about to change. Returning `false` here would prevent the pose from being updated, rendering the device frozen in place.

### Where is my `OnHapticComponentUpdate`?

There is no such thing. In the past where Vive Streaming Hub won't cause so much trouble, haptic events are filtered out from `PollNextEvent` and handled respectively (including injecting a haptic event and discarding haptic events to prevent the device from vibrating).

If you can make sure that your end user won't install Vive Streaming Hub on their computers (yes, installing without enabling the drive would cause SteamVR to crash on start up), you can define `OPENVR_HOOKS_ENABLE_EVENT_POLL_HOOK` macro and implement `OnPollNextEvent`.

### How to force an input?

It is possible to synthesize an update by invoking following methods:

* `vrhook::VRDriverInputHooks::UpdateBooleanComponentOriginal`
* `vrhook::VRDriverInputHooks::UpdateScalarComponentOriginal`

You will need `driver` pointer provided in `OnTrackedDeviceAdded` to properly invoke these functions.

### How to force a pose?

It is not possible to synthesize pose update calls (yet).

However, you can simply tamper with the pose in `OnTrackedDevicePoseUpdate`. Most VR device drivers would constantly update their pose on every frame even if the device does not have a valid pose (or even powered on) as long as the driver is loaded and the device is instantiated.

### How to create a new controller without writing a new driver for it?

It is no longer possible to fake a controller in the new input framework. You have to write a dedicated controller driver if you want to create a new controller.

## Limitations

### Load your driver as early as possible

To properly hook into other drivers, you need to ensure that your driver is loaded by OpenVR as early as possible since we can only obtain the run-time address of other drivers AFTER we hook into `IVRDriverContext`.

Common techniques including:

* Naming your driver something like `01mydriver` so its alphabetically smaller
* In `default.vrsettings`, give your driver a `loadPriority` of very high value like `1000`
* Fake that your driver may process graphics information by declaring `"redirectsDisplay": true` in your `driver.vrdrivermanifest`

### Only SteamVR runtime is currently supported

Ocular runtime would load its drivers first no matter what you do. So, tough luck if you need to hook into Ocular devices in Ocular runtime.

### Hooking cascade may lead to instability

If the end user installed multiple libraries all hooking into OpenVR runtime, a ill-behaving driver may lead to catastrophic data corruption in the runtime. Crashing would be frequent and the generated diagnostic messages would be cryptic at best.

So, before using this library to temper with OpenVR runtime, ask yourself if the function you need can be implemented without resorting to using a rather dangerous method.

If the answer is "no" and you have to dive into this insanity, be sure to double check that your logic is correct.

[vs]: https://visualstudio.microsoft.com/
[cmake]: https://cmake.org/download/
[vcpkg]: https://github.com/microsoft/vcpkg
