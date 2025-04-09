#pragma once
#include "CoreMinimal.h"

class AAICharacter;

struct Accelerations
{
	FVector linear_acceleration;
	float angular_acceleration;
};

class ISteering
{
public:
	ISteering(AAICharacter* owner) : m_owner(owner) {}
	virtual Accelerations GetSteering() = 0;
protected:
	AAICharacter* m_owner;
};
