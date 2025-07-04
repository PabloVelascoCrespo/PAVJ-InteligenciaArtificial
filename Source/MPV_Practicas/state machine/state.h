#pragma once

#include "CoreMinimal.h"

class Action;
class Transition;

class State
{
public:
  State(Action* enterAction, Action* exitAction, Action* stateAction, TArray<Transition*> transitions);
  void onEnter();
  void update(float deltaSeconds);
  void onExit();
  const TArray<Transition*>& getTransitions();
  TArray<Transition*>& GetTransitionsMutable();

private:
  Action* m_enterAction;
  Action* m_exitAction;
  Action* m_stateAction;
  TArray<Transition*> m_transitions;

};