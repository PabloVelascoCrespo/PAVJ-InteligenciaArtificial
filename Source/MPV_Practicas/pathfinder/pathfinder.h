#pragma once
#include "CoreMinimal.h"

struct GridNode
{
  int x, y;
  bool walkable = true;
  float cost = 1.0f;
};

class Pathfinder
{
public:
  bool LoadMap(const char* mapFile, const char* costFile = nullptr);
  void SetStart(const FVector2D& gridPos);
  void SetGoal(const FVector2D& gridPos);

  bool FindPath();

  const TArray<FVector2D>& GetPath() const { return Path; }

  const TArray<GridNode>& GetGrid() const { return Grid; }
  int GetCols() const { return Cols; }
  int GetRows() const { return Rows; }
  float GetMaxCost() const;
private:
  struct OpenNode
  {
    int idx;
    float f, g, h;
    int parent = -1;
    bool operator<(const OpenNode& o) const { return f > o.f; }
  };

  int Index(int x, int y) const { return y * Cols + x; }
  void ReconstructPath(int goalIdx);

  TArray<GridNode> Grid;
  TArray<FVector2D> Path;
  int Cols = 0, Rows = 0;
  int StartIdx = -1, GoalIdx = -1;
  float MaxCost;
};