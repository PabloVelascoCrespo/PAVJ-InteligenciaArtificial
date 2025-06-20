#include "PathFollowing.h"
#include "path/path.h"
#include "AICharacter.h"
#include "Arrive.h"
#include "AlignToMovement.h"

Accelerations PathFollowing::GetSteering()
{
  FVector pos = m_owner->GetActorLocation();
  float lookAhead = m_owner->GetParams().look_ahead;
  Path path = m_owner->GetPath();

  // Paso 1: encontrar el segmento más cercano
  float closestDistSq = FLT_MAX;
  FVector closestPoint = FVector::ZeroVector;
  int closestSegment = -1;
  size_t numPoints = path.points.Num();

  for (size_t i = 0; i < numPoints; ++i)
  {
    FVector a = path.points[i];
    FVector b = path.points[(i + 1) % numPoints];

    FVector ab = b - a;
    FVector ap = pos - a;

    float t = FMath::Clamp(FVector::DotProduct(ap, ab) / ab.SizeSquared(), 0.0f, 1.0f);
    FVector pointOnSegment = a + t * ab;
    float distSq = FVector::DistSquared(pointOnSegment, pos);

    if (distSq < closestDistSq)
    {
      closestDistSq = distSq;
      closestPoint = pointOnSegment;
      closestSegment = i;
    }
  }

  // Paso 2: avanzar look_ahead desde el punto más cercano
  FVector a = path.points[closestSegment];
  FVector b = path.points[(closestSegment + 1) % numPoints];
  FVector ab = b - a;
  FVector dir = ab.GetSafeNormal();
  FVector lookAheadTarget = closestPoint + dir * lookAhead;

  // Paso 3: usar Arrive hacia lookAheadTarget
  m_owner->SetTargetPosition(lookAheadTarget);
  float angle = FMath::RadiansToDegrees(atan2(dir.Z, dir.X));
  m_owner->SetTargetRotation(angle);
  m_owner->closestPoint = closestPoint;

  Arrive arrive(m_owner);
  AlignToMovement align(m_owner);

  Accelerations acc;
  acc.linear_acceleration = arrive.GetSteering().linear_acceleration;
  return acc;
}
