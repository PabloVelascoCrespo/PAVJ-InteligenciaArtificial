#pragma once
#include "CoreMinimal.h"
#include "condition.h"

class AAICharacter;

class InAttackRange : public Condition
{
public:
  InAttackRange(AAICharacter* inOwner);
  bool Check() const override;
private:
  AAICharacter* m_owner;
};