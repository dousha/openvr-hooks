/**
 * @file InterfaceHooks.cpp
 * @author dousha
 * @date 2023/7/28
 */

#include "InterfaceHooks.h"
#include "VRDriverContextHooks.h"
#include "VRDriverInputHooks.h"
#include "VRServerDriverHostHooks.h"
#include "VRControllerComponentHooks.h"
#include "TrackedDeviceServerDriverHooks.h"
#include "HookEventConsumer.h"

namespace vrhook {

HookEventConsumer* InterfaceHooks::eventConsumer = nullptr;

std::shared_ptr<InterfaceHooks> vrhook::InterfaceHooks::HookInterface(
    void* interfaceRef, const std::string& interfaceVersion) {
    std::shared_ptr<InterfaceHooks> result = nullptr;

    if (interfaceVersion == "IVRDriverContext") {
        ObtainLogger()->info("Hooking on interface {}", interfaceVersion);
        result = VRDriverContextHooks::createHooks(interfaceRef);
    } else if (interfaceVersion == "IVRServerDriverHost_004") {
        // NOTE: this branch is never invoked in the current version of OpenVR
        // it's still here for compatibility reasons. It might be already
        // removed.
        ObtainLogger()->warn("IVRServerDriverHost_004 is deprecated, but was "
                             "requested to be created.");
        result = hook4::VRServerDriverHostHooks::createHooks(interfaceRef);
    } else if (interfaceVersion == "IVRServerDriverHost_005") {
        ObtainLogger()->info("IVRServerDriverHost_005 is requested to be "
                             "created.");
        result = hook5::VRServerDriverHostHooks::createHooks(interfaceRef);
    } else if (interfaceVersion == "IVRServerDriverHost_006") {
        ObtainLogger()->info("IVRServerDriverHost_006 is requested to be "
                             "created.");
        result = hook6::VRServerDriverHostHooks::createHooks(interfaceRef);
    } else if (interfaceVersion == "IVRDriverInput_001"
               || interfaceVersion == "IVRDriverInput_003") {
        ObtainLogger()->info("Hooking on interface {}", interfaceVersion);
        result = VRDriverInputHooks::createHooks(interfaceRef);
    } else if (interfaceVersion == "ITrackedDeviceServerDriver_005") {
        ObtainLogger()->info("Hooking on interface {}", interfaceVersion);
        result = TrackedDeviceServerDriverHooks::createHooks(interfaceRef);
    } else if (interfaceVersion == "IVRControllerComponent_001") {
        // NOTE: this branch is never invoked in the current version of OpenVR
        // it's basically used to trigger haptic pulses, so, not really needed
        // we will have to migrate to IVRDriverInput at some point anyway
        ObtainLogger()->info("Hooking on interface {}", interfaceVersion);
        result = VRControllerComponentHooks::createHooks(interfaceRef);
    }

    return result;
}

void InterfaceHooks::SetEventConsumer(HookEventConsumer* consumer) {
    eventConsumer = consumer;
}

bool InterfaceHooks::InitHooks() {
    auto hookError = MH_Initialize();
    if (hookError != MH_OK) {
        return false;
    }

    return true;
}

void InterfaceHooks::CleanUpHooks() {
    MH_Uninitialize();
}

} // namespace vrhook
