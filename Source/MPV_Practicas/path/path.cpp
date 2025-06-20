#include "path.h"
#include "XmlFile.h"
#include "Misc/DefaultValueHelper.h"

bool ReadPath(const char* filename, Path& path)
{
  FString filePath = FPaths::Combine(FPaths::ProjectContentDir(), filename);
  FXmlFile xml(filePath, EConstructMethod::ConstructFromFile);
  if (!xml.IsValid()) return false;

  const FXmlNode* root = xml.GetRootNode();
  const FXmlNode* pointsNode = root->FindChildNode(TEXT("points"));
  if (!pointsNode) return false;

  for (const FXmlNode* pointNode : pointsNode->GetChildrenNodes())
  {
    float x = 0, y = 0;
    FDefaultValueHelper::ParseFloat(pointNode->GetAttribute("x"), x);
    FDefaultValueHelper::ParseFloat(pointNode->GetAttribute("y"), y);
    path.points.Add(FVector(x, 0.0f, y));
  }

  return true;
}
