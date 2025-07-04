#include "transition.h"
#include "condition.h"
#include "action.h"

Transition::Transition(Condition* condition, State* targetState, Action* trigger) :
  m_condition(condition),
  m_targetState(targetState),
  m_triggerAction(trigger)
{
}

bool Transition::canTrigger() const
{
  if (!m_condition)
  {
    return false;
  }
  return m_condition->Check();
}

State* Transition::trigger() const
{
  if (m_triggerAction)
  {
    m_triggerAction->start();
  }
  return m_targetState;
}
