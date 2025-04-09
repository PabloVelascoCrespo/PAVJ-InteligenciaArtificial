#include "Arrive.h"
#include "AICharacter.h"

Accelerations Arrive::GetSteering()
{
  Accelerations acc;
  FVector targetPosition = m_owner->GetParams().targetPosition;
  FVector charPos = m_owner->GetActorLocation();

  if ((targetPosition - charPos).Length() <= m_owner->GetParams().arrive_radius)
  {
    FVector desiredVel = FVector::Zero(); // TODO
    FVector normVel = desiredVel.GetSafeNormal();
    desiredVel = normVel * m_owner->GetParams().max_velocity;

    FVector desiredAcc = desiredVel - m_owner->GetLinearVelocity();
    FVector normAcc = desiredAcc.GetSafeNormal();
    desiredAcc = normAcc * m_owner->GetParams().max_acceleration;
    acc.linear_acceleration = desiredAcc;
  }
  else
  {
    FVector desiredVel = targetPosition - charPos;
    FVector normVel = desiredVel.GetSafeNormal();
    desiredVel = normVel * m_owner->GetParams().max_velocity;

    FVector desiredAcc = desiredVel - m_owner->GetLinearVelocity();
    FVector normAcc = desiredAcc.GetSafeNormal();
    desiredAcc = normAcc * m_owner->GetParams().max_acceleration;
    acc.linear_acceleration = desiredAcc;
  }
 
  return acc;
}