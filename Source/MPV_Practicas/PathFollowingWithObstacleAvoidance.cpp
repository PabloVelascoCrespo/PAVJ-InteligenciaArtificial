#include "PathFollowingWithObstacleAvoidance.h"
#include "AICharacter.h"
#include "ObstacleAvoidance.h"
#include "PathFollowing.h"
Accelerations PathFollowingWithObstacleAvoidance::GetSteering()
{
    Accelerations avoid = m_obstacle->GetSteering();
    Accelerations path = m_path->GetSteering();

    Accelerations result;

    if (!avoid.linear_acceleration.IsNearlyZero())
    {
        result.linear_acceleration = avoid.linear_acceleration;
    }
    else
    {
        result.linear_acceleration = path.linear_acceleration;
    }

    result.angular_acceleration = 0.0f;
    return result;
}