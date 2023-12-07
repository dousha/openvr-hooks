/**
 * @file VRDriverInputHooks.cpp
 * @author dousha
 * @date 2023/7/28
 */

#include "VRDriverInputHooks.h"

namespace vrhook {

HookData<VRDriverInputHooks::CreateBooleanComponent>
    VRDriverInputHooks::createBooleanComponentHook;
HookData<VRDriverInputHooks::CreateScalarComponent>
    VRDriverInputHooks::createScalarComponentHook;
HookData<VRDriverInputHooks::CreateHapticComponent>
    VRDriverInputHooks::createHapticComponentHook;
HookData<VRDriverInputHooks::UpdateBooleanComponent>
    VRDriverInputHooks::updateBooleanComponentHook;
HookData<VRDriverInputHooks::UpdateScalarComponent>
    VRDriverInputHooks::updateScalarComponentHook;

VRDriverInputHooks::VRDriverInputHooks(void* ptr) {
    if (isHooked) {
        ObtainLogger()->warn("VRDriverInputHooks is already hooked");
        return;
    }

    CreateHook(createBooleanComponentHook,
               reinterpret_cast<void*>(createBooleanComponent),
               "VRDriverInputHooks::CreateBooleanComponent",
               ptr,
               0);
    CreateHook(updateBooleanComponentHook,
               reinterpret_cast<void*>(updateBooleanComponent),
               "VRDriverInputHooks::UpdateBooleanComponent",
               ptr,
               1);
    CreateHook(createScalarComponentHook,
               reinterpret_cast<void*>(createScalarComponent),
               "VRDriverInputHooks::CreateScalarComponent",
               ptr,
               2);
    CreateHook(updateScalarComponentHook,
               reinterpret_cast<void*>(updateScalarComponent),
               "VRDriverInputHooks::UpdateScalarComponent",
               ptr,
               3);
    CreateHook(createHapticComponentHook,
               reinterpret_cast<void*>(createHapticComponent),
               "VRDriverInputHooks::CreateHapticComponent",
               ptr,
               4);
    isHooked = true;
}

VRDriverInputHooks::~VRDriverInputHooks() {
    if (!isHooked) {
        return;
    }

    RemoveHook(createBooleanComponentHook);
    RemoveHook(updateBooleanComponentHook);
    RemoveHook(createScalarComponentHook);
    RemoveHook(updateScalarComponentHook);
    RemoveHook(createHapticComponentHook);
    isHooked = false;
}

std::shared_ptr<InterfaceHooks> VRDriverInputHooks::createHooks(void* ptr) {
    std::shared_ptr<InterfaceHooks> result
        = std::shared_ptr<InterfaceHooks>(new VRDriverInputHooks(ptr));
    return result;
}

vr::EVRInputError VRDriverInputHooks::createBooleanComponent(
    void* ctx,
    vr::PropertyContainerHandle_t container,
    const char* name,
    vr::VRInputComponentHandle_t* component) {
    auto result = createBooleanComponentHook.originalFunc(
        ctx, container, name, component);
    if (result == vr::VRInputError_None) {
        eventConsumer->OnBooleanComponentCreated(
            ctx, 1, container, name, component);
    }

    return result;
}

vr::EVRInputError VRDriverInputHooks::updateBooleanComponent(
    void* ctx,
    vr::VRInputComponentHandle_t componentHandle,
    bool value,
    double timeOffset) {
    auto result = vr::VRInputError_None;
    // let server driver handle the component directly
    // even if it means something strange
    if (eventConsumer->OnBooleanComponentUpdate(
            ctx, 1, componentHandle, value, timeOffset)) {
        // server driver did not handle the component, let the original function
        // handle it
        result = updateBooleanComponentHook.originalFunc(
            ctx, componentHandle, value, timeOffset);
    }

    return result;
}

vr::EVRInputError VRDriverInputHooks::createScalarComponent(
    void* ctx,
    vr::PropertyContainerHandle_t container,
    const char* name,
    vr::VRInputComponentHandle_t* component,
    vr::EVRScalarType scalarType,
    vr::EVRScalarUnits scalarUnits) {
    auto result = createScalarComponentHook.originalFunc(
        ctx, container, name, component, scalarType, scalarUnits);
    if (result == vr::VRInputError_None) {
        eventConsumer->OnScalarComponentCreated(
            ctx, 1, container, name, component, scalarType, scalarUnits);
    }

    return result;
}

vr::EVRInputError VRDriverInputHooks::updateScalarComponent(
    void* ctx,
    vr::VRInputComponentHandle_t componentHandle,
    float value,
    double timeOffset) {
    auto result = vr::VRInputError_None;
    // let server driver handle the component directly
    // even if it means something strange
    if (eventConsumer->OnScalarComponentUpdate(
            ctx, 1, componentHandle, value, timeOffset)) {
        // server driver did not handle the component, let the original function
        // handle it
        result = updateScalarComponentHook.originalFunc(
            ctx, componentHandle, value, timeOffset);
    }

    return result;
}

vr::EVRInputError VRDriverInputHooks::createHapticComponent(
    void* ctx,
    vr::PropertyContainerHandle_t container,
    const char* name,
    vr::VRInputComponentHandle_t* component) {
    auto result = createHapticComponentHook.originalFunc(
        ctx, container, name, component);
    if (result == vr::VRInputError_None) {
        eventConsumer->OnHapticComponentCreated(
            ctx, 1, container, name, component);
    }

    return result;
}

vr::EVRInputError VRDriverInputHooks::UpdateBooleanComponentOriginal(
    void* ctx,
    vr::VRInputComponentHandle_t componentHandle,
    bool value,
    double timeOffset) {
    return updateBooleanComponentHook.originalFunc(
        ctx, componentHandle, value, timeOffset);
}

vr::EVRInputError VRDriverInputHooks::UpdateScalarComponentOriginal(
    void* ctx,
    vr::VRInputComponentHandle_t componentHandle,
    float value,
    double timeOffset) {
    return updateScalarComponentHook.originalFunc(
        ctx, componentHandle, value, timeOffset);
}

} // namespace vrhook
