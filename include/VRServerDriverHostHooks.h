/**
 * @file VRServerDriverHostHooks.h
 * @author dousha
 * @date 2023/8/2
 */

#ifndef HIGCALIBRE_VRSERVERDRIVERHOSTHOOKS_H
#define HIGCALIBRE_VRSERVERDRIVERHOSTHOOKS_H

#include "common.h"
#include "InterfaceHooks.h"
#include <memory>
#include <openvr_driver.h>

namespace vrhook {

namespace hook4 {
class VRServerDriverHostHooks : public InterfaceHooks {
public:
    typedef bool (*TrackedDeviceAdded)(void*,
                                       const char*,
                                       vr::ETrackedDeviceClass,
                                       void*);

    typedef void (*TrackedDevicePoseUpdated)(void*,
                                             uint32_t,
                                             const vr::DriverPose_t&,
                                             uint32_t);

    typedef void (*TrackedDeviceButtonMake)(void*,
                                            uint32_t,
                                            vr::EVRButtonId,
                                            double);

    typedef void (*TrackedDeviceButtonBreak)(void*,
                                             uint32_t,
                                             vr::EVRButtonId,
                                             double);

    typedef void (*TrackedDeviceButtonTouched)(void*,
                                               uint32_t,
                                               vr::EVRButtonId,
                                               double);

    typedef void (*TrackedDeviceButtonReleased)(void*,
                                                uint32_t,
                                                vr::EVRButtonId,
                                                double);

    typedef void (*TrackedDeviceAxisUpdated)(void*,
                                             uint32_t,
                                             uint32_t,
                                             const vr::VRControllerAxis_t&);

    static std::shared_ptr<InterfaceHooks> createHooks(void* ptr);

    ~VRServerDriverHostHooks() override;

    static void TrackedDevicePoseUpdatedOriginal(
        void* ctx,
        uint32_t which,
        const vr::DriverPose_t& nextPost,
        uint32_t poseStructureSize);

    static void TrackedDeviceButtonMakeOriginal(void* ctx,
                                                uint32_t which,
                                                vr::EVRButtonId button,
                                                double time);

    static void TrackedDeviceButtonBreakOriginal(void* ctx,
                                                 uint32_t which,
                                                 vr::EVRButtonId button,
                                                 double time);

    static void TrackedDeviceButtonContactOriginal(void* ctx,
                                                   uint32_t which,
                                                   vr::EVRButtonId button,
                                                   double time);

    static void TrackedDeviceButtonReleaseOriginal(void* ctx,
                                                   uint32_t which,
                                                   vr::EVRButtonId button,
                                                   double time);

    static void TrackedDeviceAxisUpdatedOriginal(
        void* ctx,
        uint32_t which,
        uint32_t axis,
        const vr::VRControllerAxis_t& state);

private:
    bool isHooked = false;

    explicit VRServerDriverHostHooks(void* ptr);

    static HookData<TrackedDeviceAdded> trackedDeviceAddedHook;
    static HookData<TrackedDevicePoseUpdated> trackedDevicePoseUpdatedHook;
    static HookData<TrackedDeviceButtonMake> trackedDeviceButtonMakeHook;
    static HookData<TrackedDeviceButtonBreak> trackedDeviceButtonBreakHook;
    static HookData<TrackedDeviceButtonTouched> trackedDeviceButtonContactHook;
    static HookData<TrackedDeviceButtonReleased> trackedDeviceButtonReleaseHook;
    static HookData<TrackedDeviceAxisUpdated> trackedDeviceAxisUpdatedHook;

    static bool trackedDeviceAdded(void* ctx,
                                   const char* serial,
                                   vr::ETrackedDeviceClass deviceClass,
                                   void* driver);

    static void trackedDevicePoseUpdated(void* ctx,
                                         uint32_t which,
                                         const vr::DriverPose_t& nextPose,
                                         uint32_t poseStructureSize);

    static void trackedDeviceButtonMake(void* ctx,
                                        uint32_t which,
                                        vr::EVRButtonId button,
                                        double time);

