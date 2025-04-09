#pragma once
#include "ISteering.h"

class Seek : public ISteering
{
public:
  Seek(AAICharacter* owner) : ISteering(owner) {}
  virtual Accelerations GetSteering();
};
