#include "hover_controller.h"
#include "sim_config.h"

#include <math.h>

namespace
{
constexpr float GravityMps2 = 9.81f;
constexpr float VehicleMassKg = TADPOLE_SIM_VEHICLE_MASS_KG;
constexpr float MaximumThrustN = TADPOLE_SIM_MAX_THRUST_N;
constexpr float MaximumGimbalDegrees = TADPOLE_SIM_MAX_GIMBAL_DEGREES;
constexpr float MaximumTiltDegrees = 8.0f;
constexpr float MaximumTiltRateDegreesPerSecond = 18.0f;
constexpr float DegreesPerRadian = 57.295779513f;
}

void HoverController::Reset()
{
    integralX_ = 0.0f;
    integralY_ = 0.0f;
    integralZ_ = 0.0f;
    commandedTiltXDegrees_ = 0.0f;
    commandedTiltZDegrees_ = 0.0f;
}

SimActuatorCommand HoverController::Update(
    const SimState& state,
    const SimTarget& target,
    float dt
)
{
    dt = Clamp(dt, 0.001f, 0.05f);

    const float desiredAccelerationX = UpdateAxis(
        target.positionM[0] - state.positionM[0], state.velocityMps[0],
        dt, 0.8f, 0.01f, 2.4f, 1.0f, integralX_
    );
    const float desiredAccelerationY = UpdateAxis(
        target.positionM[1] - state.positionM[1], state.velocityMps[1],
        dt, 2.4f, 0.03f, 3.2f, 1.5f, integralY_
    );
    const float desiredAccelerationZ = UpdateAxis(
        target.positionM[2] - state.positionM[2], state.velocityMps[2],
        dt, 0.8f, 0.01f, 2.4f, 1.0f, integralZ_
    );

    const float requestedVerticalAcceleration = GravityMps2 + desiredAccelerationY;
    const float safeVerticalAcceleration = requestedVerticalAcceleration > 0.5f
        ? requestedVerticalAcceleration
        : 0.5f;

    // The outer position loop commands the vehicle tilt required to create the
    // requested world-frame lateral acceleration. It does not directly add a
    // lateral gimbal command: the inner attitude loop uses TVC to achieve and
    // hold this attitude, which avoids fighting the vehicle dynamics.
    float targetTiltX = atanf(
        desiredAccelerationZ / safeVerticalAcceleration
    ) * DegreesPerRadian;
    float targetTiltZ = -atanf(
        desiredAccelerationX / safeVerticalAcceleration
    ) * DegreesPerRadian;
    const float targetTiltMagnitude = sqrtf(
        targetTiltX * targetTiltX + targetTiltZ * targetTiltZ
    );
    if (targetTiltMagnitude > MaximumTiltDegrees)
    {
        const float scale = MaximumTiltDegrees / targetTiltMagnitude;
        targetTiltX *= scale;
        targetTiltZ *= scale;
    }

    // A commanded attitude cannot jump instantly. Slew the target tilt so a
    // position-slider step does not demand an unrecoverable TVC maneuver.
    const float maximumTiltStep = MaximumTiltRateDegreesPerSecond * dt;
    commandedTiltXDegrees_ = MoveToward(
        commandedTiltXDegrees_, targetTiltX, maximumTiltStep
    );
    commandedTiltZDegrees_ = MoveToward(
        commandedTiltZDegrees_, targetTiltZ, maximumTiltStep
    );

    SimActuatorCommand output;
    // The engine is below the center of mass. A positive attitude error
    // requires a gimbal command that produces an opposing TVC moment.
    const float angularVelocityXDegrees = state.angularVelocityBodyRadps[0] * DegreesPerRadian;
    const float angularVelocityZDegrees = state.angularVelocityBodyRadps[2] * DegreesPerRadian;
    constexpr float AttitudeGain = 0.75f;
    constexpr float RateGain = 0.45f;
    float gimbalX = AttitudeGain *
        (state.orientationEulerDeg[0] - commandedTiltXDegrees_) +
        RateGain * angularVelocityXDegrees;
    float gimbalY = AttitudeGain *
        (state.orientationEulerDeg[2] - commandedTiltZDegrees_) +
        RateGain * angularVelocityZDegrees;

    // A real two-axis gimbal has a circular travel limit, not two independent
    // square limits. Preserve the requested direction when saturated.
    const float gimbalMagnitude = sqrtf(gimbalX * gimbalX + gimbalY * gimbalY);
    if (gimbalMagnitude > MaximumGimbalDegrees)
    {
        const float scale = MaximumGimbalDegrees / gimbalMagnitude;
        gimbalX *= scale;
        gimbalY *= scale;
    }
    output.gimbalXDegrees = gimbalX;
    output.gimbalYDegrees = gimbalY;

    // Gimbaling tilts the thrust vector, reducing its vertical component.
    // Compensate before limiting thrust so lateral corrections do not make the
    // vehicle lose altitude. The 500 lbf motor still limits the achievable
    // vertical acceleration, so this naturally saturates at full throttle.
    const float bodyVerticalFactor = Clamp(
        cosf(state.orientationEulerDeg[0] / DegreesPerRadian) *
        cosf(state.orientationEulerDeg[2] / DegreesPerRadian),
        0.25f,
        1.0f
    );
    const float gimbalVerticalFactor =
        bodyVerticalFactor *
        cosf(output.gimbalXDegrees / DegreesPerRadian) *
        cosf(output.gimbalYDegrees / DegreesPerRadian);
    const float maximumVerticalAcceleration =
        MaximumThrustN * gimbalVerticalFactor / VehicleMassKg;
    const float commandedVerticalAcceleration = Clamp(
        safeVerticalAcceleration,
        0.5f,
        maximumVerticalAcceleration
    );
    output.thrustNewtons = Clamp(
        VehicleMassKg * commandedVerticalAcceleration / gimbalVerticalFactor,
        0.0f,
        MaximumThrustN
    );

    return output;
}

float HoverController::UpdateAxis(
    float error,
    float velocity,
    float dt,
    float kp,
    float ki,
    float kd,
    float integralLimit,
    float& integral
)
{
    integral += error * dt;
    integral = Clamp(integral, -integralLimit, integralLimit);
    return Clamp(kp * error + ki * integral - kd * velocity, -6.0f, 6.0f);
}

float HoverController::Clamp(float value, float minimum, float maximum)
{
    if (value < minimum)
        return minimum;
    if (value > maximum)
        return maximum;
    return value;
}

float HoverController::MoveToward(float current, float target, float maximumStep)
{
    const float delta = target - current;
    if (fabsf(delta) <= maximumStep)
        return target;
    return current + (delta > 0.0f ? maximumStep : -maximumStep);
}
