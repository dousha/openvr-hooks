/**
 * @file VRDriverContextHooks.h
 * @author dousha
 * @date 2023/7/28
 */

#ifndef OPENVR_HOOK_VRDRIVERCONTEXTHOOKS_H
#define OPENVR_HOOK_VRDRIVERCONTEXTHOOKS_H

#include <openvr_driver.h>

#include <unordered_map>

#include "HookEventConsumer.h"
#include "InterfaceHooks.h"
#include "common.h"

namespace vrhook {

class VRDriverContextHooks : public InterfaceHooks {
public:
    typedef void* (*getGenericInterface_t)(vr::IVRDriverContext*,
                                           const char* interfaceVersion,
                                           vr::EVRInitError* error);

    static std::shared_ptr<InterfaceHooks> getInterfaceHook(
        const std::string& interfaceVersion);

    static std::shared_ptr<InterfaceHooks> createHooks(void* ptr);

    ~VRDriverContextHooks() override;

private:
    bool isHooked = false;

    explicit VRDriverContextHooks(void* ptr);

    static HookData<getGenericInterface_t> getGenericInterfaceHook;

    static std::unordered_map<std::string, std::shared_ptr<InterfaceHooks>>
        hookedInterfaces;

    static void* getGenericInterface(vr::IVRDriverContext* context,
                                     const char* interfaceVersion,
                                     vr::EVRInitError* error);
};

} // namespace vrhook

#endif // OPENVR_HOOK_VRDRIVERCONTEXTHOOKS_H
