#include "pathfinder.h"
#include <queue>

bool Pathfinder::LoadMap(const char* mapFile, const char* costFile)
{
  TMap<char, float> letterCost;
  FString pathfinderMapFile = mapFile;
  FString pathfinderCostFile = costFile;
  FString ContentFolderDir = FPaths::ProjectContentDir();
  FString mapFilePath = FPaths::Combine(*ContentFolderDir, *pathfinderMapFile);
  FString costFilePath = FPaths::Combine(*ContentFolderDir, *pathfinderCostFile);

  float currentMaxCost = -1;
  FString txt;
  FFileHelper::LoadFileToString(txt, *costFilePath);
  TArray<FString> lines;
  txt.ParseIntoArrayLines(lines);
  for (auto& L : lines)
  {
    char key; float c;
    if (sscanf(TCHAR_TO_ANSI(*L), "%c %f", &key, &c) == 2)
    {
      letterCost.Add(key, c);
      if (c > currentMaxCost)
      {
        currentMaxCost = c;
      }
    }
  }
  MaxCost = currentMaxCost;


  FString raw;
  if (!FFileHelper::LoadFileToString(raw, *mapFilePath)) return false;

  raw.ParseIntoArrayLines(lines);
  Rows = lines.Num();
  Cols = lines[0].Len();
  Grid.SetNum(Rows * Cols);

  for (int y = 0; y < Rows; ++y)
  {
    for (int x = 0; x < Cols; ++x)
    {
      char ch = lines[y][x];
      GridNode& n = Grid[Index(x, y)];
      n.x = x;
      n.y = y;

      if (ch == '#')
      {
        n.walkable = false;
        n.cost = FLT_MAX;
      }
      else if (letterCost.Contains(ch))
      {
        n.cost = letterCost[ch];
      }
      else
      {
        n.cost = 0.f;
      }
    }
  }
  return true;
}

void Pathfinder::SetStart(const FVector2D& gridPos)
{
  StartIdx = Index(gridPos.X, gridPos.Y);
}

void Pathfinder::SetGoal(const FVector2D& gridPos)
{
  GoalIdx = Index(gridPos.X, gridPos.Y);
}

bool Pathfinder::FindPath()
{
  Path.Empty();
  if (StartIdx < 0 || GoalIdx < 0)
  {
    return false;
  }

  TArray<float> Score; Score.Init(FLT_MAX, Grid.Num());
  TArray<int> parents; parents.Init(-1, Grid.Num());
  Score[StartIdx] = 0.0f;

  std::priority_queue<OpenNode> open;

  auto Heuristic = [&](int a, int b)
    {
      int ax = Grid[a].x, ay = Grid[a].y;
      int bx = Grid[b].x, by = Grid[b].y;
      return FMath::Abs(ax - bx) + FMath::Abs(ay - by);
    };

  float h = Heuristic(StartIdx, GoalIdx);
  open.push(OpenNode{ StartIdx, h, 0.f, h, -1 });

  TSet<int> closed;

  while (!open.empty())
  {
    OpenNode current = open.top(); open.pop();
    if (current.idx == GoalIdx)
    {
      parents[GoalIdx] = current.parent;
      break;
    }
    if (closed.Contains(current.idx))
    {
      continue;
    }
    closed.Add(current.idx);

    int cx = Grid[current.idx].x, cy = Grid[current.idx].y;

    const int OFF[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    for (auto& o : OFF)
    {
      int nx = cx + o[0], ny = cy + o[1];
      if (nx < 0 || nx >= Cols || ny < 0 || ny >= Rows) continue;
      int nidx = Index(nx, ny);
      if (!Grid[nidx].walkable) continue;

      float tentative = Score[current.idx] + Grid[nidx].cost;
      if (tentative < Score[nidx])
      {
        Score[nidx] = tentative;
        parents[nidx] = current.idx;
        h = Heuristic(nidx, GoalIdx);
        open.push(OpenNode(nidx, tentative + h, tentative, h, current.idx));
      }
    }
  }

  if (parents[GoalIdx] == -1)
  {
    return false;
  }

  int idx = GoalIdx;
  while (idx != -1)
  {
    Path.Insert(FVector2D(Grid[idx].x, Grid[idx].y), 0);
    idx = parents[idx];
  }
  return true;
}

float Pathfinder::GetMaxCost() const
{
  return MaxCost;
}

void Pathfinder::ReconstructPath(int goalIdx)
{
}
