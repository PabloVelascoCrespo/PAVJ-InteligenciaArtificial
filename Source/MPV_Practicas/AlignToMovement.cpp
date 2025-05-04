#include "AlignToMovement.h"
#include "AICharacter.h"
#include "Align.h"
Accelerations AlignToMovement::GetSteering()
{
  Accelerations acc;

  FVector velocity = m_owner->GetLinearVelocity();
  
  if (velocity.Length() < KINDA_SMALL_NUMBER)
  {
    return acc;
  }

  float desiredOrientation = (atan2f(velocity.Y, velocity.X));
  acc.angular_acceleration = m_align->GetSteering().angular_acceleration;

  return acc;
}
