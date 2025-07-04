#pragma once

#include "CoreMinimal.h"

class Condition
{
public:
  virtual ~Condition() = default;
  virtual bool Check() const = 0;
};