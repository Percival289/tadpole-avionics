#pragma once

#include <Arduino.h>

namespace sim_hil
{

static constexpr uint16_t FRAME_MAGIC = 0xA15A;
static constexpr uint8_t PROTOCOL_VERSION = 1;
static constexpr uint16_t MAX_PAYLOAD_SIZE = 512;

enum PacketType : uint8_t
{
    PACKET_SENSOR_DATA = 1,
    PACKET_ACTUATOR_COMMAND = 2,
    PACKET_HEARTBEAT = 3,
    PACKET_SIMULATION_CONTROL = 4
};

#pragma pack(push, 1)

struct FrameHeader
{
    uint16_t magic;
    uint8_t version;
    uint8_t type;
    uint16_t payloadSize;
    uint32_t sequence;
    uint64_t timestampUs;
};

struct SensorPayload
{
    uint32_t vehicleId;
    float positionM[3];
    float velocityMps[3];
    float orientationEulerDeg[3];
    float accelerationBodyMps2[3];
    float angularVelocityBodyRadps[3];
    float altitudeM;
    float pressurePa;
    float temperatureC;
};

struct ActuatorPayload
{
    uint32_t vehicleId;
    float thrustNewtons;
    float gimbalXDegrees;
    float gimbalYDegrees;
    uint32_t flags;
};

struct SimulationControlPayload
{
    uint32_t vehicleId;
    float targetPositionM[3];
    uint32_t flags;
};

struct HeartbeatPayload
{
    uint32_t firmwareVersion;
    uint32_t capabilities;
};

#pragma pack(pop)

} // namespace sim_hil
