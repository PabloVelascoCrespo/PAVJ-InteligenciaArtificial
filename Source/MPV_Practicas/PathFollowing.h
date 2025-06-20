#pragma once
#include "ISteering.h"

struct Path;

class PathFollowing : public ISteering
{
public:
	PathFollowing(AAICharacter* owner) : ISteering(owner) {}
	virtual Accelerations GetSteering() override;
};