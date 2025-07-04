#pragma once
#include "CoreMinimal.h"
#include "action.h"

class AAICharacter;

class Attack : public Action
{
public:
  Attack(AAICharacter* inOwner);
  void start() override;

private:
  AAICharacter* m_owner;
};