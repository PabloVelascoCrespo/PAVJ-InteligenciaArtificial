#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include <CoreMinimal.h>

struct Obstacle
{
	FVector center;
	float radius;
};

bool ReadObstacles(const char* filename, TArray<Obstacle>& out);
#endif