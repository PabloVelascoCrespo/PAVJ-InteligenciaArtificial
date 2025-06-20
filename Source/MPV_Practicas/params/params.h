#ifndef __PARAMS_H__
#define __PARAMS_H__

#include <CoreMinimal.h>

struct Params
{
  // P1
    FVector targetPosition;
    float max_velocity;
    float max_acceleration;
  // P2
    float arrive_radius;
  // P3
    float max_angular_velocity;
    float max_angular_acceleration;
    float angular_arrive_radius;
    float targetRotation;
  // P4
    float look_ahead;
    float time_ahead;
  // P5
    float char_radius;
};

bool ReadParams(const char* filename, Params& params);

#endif