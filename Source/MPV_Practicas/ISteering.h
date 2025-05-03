#pragma once
#include "CoreMinimal.h"

class AAICharacter;

struct Accelerations
{
	FVector linear_acceleration = FVector(0.0f, 0.0f, 0.0f);
	float angular_acceleration = 0.0f;
};

class ISteering
{
public:
	ISteering(AAICharacter* owner) : m_owner(owner) {}
	virtual Accelerations GetSteering() = 0;
protected:
	AAICharacter* m_owner;
};
