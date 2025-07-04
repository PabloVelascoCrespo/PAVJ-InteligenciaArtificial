#pragma once
#include "CoreMinimal.h"
#include "condition.h"

class AAICharacter;

class IsDead : public Condition
{
public:
  IsDead(AAICharacter* inOwner);
  bool Check() const override;
private:
  AAICharacter* m_owner;
};