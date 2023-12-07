/**
 * @file TrackedDeviceServerDriverHooks.h
 * @author dousha
 * @date 2023/8/2
 */

#ifndef OPENVR_HOOK_TRACKEDDEVICESERVERDRIVERHOOKS_H
#define OPENVR_HOOK_TRACKEDDEVICESERVERDRIVERHOOKS_H

#include <memory>
#include <map>

#include <openvr_driver.h>

#include "common.h"
#include "InterfaceHooks.h"

namespace vrhook {

class TrackedDeviceServerDriverHooks : public InterfaceHooks {
public:
    typedef vr::EVRInitError (*Activate)(void*, uint32_t);

    static std::shared_ptr<InterfaceHooks> createHooks(void* ptr);

    ~TrackedDeviceServerDriverHooks() override;

private:
    HookData<Activate> activateHook;
    void* activateAddress = nullptr;

    explicit TrackedDeviceServerDriverHooks(void* ptr);

    static std::map<void*, HookedAddressMapEntry<Activate>> hookedAddresses;

    static vr::EVRInitError activate(void*, uint32_t);
};

} // namespace hig

#endif // OPENVR_HOOK_TRACKEDDEVICESERVERDRIVERHOOKS_H
