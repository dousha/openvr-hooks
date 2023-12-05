/**
 * @file TrackedDeviceServerDriverHooks.cpp
 * @author dousha
 * @date 2023/8/2
 */

#include "TrackedDeviceServerDriverHooks.h"

namespace vrhook {
std::map<void*, HookedAddressMapEntry<TrackedDeviceServerDriverHooks::Activate>>
    TrackedDeviceServerDriverHooks::hookedAddresses;

TrackedDeviceServerDriverHooks::TrackedDeviceServerDriverHooks(void* ptr) {
    ObtainLogger()->trace("TrackedDeviceServerDriverHooks::"
                          "TrackedDeviceServerDriverHooks: ptr = {}",
                          ptr);
    auto vTable = GetVTable(ptr);
    activateAddress = vTable[0];
    auto it = hookedAddresses.find(activateAddress);
    if (it == hookedAddresses.end()) {
        CreateHook(activateHook,
                   reinterpret_cast<void*>(activate),
                   "TrackedDeviceServerDriverHooks::Activate",
                   ptr,
                   0);
        hookedAddresses[activateAddress].useCount = 1;
        hookedAddresses[activateAddress].hookData = activateHook;
    } else {
        activateHook = it->second.hookData;
        it->second.useCount += 1;
    }
}

TrackedDeviceServerDriverHooks::~TrackedDeviceServerDriverHooks() {
    auto it = hookedAddresses.find(activateAddress);
    if (it == hookedAddresses.end()) {
        return;
    }

    if (it->second.useCount <= 1) {
        RemoveHook(it->second.hookData);
        hookedAddresses.erase(it);
    } else {
        it->second.useCount -= 1;
    }
}

std::shared_ptr<InterfaceHooks> TrackedDeviceServerDriverHooks::createHooks(
    void* ptr) {
    return std::shared_ptr<InterfaceHooks>(
        new TrackedDeviceServerDriverHooks(ptr));
}

vr::EVRInitError TrackedDeviceServerDriverHooks::activate(void* ctx,
                                                          uint32_t id) {
    ObtainLogger()->trace(
        "TrackedDeviceServerDriverHooks::activate: ctx = {}, openVrId = {}",
        ctx,
        id);
    auto vTable = GetVTable(ctx);
    auto activateAddress = vTable[0];
    auto it = hookedAddresses.find(activateAddress);
    if (it != hookedAddresses.end()) {
        eventConsumer->OnTrackedDeviceActivate(ctx, 5, id);
        auto result = it->second.hookData.originalFunc(ctx, id);
        if (result == vr::VRInitError_None) {
            eventConsumer->OnTrackedDeviceActivated(ctx, 5, id);
        }
        return result;
    } else {
        ObtainLogger()->error("TrackedDeviceServerDriverHooks::activate: "
                              "activating a device that is not registered");
        return vr::VRInitError_Unknown;
    }
}
} // namespace hig
