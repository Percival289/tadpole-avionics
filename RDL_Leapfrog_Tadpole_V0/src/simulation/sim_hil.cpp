#include "sim_hil.h"
#include "sim_config.h"

#include <string.h>

bool SimulationLink::Begin()
{
    Serial.begin(921600);
    active_ = true;
    receiveSize_ = 0;
    haveSensor_ = false;
    haveTarget_ = false;
    controller_.Reset();
    return true;
}

void SimulationLink::Update(ImuData& imu)
{
    if (!active_)
        return;

    ReadSerial();
    ParseFrames();

    const uint32_t nowUs = micros();
    if (lastHeartbeatUs_ == 0 || static_cast<uint32_t>(nowUs - lastHeartbeatUs_) >= 500000)
    {
        SendHeartbeat(nowUs);
        lastHeartbeatUs_ = nowUs;
    }

    if (!IsFresh(nowUs))
    {
        if (lastControllerUs_ == 0 ||
            static_cast<uint32_t>(nowUs - lastControllerUs_) >= ControlPeriodUs)
        {
            SendSafeCommand(nowUs);
            lastControllerUs_ = nowUs;
        }
        return;
    }

    if (!haveTarget_ || static_cast<uint32_t>(nowUs - lastControllerUs_) < ControlPeriodUs)
    {
        imu = imu_;
        return;
    }

    float dt = static_cast<float>(nowUs - lastControllerUs_) * 0.000001f;
    lastControllerUs_ = nowUs;

    const SimActuatorCommand command = controller_.Update(state_, target_, dt);
    SendActuatorCommand(command, nowUs);
    imu = imu_;
}

bool SimulationLink::IsActive() const
{
    return active_;
}

bool SimulationLink::HasFreshSensorData() const
{
    return haveSensor_ && IsFresh(micros());
}

const SimState& SimulationLink::State() const
{
    return state_;
}

const SimTarget& SimulationLink::Target() const
{
    return target_;
}

void SimulationLink::ReadSerial()
{
    while (Serial.available() > 0)
    {
        const uint8_t byte = static_cast<uint8_t>(Serial.read());
        if (receiveSize_ >= ReceiveBufferSize)
            receiveSize_ = 0;
        receiveBuffer_[receiveSize_++] = byte;
    }
}

void SimulationLink::ParseFrames()
{
    while (receiveSize_ >= sizeof(sim_hil::FrameHeader) + sizeof(uint16_t))
    {
        sim_hil::FrameHeader header{};
        memcpy(&header, receiveBuffer_, sizeof(header));

        if (header.magic != sim_hil::FRAME_MAGIC ||
            header.version != sim_hil::PROTOCOL_VERSION ||
            header.payloadSize > sim_hil::MAX_PAYLOAD_SIZE)
        {
            memmove(receiveBuffer_, receiveBuffer_ + 1, --receiveSize_);
            continue;
        }

        const size_t frameSize = sizeof(header) + header.payloadSize + sizeof(uint16_t);
        if (receiveSize_ < frameSize)
            return;

        uint16_t receivedCrc = 0;
        memcpy(&receivedCrc, receiveBuffer_ + sizeof(header) + header.payloadSize, sizeof(receivedCrc));
        if (receivedCrc != Crc16(receiveBuffer_, sizeof(header) + header.payloadSize))
        {
            memmove(receiveBuffer_, receiveBuffer_ + 1, --receiveSize_);
            continue;
        }

        HandleFrame(header, receiveBuffer_ + sizeof(header));
        receiveSize_ -= frameSize;
        memmove(receiveBuffer_, receiveBuffer_ + frameSize, receiveSize_);
    }
}

