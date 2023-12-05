/**
 * @file VRDriverInputHooks.h
 * @author dousha
 * @date 2023/7/28
 */

#ifndef HIGCALIBRE_VRDRIVERINPUTHOOKS_H
#define HIGCALIBRE_VRDRIVERINPUTHOOKS_H

#include "InterfaceHooks.h"
#include "common.h"

namespace vrhook {

class VRDriverInputHooks : public InterfaceHooks {
public:
    typedef vr::EVRInputError (*CreateBooleanComponent)(
        void*, vr::PropertyContainerHandle_t, const char*, void*);
    typedef vr::EVRInputError (*UpdateBooleanComponent)(
        void*, vr::VRInputComponentHandle_t, bool, double);
    typedef vr::EVRInputError (*CreateScalarComponent)(
        void*,
        vr::PropertyContainerHandle_t,
        const char*,
        void*,
        vr::EVRScalarType,
        vr::EVRScalarUnits);
    typedef vr::EVRInputError (*UpdateScalarComponent)(
        void*, vr::VRInputComponentHandle_t, float, double);
    typedef vr::EVRInputError (*CreateHapticComponent)(
        void*, vr::PropertyContainerHandle_t, const char*, void*);

    static std::shared_ptr<InterfaceHooks> createHooks(void* ptr);
    ~VRDriverInputHooks() override;

    static vr::EVRInputError UpdateBooleanComponentOriginal(
        void* ctx,
        vr::VRInputComponentHandle_t componentHandle,
        bool value,
        double timeOffset);
    static vr::EVRInputError UpdateScalarComponentOriginal(
        void* ctx,
        vr::VRInputComponentHandle_t componentHandle,
        float value,
        double timeOffset);

private:
    bool isHooked = false;
    explicit VRDriverInputHooks(void* ptr);

    static HookData<CreateBooleanComponent> createBooleanComponentHook;
    static HookData<UpdateBooleanComponent> updateBooleanComponentHook;
    static HookData<CreateScalarComponent> createScalarComponentHook;
    static HookData<UpdateScalarComponent> updateScalarComponentHook;
    static HookData<CreateHapticComponent> createHapticComponentHook;

    static vr::EVRInputError createBooleanComponent(
        void* ctx,
        vr::PropertyContainerHandle_t container,
        const char* name,
        vr::VRInputComponentHandle_t* component);
    static vr::EVRInputError updateBooleanComponent(
        void* ctx,
        vr::VRInputComponentHandle_t componentHandle,
        bool value,
        double timeOffset);
    static vr::EVRInputError createScalarComponent(
        void* ctx,
        vr::PropertyContainerHandle_t container,
        const char* name,
        vr::VRInputComponentHandle_t* component,
        vr::EVRScalarType type,
        vr::EVRScalarUnits units);
    static vr::EVRInputError updateScalarComponent(
        void* ctx,
        vr::VRInputComponentHandle_t componentHandle,
        float value,
        double timeOffset);
    static vr::EVRInputError createHapticComponent(
        void* ctx,
        vr::PropertyContainerHandle_t container,
        const char* name,
        vr::VRInputComponentHandle_t* component);
};

} // namespace hig

#endif // HIGCALIBRE_VRDRIVERINPUTHOOKS_H
