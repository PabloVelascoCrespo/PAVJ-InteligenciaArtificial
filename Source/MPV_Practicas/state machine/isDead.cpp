#include "isDead.h"
#include "../AICharacter.h"

IsDead::IsDead(AAICharacter* inOwner) : 
  m_owner(inOwner)
{
}

bool IsDead::Check() const
{
  if (!m_owner)
  {
    return false;
  }
  return m_owner->IsDead();
}
