/**
 * @file VRControllerComponentHooks.h
 * @author dousha
 * @date 2023/8/2
 */

#ifndef HIGCALIBRE_VRCONTROLLERCOMPONENTHOOKS_H
#define HIGCALIBRE_VRCONTROLLERCOMPONENTHOOKS_H

#include <memory>
#include <map>

#include <openvr_driver.h>

#include "common.h"
#include "InterfaceHooks.h"

namespace vrhook {

class VRControllerComponentHooks : public InterfaceHooks {
public:
    typedef bool (*TriggerHapticPulse)(void*, uint32_t, uint16_t);

    static std::shared_ptr<InterfaceHooks> createHooks(void* ptr);

    ~VRControllerComponentHooks() override;

    bool TriggerHapticPulseOriginal(uint32_t unAxisId,
                                    uint16_t usPulseDurationMicroseconds);

private:
    HookData<TriggerHapticPulse> triggerHapticPulseHook;
    void* triggerHapticPulseAddress;
    void* controllerComponent = nullptr; // unused?

    explicit VRControllerComponentHooks(void* ptr);

    static std::map<void*, HookedAddressMapEntry<TriggerHapticPulse>>
        hookedAddresses;

    static bool triggerHapticPulse(void* ctx,
                                   uint32_t axis,
                                   uint16_t durationMs);
};

} // namespace hig

#endif // HIGCALIBRE_VRCONTROLLERCOMPONENTHOOKS_H
