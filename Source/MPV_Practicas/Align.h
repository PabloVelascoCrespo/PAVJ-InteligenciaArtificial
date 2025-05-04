#pragma once
#include "ISteering.h"

class AlignSteering : public ISteering
{
public:
	AlignSteering(AAICharacter* owner) : ISteering(owner) {}
  virtual Accelerations GetSteering();
};
