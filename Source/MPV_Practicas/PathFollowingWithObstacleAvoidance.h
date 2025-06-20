#pragma once
#include "ISteering.h"
class PathFollowing;
class ObstacleAvoidance;

class PathFollowingWithObstacleAvoidance : ISteering
{
public:
	PathFollowingWithObstacleAvoidance(AAICharacter* owner, PathFollowing* path, ObstacleAvoidance* obstacle) : 
		ISteering(owner), 
		m_path(path), 
		m_obstacle(obstacle) 
	{}

	virtual Accelerations GetSteering() override;
private:
	PathFollowing* m_path;
	ObstacleAvoidance* m_obstacle;
};