#include "canSeeEnemy.h"
#include "../AICharacter.h"

CanSeeEnemy::CanSeeEnemy(AAICharacter* m_playerToCheck) :
  m_player(m_playerToCheck)
{ }


bool CanSeeEnemy::Check() const
{
  if (!m_player)
  {
    return false;
  }

  return m_player->CanSeePlayer();
}
