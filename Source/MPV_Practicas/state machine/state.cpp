#include "state.h"
#include "action.h"

State::State(Action* enterAction, Action* exitAction, Action* stateAction, TArray<Transition*> transitions) :
  m_enterAction(enterAction),
  m_exitAction(exitAction),
  m_stateAction(stateAction),
  m_transitions(transitions)
{
}

void State::onEnter()
{
  if (m_enterAction)
  {
    m_enterAction->start();
  }
  if (m_stateAction)
  {
    m_stateAction->start();
  }
}

void State::update(float deltaSeconds)
{
  if (!m_stateAction)
  {
    return;
  }

  m_stateAction->update(deltaSeconds);
}

void State::onExit()
{
  if (m_stateAction)
  {
    m_stateAction->end();
  }
  if (m_exitAction)
  {
    m_exitAction->start();
  }
}

const TArray<Transition*>& State::getTransitions()
{
  return m_transitions;
}

TArray<Transition*>& State::GetTransitionsMutable()
{
  return m_transitions;
}
