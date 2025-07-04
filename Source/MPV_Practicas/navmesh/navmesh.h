#ifndef __NAVMESH_H__
#define __NAVMESH_H__

#include "CoreMinimal.h"

struct Link
{
  int fromPolygon;
  int fromEdgeStart, fromEdgeEnd;

  int toPolygon;
  int toEdgeStart, toEdgeEnd;
};

struct NavMesh
{
  TArray<TArray<FVector>> polygons;
  TArray<Link> links;
  TArray<FVector> CalculatePath(FVector StartingPosition, FVector EndingPosition);
};

bool ReadNavMesh(const char* filename, NavMesh& out);

#endif