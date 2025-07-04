#pragma once
#include "CoreMinimal.h"
#include "action.h"

class AAICharacter;

class SetImage : public Action
{
public:
  SetImage(AAICharacter* inOwner, const char* path);

  void start() override;

private:
  AAICharacter* m_inOwner;
  const char* m_path;
};