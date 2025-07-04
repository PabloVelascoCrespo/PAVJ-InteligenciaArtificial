#pragma once

#include "CoreMinimal.h"
#include "action.h"

class GotoAction : public Action
{
public:
  GotoAction(const FVector& pos);
  void start() override;
  void update() override;
private:
  FVector m_destination;
};