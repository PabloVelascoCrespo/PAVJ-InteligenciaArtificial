#pragma once
#include "ISteering.h"

class Arrive : public ISteering
{
public:
  Arrive(AAICharacter* owner) : ISteering(owner) {}
  virtual Accelerations GetSteering();
};
