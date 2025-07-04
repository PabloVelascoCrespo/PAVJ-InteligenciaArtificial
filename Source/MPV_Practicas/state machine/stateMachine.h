#pragma once

#include "CoreMinimal.h"

class State;
class AAICharacter;

class StateMachine
{
  TArray<State*> m_states;
  State* m_currentState;
public:
  void load(AAICharacter* Owner);
  void start();
  void update(float deltaSeconds);
};