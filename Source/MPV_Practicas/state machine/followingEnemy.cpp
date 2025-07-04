#include "followingEnemy.h"
#include "../AICharacter.h"

FollowingEnemy::FollowingEnemy(AAICharacter* inOwner) : 
  m_owner(inOwner)
{}

void FollowingEnemy::update(float deltaSeconds)
{
  if (!m_owner)
  {
    return;
  }

  m_owner->MoveTowardsEnemy();
}
