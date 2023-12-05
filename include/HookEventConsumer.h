/**
 * @file HookEventConsumer.h
 * @author dousha
 * @date 2023/12/5
 */

#ifndef OPENVRHOOKS_HOOKEVENTCONSUMER_H
#define OPENVRHOOKS_HOOKEVENTCONSUMER_H

#include <memory>

#include <openvr_driver.h>

namespace vrhook {

class HookEventConsumer {
public:
    HookEventConsumer() = default;

    virtual ~HookEventConsumer() = default;

#pragma region Event interface
    /**
     * @brief Called when a device is requested to be added to the system.
     * @param context Pointer to `this`
     * @param version Version of the interface, usually 5 or 6
     * @param serial Serial number of the device
     * @param deviceClass Class of the device
     * @param driver Pointer to the driver instance
     */
    virtual void OnTrackedDeviceAdded(void* context,
                                      int version,
                                      const char* serial,
                                      vr::ETrackedDeviceClass deviceClass,
                                      void* driver)
        = 0;

    /**
     * @brief Called when a device is about to be added to the system.
     *
     * This hook is called BEFORE calling the original device activation
     * function. OpenVR has assigned an ID to the device, but it is yet to
     * be activated.
     *
     * @param context Pointer to `this`
     * @param version Version of the interface, usually 5
     * @param id OpenVR ID of the device
     */
    virtual void OnTrackedDeviceActivate(void* context,
                                         int version,
                                         vr::TrackedDeviceIndex_t id)
        = 0;

    /**
     * @brief Called when a device is activated.
     *
     * This function is called AFTER the original device activation function.
     *
     * @param context Pointer to `this`
     * @param version Version of the interface, usually 5
     * @param id OpenVR ID of the device
     */
    virtual void OnTrackedDeviceActivated(void* context,
                                          int version,
                                          vr::TrackedDeviceIndex_t id)
        = 0;

    /**
     * @brief Called when a device is being told to vibrate.
     *
     * This function is no longer used by any contemporary OpenVR compliant
     * device. It is here for the compatibility with older devices.
     *
     * @param context Pointer to `this`
     * @param version Interface version. Always 1
     * @param axis Which axis is used
     * @param durationMs How long the vibration should be in milliseconds
     * @returns `true` if the vibration should be performed, `false` otherwise.
     */
    virtual bool OnHapticPulseTrigger(void* context,
                                      int version,
                                      uint32_t axis,
                                      uint16_t durationMs) {
        return true;
    }

    /**
     * @brief Called when a boolean component is created for a device.
     *
     * @param context Pointer to `this`
     * @param version Interface version. Always 1.
     * @param container Handle to the property container. A property container
     * is unique to a device.
     * @param name Name of the component. See OpenVR component naming convention
     * @param component Pointer to the component handle
     */
    virtual void OnBooleanComponentCreated(
        void* context,
        int version,
        vr::PropertyContainerHandle_t container,
        const char* name,
        vr::VRInputComponentHandle_t* component)
        = 0;

    /**
     * @brief Called when a scalar component is created for a device.
     *
     * @param context Pointer to `this`
     * @param version Interface version. Always 1.
     * @param container Handle to the property container. A property container
     * is unique to a device.
     * @param name Name of the component. See OpenVR component naming convention
     * @param component Pointer to the component handle
     * @param scalarType Type of the scalar
     * @param scalarUnits Units of the scalar
     */
    virtual void OnScalarComponentCreated(
        void* context,
        int version,
        vr::PropertyContainerHandle_t container,
        const char* name,
        vr::VRInputComponentHandle_t* component,
        vr::EVRScalarType scalarType,
        vr::EVRScalarUnits scalarUnits)
        = 0;

    /**
     * @brief Called when a haptic component is created.
     * @param context Pointer to `this`
     * @param version Interface version. Always 1.
     * @param container Handle to the property container. A property container
     * is unique to a device.
     * @param name Name of the component. See OpenVR component naming convention
     * @param component Pointer to the component handle
     */
    virtual void OnHapticComponentCreated(
        void* context,
        int version,
        vr::PropertyContainerHandle_t container,
        const char* name,
        vr::VRInputComponentHandle_t* component)
        = 0;

    /**
     * @brief Called when a boolean component is about to be updated.
     * @param context Pointer to `this`
     * @param version Interface version. Always 1.
     * @param componentHandle Handle to the component
     * @param value New value of the component
     * @param timeOffset Time offset in seconds
     * @return `true` if the component should be updated, `false` otherwise.
     */
    virtual bool OnBooleanComponentUpdate(
        void* context,
        int version,
        vr::VRInputComponentHandle_t componentHandle,
        bool value,
        double timeOffset)
        = 0;

    /**
     * @brief Called when a scalar component is about to be updated.
     * @param context Pointer to `this`
     * @param version Interface version. Always 1.
     * @param componentHandle Handle to the component
     * @param value New value of the component
     * @param timeOffset Time offset in seconds
     * @return `true` if the component should be updated, `false` otherwise.
     */
    virtual bool OnScalarComponentUpdate(
        void* context,
        int version,
        vr::VRInputComponentHandle_t componentHandle,
        float value,
        double timeOffset)
        = 0;

    virtual bool OnTrackedDevicePoseUpdated(void* context,
                                            int version,
                                            vr::TrackedDeviceIndex_t id,
                                            const vr::DriverPose_t& pose,
                                            uint32_t poseStructureSize)
        = 0;

    virtual std::pair<std::shared_ptr<void>, uint32_t>
    GetDriverEventForInjection(void* serverDriverHost) = 0;

    virtual bool OnPollNextEvent(void* context,
                                 int version,
                                 vr::VREvent_t* event,
                                 uint32_t sizeOfEvent)
        = 0;

#pragma region Legacy support
    virtual bool OnTrackedDeviceButtonMake(void* context,
                                           int version,
                                           vr::TrackedDeviceIndex_t id,
                                           vr::EVRButtonId button,
                                           double timeOffset)
        = 0;

    virtual bool OnTrackedDeviceButtonBreak(void* context,
                                            int version,
                                            vr::TrackedDeviceIndex_t id,
                                            vr::EVRButtonId button,
                                            double timeOffset)
        = 0;

    virtual bool OnTrackedDeviceButtonContact(void* context,
                                              int version,
                                              vr::TrackedDeviceIndex_t id,
                                              vr::EVRButtonId button,
                                              double timeOffset)
        = 0;

    virtual bool OnTrackedDeviceButtonRelease(void* context,
                                              int version,
                                              vr::TrackedDeviceIndex_t id,
                                              vr::EVRButtonId button,
                                              double timeOffset)
        = 0;

    virtual bool OnTrackedDeviceAxisUpdated(void* context,
                                            int version,
                                            vr::TrackedDeviceIndex_t id,
                                            uint32_t axis,
                                            vr::VRControllerAxis_t& value)
        = 0;
#pragma endregion
#pragma endregion
};

} // namespace vrhook

#endif // OPENVRHOOKS_HOOKEVENTCONSUMER_H
