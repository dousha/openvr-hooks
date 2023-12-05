/**
 * @file InterfaceHooks.h
 * @author dousha
 * @date 2023/7/28
 */

#ifndef HIGCALIBRE_INTERFACEHOOKS_H
#define HIGCALIBRE_INTERFACEHOOKS_H

#include <memory>
#include <string>

#include "HookEventConsumer.h"

namespace vrhook {
class InterfaceHooks {
public:
    virtual ~InterfaceHooks() = default;

    static std::shared_ptr<InterfaceHooks> HookInterface(
        void* interfaceRef, const std::string& interfaceVersion);

    static void SetEventConsumer(HookEventConsumer* consumer);

protected:
    static HookEventConsumer* eventConsumer;
};
} // namespace hig

#endif // HIGCALIBRE_INTERFACEHOOKS_H
