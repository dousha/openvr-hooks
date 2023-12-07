/**
 * @file common.h
 * @author dousha
 * @date 2023/7/28
 */

#ifndef OPENVR_HOOK_COMMON_H
#define OPENVR_HOOK_COMMON_H

#include <openvr_driver.h>

#include <MinHook.h>

#include "logger.h"

namespace vrhook {

template <typename T> struct HookData {
    bool enabled = false;
    void* targetFunc = nullptr;
    T originalFunc = nullptr;
};

template <typename T>
void CreateHook(HookData<T>& detourInfo,
                void* detourFunc,
                const char* logName,
                void* objPtr,
                ptrdiff_t vtableOffset) {
    detourInfo.targetFunc = GetVTable(objPtr)[vtableOffset];
    ObtainLogger()->trace("Extracted target function is {}",
                          detourInfo.targetFunc);
    MH_STATUS mhError
        = MH_CreateHook(detourInfo.targetFunc,
                        detourFunc,
                        reinterpret_cast<LPVOID*>(&detourInfo.originalFunc));
    ObtainLogger()->trace("Trampoline function is {}",
                          reinterpret_cast<void*>(detourInfo.originalFunc));
    if (mhError == MH_OK) {
        mhError = MH_EnableHook(detourInfo.targetFunc);
        if (mhError == MH_OK) {
            detourInfo.enabled = true;
            ObtainLogger()->info(
                "{} is enabled, address {}", logName, detourInfo.targetFunc);
        } else {
            MH_RemoveHook(detourInfo.targetFunc);
            ObtainLogger()->error("{} cannot be enabled, reason: {}",
                                  logName,
                                  MH_StatusToString(mhError));
        }
    } else {
        ObtainLogger()->error("cannot create windowEventHook {}, reason: {}",
                              logName,
                              MH_StatusToString(mhError));
    }
}

template <typename T> void RemoveHook(HookData<T>& detourInfo) {
    if (detourInfo.enabled) {
        MH_RemoveHook(detourInfo.targetFunc);
        detourInfo.enabled = false;
    }
}

template <typename T> struct HookedAddressMapEntry {
    unsigned useCount = 0;
    HookData<T> hookData;
};

extern void** GetVTable(void* ptr);
} // namespace hig

#endif // OPENVR_HOOK_COMMON_H
