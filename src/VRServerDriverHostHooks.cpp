/**
 * @file VRServerDriverHostHooks.cpp
 * @author dousha
 * @date 2023/8/2
 */

#include "VRServerDriverHostHooks.h"

namespace vrhook {
namespace hook4 {
HookData<VRServerDriverHostHooks::TrackedDeviceAdded>
    VRServerDriverHostHooks::trackedDeviceAddedHook;
HookData<VRServerDriverHostHooks::TrackedDevicePoseUpdated>
    VRServerDriverHostHooks::trackedDevicePoseUpdatedHook;
HookData<VRServerDriverHostHooks::TrackedDeviceButtonMake>
    VRServerDriverHostHooks::trackedDeviceButtonMakeHook;
HookData<VRServerDriverHostHooks::TrackedDeviceButtonBreak>
    VRServerDriverHostHooks::trackedDeviceButtonBreakHook;
HookData<VRServerDriverHostHooks::TrackedDeviceButtonTouched>
    VRServerDriverHostHooks::trackedDeviceButtonContactHook;
HookData<VRServerDriverHostHooks::TrackedDeviceButtonReleased>
    VRServerDriverHostHooks::trackedDeviceButtonReleaseHook;
HookData<VRServerDriverHostHooks::TrackedDeviceAxisUpdated>
    VRServerDriverHostHooks::trackedDeviceAxisUpdatedHook;

VRServerDriverHostHooks::VRServerDriverHostHooks(void* ptr) {
    if (isHooked) {
        return;
    }

    ObtainLogger()->trace("Hooking IVRServerDriverHost_004 ({})",
                          (ptrdiff_t)ptr);
    CreateHook(trackedDeviceAddedHook,
               reinterpret_cast<void*>(trackedDeviceAdded),
               "hook4::VRServerDriverHostHooks::TrackedDeviceAdded",
               ptr,
               0);
    CreateHook(trackedDevicePoseUpdatedHook,
               reinterpret_cast<void*>(trackedDevicePoseUpdated),
               "hook4::VRServerDriverHostHooks::TrackedDevicePoseUpdated",
               ptr,
               1);
    CreateHook(trackedDeviceButtonMakeHook,
               reinterpret_cast<void*>(trackedDeviceButtonMake),
               "hook4::VRServerDriverHostHooks::TrackedDeviceButtonMake",
               ptr,
               3);
    CreateHook(trackedDeviceButtonBreakHook,
               reinterpret_cast<void*>(trackedDeviceButtonBreak),
               "hook4::VRServerDriverHostHooks::TrackedDeviceButtonBreak",
               ptr,
               4);
    CreateHook(trackedDeviceButtonContactHook,
               reinterpret_cast<void*>(trackedDeviceButtonContact),
               "hook4::VRServerDriverHostHooks::TrackedDeviceButtonTouched",
               ptr,
               5);
    CreateHook(trackedDeviceButtonReleaseHook,
               reinterpret_cast<void*>(trackedDeviceButtonRelease),
               "hook4::VRServerDriverHostHooks::TrackedDeviceButtonReleased",
               ptr,
               6);
    CreateHook(trackedDeviceAxisUpdatedHook,
               reinterpret_cast<void*>(trackedDeviceAxisUpdated),
               "hook4::VRServerDriverHostHooks::TrackedDeviceAxisUpdated",
               ptr,
               7);
    isHooked = true;
}

VRServerDriverHostHooks::~VRServerDriverHostHooks() {
    if (!isHooked) {
        ObtainLogger()->warn(
            "hook4::VRServerDriverHostHooks::~VRServerDriverHostHooks: "
            "already unhooked");
        return;
    }

    ObtainLogger()->debug("Unhooking IVRServerDriverHost_004");
    RemoveHook(trackedDeviceAddedHook);
    RemoveHook(trackedDevicePoseUpdatedHook);
    RemoveHook(trackedDeviceButtonMakeHook);
    RemoveHook(trackedDeviceButtonBreakHook);
    RemoveHook(trackedDeviceButtonContactHook);
    RemoveHook(trackedDeviceButtonReleaseHook);
    RemoveHook(trackedDeviceAxisUpdatedHook);
    isHooked = false;
}

std::shared_ptr<InterfaceHooks> VRServerDriverHostHooks::createHooks(
    void* ptr) {
    std::shared_ptr<InterfaceHooks> result
        = std::shared_ptr<InterfaceHooks>(new VRServerDriverHostHooks(ptr));
    return result;
}

void VRServerDriverHostHooks::TrackedDevicePoseUpdatedOriginal(
    void* ctx,
    uint32_t which,
    vr::DriverPose_t& nextPost,
    uint32_t poseStructureSize) {
    trackedDevicePoseUpdatedHook.originalFunc(
        ctx, which, nextPost, poseStructureSize);
}

void VRServerDriverHostHooks::TrackedDeviceButtonMakeOriginal(
    void* ctx, uint32_t which, vr::EVRButtonId button, double time) {
    trackedDeviceButtonMakeHook.originalFunc(ctx, which, button, time);
}

void VRServerDriverHostHooks::TrackedDeviceButtonBreakOriginal(
    void* ctx, uint32_t which, vr::EVRButtonId button, double time) {
    trackedDeviceButtonBreakHook.originalFunc(ctx, which, button, time);
}

void VRServerDriverHostHooks::TrackedDeviceButtonContactOriginal(
    void* ctx, uint32_t which, vr::EVRButtonId button, double time) {
    trackedDeviceButtonContactHook.originalFunc(ctx, which, button, time);
}

void VRServerDriverHostHooks::TrackedDeviceButtonReleaseOriginal(
    void* ctx, uint32_t which, vr::EVRButtonId button, double time) {
    trackedDeviceButtonReleaseHook.originalFunc(ctx, which, button, time);
}

void VRServerDriverHostHooks::TrackedDeviceAxisUpdatedOriginal(
    void* ctx,
    uint32_t which,
    uint32_t axis,
    const vr::VRControllerAxis_t& state) {
    trackedDeviceAxisUpdatedHook.originalFunc(ctx, which, axis, state);
}

bool VRServerDriverHostHooks::trackedDeviceAdded(
    void* ctx,
    const char* serial,
    vr::ETrackedDeviceClass deviceClass,
    void* driver) {
    ObtainLogger()->trace(
        "hook4::VRServerDriverHostHooks::trackedDeviceAdded({}, {}, {}, {})",
        (ptrdiff_t)ctx,
        serial,
        (uint32_t)deviceClass,
        (ptrdiff_t)driver);
    eventConsumer->OnTrackedDeviceAdded(ctx, 4, serial, deviceClass, driver);
    return false;
}

void VRServerDriverHostHooks::trackedDevicePoseUpdated(
    void* ctx,
    uint32_t which,
    vr::DriverPose_t& nextPose,
    uint32_t poseStructureSize) {
    // for vive hmd, this function is called ~1118 times per second
    // for vive controller, this function is called ~368 times per second per
    // device thus this function must run fast (enough)
    auto poseCopy = nextPose;
    if (eventConsumer->OnTrackedDevicePoseUpdate(
            ctx, 4, which, poseCopy, poseStructureSize)) {
        trackedDevicePoseUpdatedHook.originalFunc(
            ctx, which, poseCopy, poseStructureSize);
    }
}

void VRServerDriverHostHooks::trackedDeviceButtonMake(void* ctx,
                                                      uint32_t which,
                                                      vr::EVRButtonId button,
                                                      double time) {
    if (eventConsumer->OnTrackedDeviceButtonMake(ctx, 4, which, button, time)) {
        trackedDeviceButtonMakeHook.originalFunc(ctx, which, button, time);
    }
}

void VRServerDriverHostHooks::trackedDeviceButtonBreak(void* ctx,
                                                       uint32_t which,
                                                       vr::EVRButtonId button,
                                                       double time) {
    if (eventConsumer->OnTrackedDeviceButtonBreak(
            ctx, 4, which, button, time)) {
        trackedDeviceButtonBreakHook.originalFunc(ctx, which, button, time);
    }
}

void VRServerDriverHostHooks::trackedDeviceButtonContact(void* ctx,
                                                         uint32_t which,
                                                         vr::EVRButtonId button,
                                                         double time) {
    if (eventConsumer->OnTrackedDeviceButtonContact(
            ctx, 4, which, button, time)) {
        trackedDeviceButtonContactHook.originalFunc(ctx, which, button, time);
    }
}

void VRServerDriverHostHooks::trackedDeviceButtonRelease(void* ctx,
                                                         uint32_t which,
                                                         vr::EVRButtonId button,
                                                         double time) {
    if (eventConsumer->OnTrackedDeviceButtonRelease(
            ctx, 4, which, button, time)) {
        trackedDeviceButtonReleaseHook.originalFunc(ctx, which, button, time);
    }
}

void VRServerDriverHostHooks::trackedDeviceAxisUpdated(
    void* ctx,
    uint32_t which,
    uint32_t axis,
    const vr::VRControllerAxis_t& state) {
    auto copy = state;
    if (eventConsumer->OnTrackedDeviceAxisUpdate(ctx, 4, which, axis, copy)) {
        trackedDeviceAxisUpdatedHook.originalFunc(ctx, which, axis, copy);
    }
}
} // namespace hook4

namespace hook5 {
HookData<VRServerDriverHostHooks::TrackedDeviceAdded>
    VRServerDriverHostHooks::trackedDeviceAddedHook;
HookData<VRServerDriverHostHooks::TrackedDevicePoseUpdated>
    VRServerDriverHostHooks::trackedDevicePoseUpdatedHook;
HookData<VRServerDriverHostHooks::PollNextEvent>
    VRServerDriverHostHooks::pollNextEventHook;

VRServerDriverHostHooks::VRServerDriverHostHooks(void* ptr) {
    if (isHooked) {
        return;
    }

    ObtainLogger()->trace(
        "hook5::VRServerDriverHostHooks::VRServerDriverHostHooks ({})", ptr);
    CreateHook(trackedDeviceAddedHook,
               reinterpret_cast<void*>(trackedDeviceAdded),
               "hook5::VRServerDriverHostHooks::TrackedDeviceAdded",
               ptr,
               0);
    CreateHook(trackedDevicePoseUpdatedHook,
               reinterpret_cast<void*>(trackedDevicePoseUpdated),
               "hook5::VRServerDriverHostHooks::TrackedDevicePoseUpdated",
               ptr,
               1);
#ifdef OPENVR_HOOKS_ENABLE_EVENT_POLL_HOOK
    CreateHook(pollNextEventHook,
               reinterpret_cast<void*>(pollNextEvent),
               "hook5::VRServerDriverHostHooks::PollNextEvent",
               ptr,
               5);
#endif
    isHooked = true;
}

VRServerDriverHostHooks::~VRServerDriverHostHooks() {
    if (!isHooked) {
        ObtainLogger()->warn(
            "hook5::VRServerDriverHostHooks::~VRServerDriverHostHooks: "
            "already unhooked");
        return;
    }

    ObtainLogger()->debug("Unhooking IVRServerDriverHost_005");
    RemoveHook(trackedDeviceAddedHook);
    RemoveHook(trackedDevicePoseUpdatedHook);
#ifdef OPENVR_HOOKS_ENABLE_EVENT_POLL_HOOK
    RemoveHook(pollNextEventHook);
#endif
    isHooked = false;
}

std::shared_ptr<InterfaceHooks> VRServerDriverHostHooks::createHooks(
    void* ptr) {
    return std::shared_ptr<InterfaceHooks>(new VRServerDriverHostHooks(ptr));
}

void VRServerDriverHostHooks::TrackedDevicePoseUpdatedOriginal(
    void* ctx,
    uint32_t which,
    vr::DriverPose_t& nextPost,
    uint32_t poseStructureSize) {
    trackedDevicePoseUpdatedHook.originalFunc(
        ctx, which, nextPost, poseStructureSize);
}

bool VRServerDriverHostHooks::trackedDeviceAdded(
    void* ctx,
    const char* serial,
    vr::ETrackedDeviceClass deviceClass,
    void* driver) {
    ObtainLogger()->trace(
        "hook5::VRServerDriverHostHooks::trackedDeviceAdded({}, {}, {}, {})",
        ctx,
        serial,
        (uint32_t)deviceClass,
        driver);
    eventConsumer->OnTrackedDeviceAdded(ctx, 5, serial, deviceClass, driver);
    auto result
        = trackedDeviceAddedHook.originalFunc(ctx, serial, deviceClass, driver);
    return result;
}

void VRServerDriverHostHooks::trackedDevicePoseUpdated(
    void* ctx,
    uint32_t which,
    const vr::DriverPose_t& nextPose,
    uint32_t poseStructureSize) {
    // for vive hmd, this function is called ~1118 times per second
    // for vive controller, this function is called ~368 times per second per
    // device thus this function must run fast (enough)
    auto poseCopy = nextPose;
    if (eventConsumer->OnTrackedDevicePoseUpdate(
            ctx, 5, which, poseCopy, poseStructureSize)) {
        trackedDevicePoseUpdatedHook.originalFunc(
            ctx, which, poseCopy, poseStructureSize);
    }
}

[[maybe_unused]]
bool VRServerDriverHostHooks::pollNextEvent(void* ctx,
                                            vr::VREvent_t* event,
                                            uint32_t eventSize) {
    auto injectedEvent = eventConsumer->GetDriverEventForInjection(ctx);
    if (injectedEvent.first) {
        if (injectedEvent.second == eventSize) {
            memcpy(event, injectedEvent.first.get(), eventSize);
            auto openVrEvent = (vr::VREvent_t*)event;
            ObtainLogger()->debug("hook5::VRServerDriverHostHooks::"
                                  "pollNextEvent: injected event {}, {}",
                                  openVrEvent->eventType,
                                  openVrEvent->trackedDeviceIndex);
            return true;
        } else {
            auto openVrEvent = (vr::VREvent_t*)injectedEvent.first.get();
            ObtainLogger()->error("hook5::VRServerDriverHostHooks::"
                                  "pollNextEvent: injected event {}, {} has "
                                  "invalid size {} (expected {})",
                                  openVrEvent->eventType,
                                  openVrEvent->trackedDeviceIndex,
                                  injectedEvent.second,
                                  eventSize);
        }
    }

    bool result, downstreamResult;

    do {
        result = pollNextEventHook.originalFunc(ctx, event, eventSize);
        if (result) {
            downstreamResult
                = eventConsumer->OnPollNextEvent(ctx, 5, event, eventSize);
        }
    } while (result && !downstreamResult);

    return result;
}
} // namespace hook5

namespace hook6 {
HookData<VRServerDriverHostHooks::TrackedDeviceAdded>
    VRServerDriverHostHooks::trackedDeviceAddedHook;
HookData<VRServerDriverHostHooks::TrackedDevicePoseUpdated>
    VRServerDriverHostHooks::trackedDevicePoseUpdatedHook;
HookData<VRServerDriverHostHooks::PollNextEvent>
    VRServerDriverHostHooks::pollNextEventHook;

VRServerDriverHostHooks::VRServerDriverHostHooks(void* ptr) {
    if (isHooked) {
        return;
    }

    ObtainLogger()->trace(
        "hook6::VRServerDriverHostHooks::VRServerDriverHostHooks ({})", ptr);
    CreateHook(trackedDeviceAddedHook,
               reinterpret_cast<void*>(trackedDeviceAdded),
               "hook6::VRServerDriverHostHooks::TrackedDeviceAdded",
               ptr,
               0);
    CreateHook(trackedDevicePoseUpdatedHook,
               reinterpret_cast<void*>(trackedDevicePoseUpdated),
               "hook6::VRServerDriverHostHooks::TrackedDevicePoseUpdated",
               ptr,
               1);
#ifdef OPENVR_HOOKS_ENABLE_EVENT_POLL_HOOK
    CreateHook(pollNextEventHook,
               reinterpret_cast<void*>(pollNextEvent),
               "hook6::VRServerDriverHostHooks::PollNextEvent",
               ptr,
               5);
#endif
    isHooked = true;
}

VRServerDriverHostHooks::~VRServerDriverHostHooks() {
    if (!isHooked) {
        ObtainLogger()->warn(
            "hook6::VRServerDriverHostHooks::~VRServerDriverHostHooks: "
            "already unhooked");
        return;
    }

    ObtainLogger()->debug("Unhooking IVRServerDriverHost_006");
    RemoveHook(trackedDeviceAddedHook);
    RemoveHook(trackedDevicePoseUpdatedHook);
#ifdef OPENVR_HOOKS_ENABLE_EVENT_POLL_HOOK
    RemoveHook(pollNextEventHook);
#endif
    isHooked = false;
}

std::shared_ptr<InterfaceHooks> VRServerDriverHostHooks::createHooks(
    void* ptr) {
    return std::shared_ptr<InterfaceHooks>(new VRServerDriverHostHooks(ptr));
}

void VRServerDriverHostHooks::TrackedDevicePoseUpdatedOriginal(
    void* ctx,
    uint32_t which,
    vr::DriverPose_t& nextPost,
    uint32_t poseStructureSize) {
    trackedDevicePoseUpdatedHook.originalFunc(
        ctx, which, nextPost, poseStructureSize);
}

bool VRServerDriverHostHooks::trackedDeviceAdded(
    void* ctx,
    const char* serial,
    vr::ETrackedDeviceClass deviceClass,
    void* driver) {
    ObtainLogger()->trace(
        "hook6::VRServerDriverHostHooks::trackedDeviceAdded({}, {}, {}, {})",
        ctx,
        serial,
        (uint32_t)deviceClass,
        driver);
    eventConsumer->OnTrackedDeviceAdded(ctx, 6, serial, deviceClass, driver);
    auto result
        = trackedDeviceAddedHook.originalFunc(ctx, serial, deviceClass, driver);
    return result;
}

void VRServerDriverHostHooks::trackedDevicePoseUpdated(
    void* ctx,
    uint32_t which,
    const vr::DriverPose_t& nextPose,
    uint32_t poseStructureSize) {
    // for vive hmd, this function is called ~1118 times per second
    // for vive controller, this function is called ~368 times per second per
    // device thus this function must run fast (enough)
    auto poseCopy = nextPose;
    if (eventConsumer->OnTrackedDevicePoseUpdate(
            ctx, 6, which, poseCopy, poseStructureSize)) {
        trackedDevicePoseUpdatedHook.originalFunc(
            ctx, which, poseCopy, poseStructureSize);
    }
}

bool VRServerDriverHostHooks::pollNextEvent(void* ctx,
                                            vr::VREvent_t* event,
                                            uint32_t eventSize) {
    auto injectedEvent = eventConsumer->GetDriverEventForInjection(ctx);
    if (injectedEvent.first) {
        if (injectedEvent.second == eventSize) {
            memcpy(event, injectedEvent.first.get(), eventSize);
            auto openVrEvent = (vr::VREvent_t*)event;
            ObtainLogger()->debug("hook6::VRServerDriverHostHooks::"
                                  "pollNextEvent: injected event {}, {}",
                                  openVrEvent->eventType,
                                  openVrEvent->trackedDeviceIndex);
            return true;
        } else {
            auto openVrEvent = (vr::VREvent_t*)injectedEvent.first.get();
            ObtainLogger()->error("hook6::VRServerDriverHostHooks::"
                                  "pollNextEvent: injected event {}, {} has "
                                  "invalid size {} (expected {})",
                                  openVrEvent->eventType,
                                  openVrEvent->trackedDeviceIndex,
                                  injectedEvent.second,
                                  eventSize);
        }
    }

    bool result, downstreamResult;

    do {
        result = pollNextEventHook.originalFunc(ctx, event, eventSize);
        if (result) {
            downstreamResult
                = eventConsumer->OnPollNextEvent(ctx, 6, event, eventSize);
        }
    } while (result && !downstreamResult);

    return result;
}
} // namespace hook6
} // namespace vrhook
