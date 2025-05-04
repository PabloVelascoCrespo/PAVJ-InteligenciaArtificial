#include "Align.h"
#include "AICharacter.h"

Accelerations AlignSteering::GetSteering()
{
  Accelerations acc;

  float currentAngle = m_owner->GetActorAngle();
  float targetAngle = m_owner->GetParams().targetRotation;

  float rotation = targetAngle - currentAngle;
  rotation = FMath::UnwindDegrees(rotation);

  float rotationSize = FMath::Abs(rotation);
  float targetRotationSpeed = m_owner->GetParams().max_angular_velocity;
  float angularArriveRadius = m_owner->GetParams().angular_arrive_radius;

  if (rotationSize > angularArriveRadius)
  {
    targetRotationSpeed *= rotationSize / angularArriveRadius;
  }

  targetRotationSpeed *= FMath::Sign(rotation);

  float maxAngularAccel = m_owner->GetParams().max_angular_acceleration;
  float angularAccel = targetRotationSpeed - m_owner->GetAngularVelocity();
  angularAccel = FMath::Clamp(angularAccel, -maxAngularAccel, maxAngularAccel);

  acc.angular_acceleration = angularAccel;
  return acc;
}
