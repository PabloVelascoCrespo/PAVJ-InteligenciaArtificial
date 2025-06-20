#include "obstacle.h"
#include "XmlFile.h"
#include "Misc/DefaultValueHelper.h"

bool ReadObstacles(const char* filename, TArray<Obstacle>& out)
{
    FString filePath = FPaths::Combine(FPaths::ProjectContentDir(), filename);
    FXmlFile xml(filePath, EConstructMethod::ConstructFromFile);
    if (!xml.IsValid()) return false;

    const FXmlNode* root = xml.GetRootNode();
    const FXmlNode* obsNode = root->FindChildNode(TEXT("obstacles"));
    if (!obsNode) return false;

    for (const FXmlNode* node : obsNode->GetChildrenNodes())
    {
        float x, y, r;
        FDefaultValueHelper::ParseFloat(node->GetAttribute("x"), x);
        FDefaultValueHelper::ParseFloat(node->GetAttribute("y"), y);
        FDefaultValueHelper::ParseFloat(node->GetAttribute("r"), r);

        out.Add({ FVector(x, 0.f, y), r });
    }

    return true;
}
