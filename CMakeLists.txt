cmake_minimum_required(VERSION 3.25)
project(OpenVRHooks CXX)
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(OpenVR REQUIRED)
find_package(minhook REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(OpenVRHooks STATIC
        src/common.cpp
        include/common.h
        src/InterfaceHooks.cpp
        include/InterfaceHooks.h
        src/TrackedDeviceServerDriverHooks.cpp
        include/TrackedDeviceServerDriverHooks.h
        src/VRControllerComponentHooks.cpp
        include/VRControllerComponentHooks.h
        src/VRDriverContextHooks.cpp
        include/VRDriverContextHooks.h
        src/VRDriverInputHooks.cpp
        include/VRDriverInputHooks.h
        src/VRServerDriverHostHooks.cpp
        include/VRServerDriverHostHooks.h
        src/logger.cpp
        include/logger.h
        include/HookEventConsumer.h)
target_include_directories(OpenVRHooks PUBLIC include)
target_link_libraries(OpenVRHooks PUBLIC OpenVR::OpenVR minhook::minhook spdlog::spdlog_header_only)
