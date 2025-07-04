#include "stateMachine.h"
#include "state.h"
#include "canSeeEnemy.h"
#include "transition.h"
#include "followingEnemy.h"
#include "inAttackRange.h"
#include "isDead.h"
#include "setImage.h"
#include "attack.h"
#include "condition.h"

void StateMachine::load(AAICharacter* Owner)
{
  // 1. Crear condiciones
  auto* C_SeeEnemy = new CanSeeEnemy(Owner);
  auto* C_InRange = new InAttackRange(Owner);
  auto* C_IsDead = new IsDead(Owner);
  //auto* C_Hit = new HasBeenHit(Owner);

  // 2. Crear acciones
  auto* A_SetIdleImg = new SetImage(Owner, "Sprites/Dragon_Idle.png");
  auto* A_SetAlertImg = new SetImage(Owner, "Sprites/Dragon_Alert.png");
  auto* A_SetChaseImg = new SetImage(Owner, "Sprites/Dragon_Run.png");
  auto* A_SetAttackImg = new SetImage(Owner, "Sprites/Dragon_Attack.png");
  auto* A_SetHitImg = new SetImage(Owner, "Sprites/Dragon_Hit.png");
  auto* A_SetDeadImg = new SetImage(Owner, "Sprites/Dragon_Dead.png");

  auto* A_MoveToPlayer = new FollowingEnemy(Owner);
  auto* A_Attack = new Attack(Owner);

  // 3. Crear estados (enter, exit, tick, transiciones vacías)
  auto* S_Idle = new State(A_SetIdleImg, nullptr, nullptr, {});
  auto* S_Alert = new State(A_SetAlertImg, nullptr, nullptr, {});
  auto* S_Chase = new State(A_SetChaseImg, nullptr, A_MoveToPlayer, {});
  auto* S_Attack = new State(A_SetAttackImg, nullptr, A_Attack, {});
  auto* S_Hit = new State(A_SetHitImg, nullptr, nullptr, {});
  auto* S_Dead = new State(A_SetDeadImg, nullptr, nullptr, {});

  // 4. Enlazar transiciones
  S_Idle->GetTransitionsMutable().Add(new Transition(C_SeeEnemy, S_Alert));
  S_Alert->GetTransitionsMutable().Add(new Transition(C_InRange, S_Attack));
  S_Alert->GetTransitionsMutable().Add(new Transition(C_SeeEnemy, S_Chase)); // sigue persiguiendo
  S_Chase->GetTransitionsMutable().Add(new Transition(C_InRange, S_Attack));
  //S_Attack->GetTransitionsMutable().Add(new Transition(C_Hit, S_Hit));
  S_Hit->GetTransitionsMutable().Add(new Transition(C_IsDead, S_Dead));
  S_Hit->GetTransitionsMutable().Add(new Transition(C_InRange, S_Attack));
  // ¡Muerte desde cualquier estado!
  for (State* St : { S_Idle, S_Alert, S_Chase, S_Attack })
    St->GetTransitionsMutable().Add(new Transition(C_IsDead, S_Dead));

  // 5. Guardarlo todo para poder liberarlo más tarde si quieres
  m_states = { S_Idle, S_Alert, S_Chase, S_Attack, S_Hit, S_Dead };

  // 6. Estado inicial
  m_currentState = S_Idle;
}

void StateMachine::start()
{
  if (!m_currentState)
  {
    return;
  }
  m_currentState->onEnter();
}

void StateMachine::update(float deltaSeconds)
{
  if (!m_currentState)
  {
    return;
  }

  m_currentState->update(deltaSeconds);

  for (const Transition* transition : m_currentState->getTransitions())
  {
    if (transition && transition->canTrigger())
    {
      m_currentState->onExit();
      m_currentState = transition->trigger();
      m_currentState->onEnter();
      break;
    }
  }
}
