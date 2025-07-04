#pragma once

#include "CoreMinimal.h"

class Action
{
public:
  virtual ~Action() = default;
  virtual void start() {};
  virtual void update(float deltaSeconds) {};
  virtual void end() {};
};