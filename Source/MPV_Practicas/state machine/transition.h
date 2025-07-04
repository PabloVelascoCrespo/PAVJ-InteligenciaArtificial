#pragma once

#include "CoreMinimal.h"

class Condition;
class State;
class Action;

class Transition
{
public:
  Transition(Condition* condition, State* targetState, Action* trigger = nullptr);

  bool canTrigger() const;
  State* trigger() const;

private:
  Condition* m_condition;
  State* m_targetState;
  Action* m_triggerAction;
};