    static void trackedDeviceButtonBreak(void* ctx,
                                         uint32_t which,
                                         vr::EVRButtonId button,
                                         double time);

    static void trackedDeviceButtonContact(void* ctx,
                                           uint32_t which,
                                           vr::EVRButtonId button,
                                           double time);

    static void trackedDeviceButtonRelease(void* ctx,
                                           uint32_t which,
                                           vr::EVRButtonId button,
                                           double time);

    static void trackedDeviceAxisUpdated(void* ctx,
                                         uint32_t which,
                                         uint32_t axis,
                                         const vr::VRControllerAxis_t& state);
};
} // namespace hook4

namespace hook5 {
class VRServerDriverHostHooks : public InterfaceHooks {
public:
    typedef bool (*TrackedDeviceAdded)(void*,
                                       const char*,
                                       vr::ETrackedDeviceClass,
                                       void*);

    typedef void (*TrackedDevicePoseUpdated)(void*,
                                             uint32_t,
                                             const vr::DriverPose_t&,
                                             uint32_t);

    typedef bool (*PollNextEvent)(void*, vr::VREvent_t*, uint32_t);

    static std::shared_ptr<InterfaceHooks> createHooks(void* ptr);

    ~VRServerDriverHostHooks() override;

    static void TrackedDevicePoseUpdatedOriginal(
        void* ctx,
        uint32_t which,
        const vr::DriverPose_t& nextPost,
        uint32_t poseStructureSize);

private:
    bool isHooked = false;

    explicit VRServerDriverHostHooks(void* ptr);

    static HookData<TrackedDeviceAdded> trackedDeviceAddedHook;
    static HookData<TrackedDevicePoseUpdated> trackedDevicePoseUpdatedHook;
    static HookData<PollNextEvent> pollNextEventHook;

    static bool trackedDeviceAdded(void* ctx,
                                   const char* serial,
                                   vr::ETrackedDeviceClass deviceClass,
                                   void* driver);

    static void trackedDevicePoseUpdated(void* ctx,
                                         uint32_t which,
                                         const vr::DriverPose_t& nextPose,
                                         uint32_t poseStructureSize);

    static bool pollNextEvent(void* ctx,
                              vr::VREvent_t* event,
                              uint32_t eventSize);
};
} // namespace hook5

namespace hook6 {
class VRServerDriverHostHooks : public InterfaceHooks {
public:
    typedef bool (*TrackedDeviceAdded)(void*,
                                       const char*,
                                       vr::ETrackedDeviceClass,
                                       void*);

    typedef void (*TrackedDevicePoseUpdated)(void*,
                                             uint32_t,
                                             const vr::DriverPose_t&,
                                             uint32_t);

    typedef bool (*PollNextEvent)(void*, vr::VREvent_t*, uint32_t);

    static std::shared_ptr<InterfaceHooks> createHooks(void* ptr);

    ~VRServerDriverHostHooks() override;

    static void TrackedDevicePoseUpdatedOriginal(
        void* ctx,
        uint32_t which,
        const vr::DriverPose_t& nextPost,
        uint32_t poseStructureSize);

private:
    bool isHooked = false;

    explicit VRServerDriverHostHooks(void* ptr);

    static HookData<TrackedDeviceAdded> trackedDeviceAddedHook;
    static HookData<TrackedDevicePoseUpdated> trackedDevicePoseUpdatedHook;
    static HookData<PollNextEvent> pollNextEventHook;

    static bool trackedDeviceAdded(void* ctx,
                                   const char* serial,
                                   vr::ETrackedDeviceClass deviceClass,
                                   void* driver);

    static void trackedDevicePoseUpdated(void* ctx,
                                         uint32_t which,
                                         const vr::DriverPose_t& nextPose,
                                         uint32_t poseStructureSize);

    static bool pollNextEvent(void* ctx,
                              vr::VREvent_t* event,
                              uint32_t eventSize);
};
} // namespace hook6

} // namespace vrhook

#endif // HIGCALIBRE_VRSERVERDRIVERHOSTHOOKS_H
