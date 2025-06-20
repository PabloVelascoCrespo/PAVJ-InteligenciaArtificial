#pragma once
#include "ISteering.h"

struct Obstacle;

class ObstacleAvoidance : ISteering
{
public:
	ObstacleAvoidance(AAICharacter* owner) : ISteering(owner) {}
	virtual Accelerations GetSteering() override;
};