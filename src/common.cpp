/**
 * @file common.cpp
 * @author dousha
 * @date 2023/7/28
 */

#include "common.h"

namespace vrhook {

void** GetVTable(void* ptr) {
    return (*((void***)ptr));
}

} // namespace hig
