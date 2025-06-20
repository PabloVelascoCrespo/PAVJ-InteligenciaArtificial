#include "ObstacleAvoidance.h"
#include "AICharacter.h"
#include "obstacle/obstacle.h"

Accelerations ObstacleAvoidance::GetSteering()
{
	Accelerations acc;
	FVector velocity = m_owner->GetLinearVelocity();
	FVector position = m_owner->GetActorLocation();

	float lookahead = 100.0f;
	float charRadius = m_owner->GetParams().char_radius;

	FVector ahead = position + velocity.GetSafeNormal() * lookahead;

	Obstacle* mostThreatening = nullptr;
	float closestDist = FLT_MAX;
	TArray<Obstacle> Obstacles = m_owner->GetObstacles();
	for (Obstacle& obs : Obstacles)
	{
		float totalRadius = obs.radius + charRadius;
		FVector obsToAhead = ahead - obs.center;
		float dist = obsToAhead.Size();

		if (dist < totalRadius && dist < closestDist)
		{
			closestDist = dist;
			mostThreatening = &obs;
		}
	}

	if (mostThreatening)
	{
		FVector avoidanceDir = (ahead - mostThreatening->center).GetSafeNormal();
		acc.linear_acceleration = avoidanceDir * m_owner->GetParams().max_acceleration;
		acc.angular_acceleration = 0.0f;
		m_owner->SetCollidingObstacle(mostThreatening);
	}

	return acc;
}
