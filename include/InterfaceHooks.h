/**
 * @file InterfaceHooks.h
 * @author dousha
 * @date 2023/7/28
 */

#ifndef OPENVR_HOOK_INTERFACEHOOKS_H
#define OPENVR_HOOK_INTERFACEHOOKS_H

#include <memory>
#include <string>

#include "HookEventConsumer.h"

namespace vrhook {
class InterfaceHooks {
public:
    /**
     * @breif Initialize hook facilities.
     * @attention Must be called before any other hooking function. This
     * function shall only be called once.
     * @return `true` if the initialization was successful, `false` otherwise.
     */
    static bool InitHooks();

    /**
     * @brief Clean up hook facilities.
     * @attention This function MUST be called before the application exits.
     */
    static void CleanUpHooks();

    virtual ~InterfaceHooks() = default;

    /**
     * @brief Hook an interface.
     * @param interfaceRef Pointer to the interface
     * @param interfaceVersion OpenVR interface version string
     * @return A shared pointer to the hook object, or `nullptr` if the hook is
     * not supported.
     * @attention Before calling this function, you must call `InitHooks` to
     * initialize the hooking facilities.
     * @see InitHooks
     */
    static std::shared_ptr<InterfaceHooks> HookInterface(
        void* interfaceRef, const std::string& interfaceVersion);

    /**
     * @brief Set the event consumer.
     * @param consumer Pointer to the event consumer.
     * @note This function call be called before `InitHooks`. It is also
     * recommended to call this function before any other hooking function.
     */
    static void SetEventConsumer(HookEventConsumer* consumer);

    static int GetHookErrorCode();

protected:
    static HookEventConsumer* eventConsumer;
    static int hookErrorCode;
};
} // namespace vrhook

#endif // OPENVR_HOOK_INTERFACEHOOKS_H
