#include "navmesh.h"
#include "XmlFile.h"
#include "Misc/DefaultValueHelper.h"

bool ReadNavMesh(const char* filename, NavMesh& out)
{
  FString filePath = FPaths::Combine(FPaths::ProjectContentDir(), filename);
  FXmlFile xml(filePath, EConstructMethod::ConstructFromFile);
  if (!xml.IsValid()) return false;

  const FXmlNode* root = xml.GetRootNode();
  const FXmlNode* polyList = root->FindChildNode(TEXT("polygons"));
  const FXmlNode* linkList = root->FindChildNode(TEXT("links"));

  FVector polygon = FVector::ZeroVector;

  // Polígonos
  for (const FXmlNode* polyNode : polyList->GetChildrenNodes())
  {
    TArray<FVector> polygons;
    for (const FXmlNode* ptNode : polyNode->GetChildrenNodes())
    {
      FDefaultValueHelper::ParseDouble(ptNode->GetAttribute("x"), polygon.X);
      FDefaultValueHelper::ParseDouble(ptNode->GetAttribute("y"), polygon.Z);
      polygons.Add(polygon);
    }
    out.polygons.Add(polygons);
  }
  // Links
  for (const FXmlNode* linkNode : linkList->GetChildrenNodes())
  {
    const FXmlNode* start = linkNode->FindChildNode(TEXT("start"));
    const FXmlNode* end = linkNode->FindChildNode(TEXT("end"));

    Link l;
    FDefaultValueHelper::ParseInt(start->GetAttribute("polygon"), l.fromPolygon);
    FDefaultValueHelper::ParseInt(start->GetAttribute("edgestart"), l.fromEdgeStart);
    FDefaultValueHelper::ParseInt(start->GetAttribute("edgeend"), l.fromEdgeEnd);

    FDefaultValueHelper::ParseInt(end->GetAttribute("polygon"), l.toPolygon);
    FDefaultValueHelper::ParseInt(end->GetAttribute("edgestart"), l.toEdgeStart);
    FDefaultValueHelper::ParseInt(end->GetAttribute("edgeend"), l.toEdgeEnd);

    out.links.Add(l);
  }

  return true;
}

TArray<FVector> NavMesh::CalculatePath(FVector StartingPosition, FVector EndingPosition)
{
  TArray<FVector> out;

  out.Add(StartingPosition);

  for (int i = 0; i < polygons.Num() - 1; ++i)
  {
    const int32 polyA = i;
    const int32 polyB = i + 1;
    for (const Link& link : links)
    {
      const bool matchAB = (link.fromPolygon == polyA && link.toPolygon == polyB);
      const bool matchBA = (link.fromPolygon == polyB && link.toPolygon == polyA);

      if (!matchAB && !matchBA) continue;

      const bool aIsFrom = matchAB;

      const int32 edgeStart = aIsFrom ? link.fromEdgeStart : link.toEdgeStart;
      const int32 edgeEnd = aIsFrom ? link.fromEdgeEnd : link.toEdgeEnd;

      const FVector& p1 = polygons[polyA][edgeStart];
      const FVector& p2 = polygons[polyA][edgeEnd];

      out.Add((p1 + p2) * 0.5f);
      break;
    }
  }
  out.Add(EndingPosition);
  return out;
}
