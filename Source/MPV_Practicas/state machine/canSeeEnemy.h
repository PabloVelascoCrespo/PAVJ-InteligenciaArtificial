#pragma once

#include "CoreMinimal.h"
#include "condition.h"

class AAICharacter;

class CanSeeEnemy : public Condition
{
public:

  CanSeeEnemy(AAICharacter* m_playerToCheck);

  bool Check() const override;
private:
  AAICharacter* m_player;
};