#pragma once
#include "ISteering.h"
#include "Align.h"

class AlignToMovement : public ISteering
{
public:
	AlignToMovement(AAICharacter* owner) : ISteering(owner), m_align(new AlignSteering(owner)) {}
  virtual Accelerations GetSteering();
private:
  AlignSteering* m_align;
};
