#include "attack.h"
#include "../AICharacter.h"

Attack::Attack(AAICharacter* inOwner) :
  m_owner(inOwner)
{}

void Attack::start()
{
  if (!m_owner)
  {
    return;
  }
  m_owner->Attack();
}
