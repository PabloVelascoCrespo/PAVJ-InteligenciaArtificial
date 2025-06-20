#ifndef __PATH_H__
#define __PATH_H__

#include <CoreMinimal.h>

struct Path
{
  TArray<FVector> points;
};

bool ReadPath(const char* filename, Path& path);

#endif