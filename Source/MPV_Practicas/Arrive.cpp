#include "Arrive.h"
#include "AICharacter.h"

Accelerations Arrive::GetSteering()
{
  Accelerations acc;
  FVector targetPosition = m_owner->GetParams().targetPosition;
  FVector charPos = m_owner->GetActorLocation();

  FVector toTarget = targetPosition - charPos;
  float distance = toTarget.Length();

  if (distance <= KINDA_SMALL_NUMBER)
  {
    return acc;
  }

  float arriveRadius = m_owner->GetParams().arrive_radius;
  float maxVelocity = m_owner->GetParams().max_velocity;
  float maxAccel = m_owner->GetParams().max_acceleration;

  float speed = maxVelocity;
  if (distance < arriveRadius)
  {
    speed = maxVelocity * (distance / arriveRadius);
  }

  FVector desiredVelocity = toTarget.GetSafeNormal() * speed;
  FVector desiredAccel = desiredVelocity - m_owner->GetLinearVelocity();

  acc.linear_acceleration = desiredAccel.GetClampedToMaxSize(maxAccel);

  return acc;

}