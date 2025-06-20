// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "params/params.h"
#include "path/path.h"
#include "debug/debugdraw.h"
#include "Seek.h"
#include "Arrive.h"
#include "AlignToMovement.h"
#include "PathFollowing.h"

// Sets default values
AAICharacter::AAICharacter()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
  Super::BeginPlay();
  // m_steering = new Seek(this); // PRACTICA 1
  //m_movement_steering = new Arrive(this); // PRACTICA 2
  m_movement_steering = new PathFollowing(this); // PRACTICA 3
  m_rotation_steering = new AlignToMovement(this); // PRACTICA 2
  current_linear_velocity = FVector(0.0f, 0.0f, 0.0f);
  ReadParams("params.xml", m_params);
  ReadPath("path.xml", m_path);
}

void AAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  if (m_movement_steering)
  {
    delete m_movement_steering;
    m_movement_steering = nullptr;
  }  
  if (m_rotation_steering)
  {
    delete m_rotation_steering;
    m_rotation_steering = nullptr;
  }
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  current_angle = GetActorAngle();

  // == STEERING BEHAVIORS == 
  Accelerations acc;
  acc.linear_acceleration = m_movement_steering->GetSteering().linear_acceleration;
  acc.angular_acceleration = m_rotation_steering->GetSteering().angular_acceleration;

  // == LINEAR VELOCITY ==
  current_linear_velocity += acc.linear_acceleration * DeltaTime;

  if (current_linear_velocity.Length() > m_params.max_velocity)
  {
    current_linear_velocity = current_linear_velocity.GetSafeNormal() * m_params.max_velocity;
  }

  FVector charPos = GetActorLocation() + current_linear_velocity * DeltaTime;
  SetActorLocation(charPos);
  

  // == ANGULAR VELOCITY == 
  current_angular_velocity += acc.angular_acceleration * DeltaTime;

  if (current_angular_velocity > m_params.max_angular_velocity)
  {
    current_angular_velocity = FMath::Clamp(current_angular_velocity, -m_params.max_angular_velocity, m_params.max_angular_velocity);
  }
    current_angular_velocity += acc.angular_acceleration * DeltaTime;

  float newAngle = current_angle + current_angular_velocity * DeltaTime;
  SetActorRotation(FRotator(newAngle, 0, 0));
  
  DrawDebug();

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::OnClickedLeft(const FVector& mousePosition)
{
  SetActorLocation(mousePosition);
}

void AAICharacter::OnClickedRight(const FVector& mousePosition)
{
  m_params.targetPosition = mousePosition;

  FVector dir = (mousePosition - GetActorLocation()).GetSafeNormal();
  float angle = FMath::RadiansToDegrees(atan2(dir.Z, dir.X));
  m_params.targetRotation = angle;
}

void AAICharacter::DrawDebug()
{
  SetPath(this, TEXT("follow_path"), TEXT("path"), m_path.points, 5.0f, PathMaterial);
  SetCircle(this, TEXT("closest_point"), closestPoint, 10.0f, FLinearColor::Blue);
  SetCircle(this, TEXT("look_ahead_point"), lookAheadTarget, 10.0f, FLinearColor::Yellow);

  SetCircle(this, TEXT("targetPosition"), m_params.targetPosition, m_params.arrive_radius);
  FVector dir(cos(FMath::DegreesToRadians(m_params.targetRotation)), 0.0f, sin(FMath::DegreesToRadians(m_params.targetRotation)));
  SetArrow(this, TEXT("targetRotation"), dir, 80.0f);

  SetArrow(this, TEXT("linear_velocity"), current_linear_velocity, current_linear_velocity.Length());
  SetArrow(this, TEXT("linear_acceleration"), m_movement_steering->GetSteering().linear_acceleration, m_movement_steering->GetSteering().linear_acceleration.Length());

  TArray<TArray<FVector>> Polygons = {
    { FVector(0.f, 0.f, 0.f), FVector(100.f, 0.f, 0.f), FVector(100.f, 0.f, 100.0f), FVector(0.f, 0.f, 100.0f) },
    { FVector(100.f, 0.f, 0.f), FVector(200.f, 0.f, 0.f), FVector(200.f, 0.f, 100.0f) }
  };
  SetPolygons(this, TEXT("navmesh"), TEXT("mesh"), Polygons, NavmeshMaterial);
}

FVector AAICharacter::GetLinearVelocity() const
{
  return current_linear_velocity;
}

float AAICharacter::GetAngularVelocity() const
{
  return current_angular_velocity;
}

void AAICharacter::SetTargetPosition(FVector NewTargetPosition)
{
  m_params.targetPosition = NewTargetPosition;
}
