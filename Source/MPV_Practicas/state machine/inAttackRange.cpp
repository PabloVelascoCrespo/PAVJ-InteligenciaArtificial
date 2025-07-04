#include "inAttackRange.h"
#include "../AICharacter.h"

InAttackRange::InAttackRange(AAICharacter* inOwner) :
  m_owner(inOwner)
{
}

bool InAttackRange::Check() const
{
  if (!m_owner)
  {
    return false;
  }
  return m_owner->IsEnemyInRange();
}
