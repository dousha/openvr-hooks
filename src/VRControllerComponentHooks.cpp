/**
 * @file VRControllerComponentHooks.cpp
 * @author dousha
 * @date 2023/8/2
 */

#include "VRControllerComponentHooks.h"

namespace vrhook {
std::map<void*,
         HookedAddressMapEntry<VRControllerComponentHooks::TriggerHapticPulse>>
    VRControllerComponentHooks::hookedAddresses;

VRControllerComponentHooks::VRControllerComponentHooks(void* ptr) {
    auto vTable = GetVTable(ptr);
    triggerHapticPulseAddress = vTable[1];
    auto it = hookedAddresses.find(triggerHapticPulseAddress);
    if (it == hookedAddresses.end()) {
        CreateHook(triggerHapticPulseHook,
                   triggerHapticPulseAddress,
                   "TriggerHapticPulse",
                   ptr,
                   0);
        hookedAddresses.insert(
            {triggerHapticPulseAddress, {1, triggerHapticPulseHook}});
    } else {
        triggerHapticPulseHook = it->second.hookData;
        it->second.useCount += 1;
    }
}

VRControllerComponentHooks::~VRControllerComponentHooks() {
    auto it = hookedAddresses.find(triggerHapticPulseAddress);
    if (it != hookedAddresses.end()) {
        if (it->second.useCount <= 1) {
            RemoveHook(it->second.hookData);
            hookedAddresses.erase(it);
        } else {
            it->second.useCount -= 1;
        }
    }
}

std::shared_ptr<InterfaceHooks> VRControllerComponentHooks::createHooks(
    void* ptr) {
    auto result
        = std::shared_ptr<InterfaceHooks>(new VRControllerComponentHooks(ptr));
    return result;
}

bool VRControllerComponentHooks::TriggerHapticPulseOriginal(
    uint32_t unAxisId, uint16_t usPulseDurationMicroseconds) {
    return triggerHapticPulseHook.originalFunc(
        controllerComponent, unAxisId, usPulseDurationMicroseconds);
}

bool VRControllerComponentHooks::triggerHapticPulse(void* ctx,
                                                    uint32_t axis,
                                                    uint16_t durationMs) {
    if (eventConsumer->OnHapticPulseTrigger(ctx, 1, axis, durationMs)) {
        auto vTable = (*(void***)ctx);
        auto triggerHapticAddress = vTable[1];
        auto it = hookedAddresses.find(triggerHapticAddress);

        if (it != hookedAddresses.end()) {
            return it->second.hookData.originalFunc(ctx, axis, durationMs);
        } else {
            ObtainLogger()->error("triggerHapticPulse windowEventHook not found");
        }
    }

    return true;
}
} // namespace hig