void SimulationLink::HandleFrame(
    const sim_hil::FrameHeader& header,
    const uint8_t* payload
)
{
    if (header.type == sim_hil::PACKET_SENSOR_DATA &&
        header.payloadSize == sizeof(sim_hil::SensorPayload))
    {
        sim_hil::SensorPayload sensor{};
        memcpy(&sensor, payload, sizeof(sensor));

        state_.vehicleId = sensor.vehicleId;
        memcpy(state_.positionM, sensor.positionM, sizeof(state_.positionM));
        memcpy(state_.velocityMps, sensor.velocityMps, sizeof(state_.velocityMps));
        memcpy(state_.orientationEulerDeg, sensor.orientationEulerDeg, sizeof(state_.orientationEulerDeg));
        memcpy(state_.accelerationBodyMps2, sensor.accelerationBodyMps2, sizeof(state_.accelerationBodyMps2));
        memcpy(state_.angularVelocityBodyRadps, sensor.angularVelocityBodyRadps, sizeof(state_.angularVelocityBodyRadps));
        state_.altitudeM = sensor.altitudeM;
        state_.pressurePa = sensor.pressurePa;
        state_.temperatureC = sensor.temperatureC;

        imu_.ax = sensor.accelerationBodyMps2[0];
        imu_.ay = sensor.accelerationBodyMps2[1];
        imu_.az = sensor.accelerationBodyMps2[2];
        imu_.gx = sensor.angularVelocityBodyRadps[0];
        imu_.gy = sensor.angularVelocityBodyRadps[1];
        imu_.gz = sensor.angularVelocityBodyRadps[2];

        haveSensor_ = true;
        lastSensorUs_ = micros();
        if (lastControllerUs_ == 0)
            lastControllerUs_ = lastSensorUs_;
    }
    else if (header.type == sim_hil::PACKET_SIMULATION_CONTROL &&
             header.payloadSize == sizeof(sim_hil::SimulationControlPayload))
    {
        sim_hil::SimulationControlPayload control{};
        memcpy(&control, payload, sizeof(control));
        target_.positionM[0] = control.targetPositionM[0];
        target_.positionM[1] = control.targetPositionM[1];
        target_.positionM[2] = control.targetPositionM[2];
        haveTarget_ = true;
    }
}

void SimulationLink::SendActuatorCommand(
    const SimActuatorCommand& command,
    uint64_t timestampUs
)
{
    sim_hil::ActuatorPayload payload{
        state_.vehicleId,
        command.thrustNewtons,
        command.gimbalXDegrees,
        command.gimbalYDegrees,
        0u
    };

    sim_hil::FrameHeader header{
        sim_hil::FRAME_MAGIC,
        sim_hil::PROTOCOL_VERSION,
        sim_hil::PACKET_ACTUATOR_COMMAND,
        sizeof(payload),
        transmitSequence_++,
        timestampUs
    };

    uint8_t frame[sizeof(header) + sizeof(payload) + sizeof(uint16_t)]{};
    memcpy(frame, &header, sizeof(header));
    memcpy(frame + sizeof(header), &payload, sizeof(payload));
    const uint16_t crc = Crc16(frame, sizeof(header) + sizeof(payload));
    memcpy(frame + sizeof(header) + sizeof(payload), &crc, sizeof(crc));
    Serial.write(frame, sizeof(frame));
}

void SimulationLink::SendSafeCommand(uint64_t timestampUs)
{
    const SimActuatorCommand safe{};
    SendActuatorCommand(safe, timestampUs);
}

void SimulationLink::SendHeartbeat(uint64_t timestampUs)
{
    sim_hil::HeartbeatPayload payload{
        TADPOLE_SIM_FIRMWARE_VERSION,
        TADPOLE_SIM_CAPABILITIES
    };
    sim_hil::FrameHeader header{
        sim_hil::FRAME_MAGIC,
        sim_hil::PROTOCOL_VERSION,
        sim_hil::PACKET_HEARTBEAT,
        sizeof(payload),
        transmitSequence_++,
        timestampUs
    };

    uint8_t frame[sizeof(header) + sizeof(payload) + sizeof(uint16_t)]{};
    memcpy(frame, &header, sizeof(header));
    memcpy(frame + sizeof(header), &payload, sizeof(payload));
    const uint16_t crc = Crc16(frame, sizeof(header) + sizeof(payload));
    memcpy(frame + sizeof(header) + sizeof(payload), &crc, sizeof(crc));
    Serial.write(frame, sizeof(frame));
}

bool SimulationLink::IsFresh(uint32_t nowUs) const
{
    return haveSensor_ && static_cast<uint32_t>(nowUs - lastSensorUs_) <= SensorTimeoutUs;
}

uint16_t SimulationLink::Crc16(const uint8_t* data, size_t size)
{
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < size; ++i)
    {
        crc ^= static_cast<uint16_t>(data[i]) << 8;
        for (uint8_t bit = 0; bit < 8; ++bit)
            crc = (crc & 0x8000) ? static_cast<uint16_t>((crc << 1) ^ 0x1021) : static_cast<uint16_t>(crc << 1);
    }
    return crc;
}
