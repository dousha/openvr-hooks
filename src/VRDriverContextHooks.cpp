/**
 * @file VRDriverContextHooks.cpp
 * @author dousha
 * @date 2023/7/28
 */

#include "VRDriverContextHooks.h"

namespace vrhook {

HookData<VRDriverContextHooks::getGenericInterface_t>
    VRDriverContextHooks::getGenericInterfaceHook;
std::unordered_map<std::string, std::shared_ptr<InterfaceHooks>>
    VRDriverContextHooks::hookedInterfaces;

VRDriverContextHooks::VRDriverContextHooks(void* ptr) {
    if (!isHooked) {
        ObtainLogger()->trace("Hooking IVRDriverContext::GetGenericInterface");
        CreateHook(getGenericInterfaceHook,
                   reinterpret_cast<void*>(getGenericInterface),
                   "IVRDriverContext::GetGenericInterface",
                   ptr,
                   0);
        isHooked = true;
    }
}

VRDriverContextHooks::~VRDriverContextHooks() {
    if (isHooked) {
        RemoveHook(getGenericInterfaceHook);
        isHooked = false;
    }
}

std::shared_ptr<InterfaceHooks> VRDriverContextHooks::createHooks(void* ptr) {
    return std::shared_ptr<InterfaceHooks>(new VRDriverContextHooks(ptr));
}

std::shared_ptr<InterfaceHooks> VRDriverContextHooks::getInterfaceHook(
    const std::string& interfaceVersion) {
    auto it = hookedInterfaces.find(interfaceVersion);
    if (it != hookedInterfaces.end()) {
        return it->second;
    }
    return nullptr;
}

void* VRDriverContextHooks::getGenericInterface(vr::IVRDriverContext* context,
                                                const char* interfaceVersion,
                                                vr::EVRInitError* error) {
    auto result = getGenericInterfaceHook.originalFunc(
        context, interfaceVersion, error);
    if (hookedInterfaces.find(interfaceVersion) == hookedInterfaces.end()) {
        auto hooks = InterfaceHooks::HookInterface(result, interfaceVersion);
        if (hooks != nullptr) {
            hookedInterfaces.insert({std::string(interfaceVersion), hooks});
        }
    }
    return result;
}
} // namespace vrhook
