#pragma once
#include "CoreMinimal.h"
#include "action.h"

class AAICharacter;

class FollowingEnemy : public Action
{
public:
  FollowingEnemy(AAICharacter* inOwner);

  void update(float deltaSeconds) override;

private:
  AAICharacter* m_owner;
};