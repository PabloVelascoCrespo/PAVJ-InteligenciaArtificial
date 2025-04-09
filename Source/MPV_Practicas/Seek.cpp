#include "Seek.h"
#include "AICharacter.h"

Accelerations Seek::GetSteering()
{
  Accelerations acc;
  FVector targetPosition = m_owner->GetParams().targetPosition;

  FVector charPos = m_owner->GetActorLocation();
  FVector desiredVel = targetPosition - charPos;
  FVector normVel = desiredVel.GetSafeNormal();
  desiredVel = normVel * m_owner->GetParams().max_velocity;

  FVector desiredAcc = desiredVel - m_owner->GetLinearVelocity();
  FVector normAcc = desiredAcc.GetSafeNormal();
  desiredAcc = normAcc * m_owner->GetParams().max_acceleration;
  acc.linear_acceleration = desiredAcc;
  
  return acc;
}