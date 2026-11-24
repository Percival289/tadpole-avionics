#pragma once

#include "../core/types.h"
#include "hover_controller.h"
#include "sim_protocol.h"

class SimulationLink
{
public:
    bool Begin();
    void Update(ImuData& imu);

    bool IsActive() const;
    bool HasFreshSensorData() const;
    const SimState& State() const;
    const SimTarget& Target() const;

private:
    static constexpr size_t ReceiveBufferSize = 1024;
    static constexpr uint32_t SensorTimeoutUs = 500000;
    static constexpr uint32_t ControlPeriodUs = 5000;

    void ReadSerial();
    void ParseFrames();
    void HandleFrame(const sim_hil::FrameHeader& header, const uint8_t* payload);
    void SendActuatorCommand(const SimActuatorCommand& command, uint64_t timestampUs);
    void SendSafeCommand(uint64_t timestampUs);
    void SendHeartbeat(uint64_t timestampUs);

    static uint16_t Crc16(const uint8_t* data, size_t size);
    bool IsFresh(uint32_t nowUs) const;

    bool active_ = false;
    bool haveSensor_ = false;
    bool haveTarget_ = false;
    uint8_t receiveBuffer_[ReceiveBufferSize] = {};
    size_t receiveSize_ = 0;
    uint32_t transmitSequence_ = 0;
    uint32_t lastSensorUs_ = 0;
    uint32_t lastControllerUs_ = 0;
    uint32_t lastHeartbeatUs_ = 0;

    SimState state_;
    SimTarget target_;
    ImuData imu_ = {};
    HoverController controller_;
};
