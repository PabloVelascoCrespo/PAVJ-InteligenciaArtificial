// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "params/params.h"
#include "path/path.h"
#include "obstacle/obstacle.h"
#include "GameFramework/PlayerController.h"
#include "util.h"
#include "ISteering.h"

#include "AICharacter.generated.h"

class Seek;
class Arrive;
class AlignToMovement;
class ObstacleAvoidance;
class PathFollowing;
class PathFollowingWithObstacleAvoidance;
class Pathfinder;
class StateMachine;
struct NavMesh;

UCLASS()
class MPV_PRACTICAS_API AAICharacter : public APawn
{
  GENERATED_BODY()

public:
  // Sets default values for this pawn's properties
  AAICharacter();

  /**  */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIChar)
  uint32 bDoMovement : 1;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIChar)
  float current_angle;

  UPROPERTY(EditAnywhere)
  UMaterialInterface* PathMaterial;

  UPROPERTY(EditAnywhere)
  UMaterialInterface* NavmeshMaterial;

  void Attack();

  bool CanSeePlayer();

  bool IsDead();

  bool IsEnemyInRange();

  void MoveTowardsEnemy();

  void SetImage(const FString& NewSpritePath);

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  Params m_params;

  Path m_path;

  TArray<Obstacle> m_obstacles;

  TSharedPtr<Pathfinder> PathfinderSystem;

  TSharedPtr<NavMesh> m_NavMesh;
public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  UFUNCTION(BlueprintCallable, Category = "AIFunctions")
  void OnClickedLeft(const FVector& mousePosition);
  UFUNCTION(BlueprintCallable, Category = "AIFunctions")
  void OnClickedRight(const FVector& mousePosition);

  const Params& GetParams() const { return m_params; }

  const Path& GetPath() const { return m_path; }
  float GetActorAngle() const
  {
    FQuat currQuat = GetActorQuat();
    FVector axis;
    float axisAngle;
    currQuat.ToAxisAndAngle(axis, axisAngle);
    axisAngle = FMath::RadiansToDegrees(axisAngle);
    if (axis.Y > 0.0f)
      axisAngle = -axisAngle;

    axisAngle = convertTo360(axisAngle);
    return axisAngle;
  }
  void SetActorAngle(float angle) { FRotator newRot(angle, 0.0f, 0.0f); SetActorRotation(newRot); }

  void DrawDebug();

  FVector GetLinearVelocity() const;

  float GetAngularVelocity() const;

  void SetTargetPosition(FVector NewTargetPosition);

  void SetTargetRotation(float NewTargetRotation);

  FVector closestPoint;

  const TArray<Obstacle>& GetObstacles() const;

  void SetCollidingObstacle(Obstacle* CollidingObstacle);
private:
  FVector current_linear_velocity;
  float current_angular_velocity = 0.0f;
  Obstacle* m_MostThreateningObstacle;
  Seek* m_steering; // P1
  //Arrive* m_movement_steering; // P2
  //PathFollowing* m_movement_steering; // P3
  //ObstacleAvoidance* m_movement_steering; // P4	
  //PathFollowing* m_pathFollowing; // P5
  //ObstacleAvoidance* m_obstacleAvoidance; // P5
  //PathFollowingWithObstacleAvoidance* m_movement_steering; // P5
  AlignToMovement* m_rotation_steering;
  float CellSize = 100.0f;
  FVector2D WorldToGrid(const FVector& WorldPos, const FVector& Orig = FVector::ZeroVector);
  FVector GridToWorld(const FVector2D& GridPos, const FVector& Orig = FVector::ZeroVector);
  TArray<FVector> Path;
  FVector StartingPosition = FVector::ZeroVector;
  FVector EndingPosition = FVector::ZeroVector;
  int32 GetContainingPolygonIndex(const FVector& Point, const TArray<TArray<FVector>>& Polygons);

  bool enemyAlive = true;
  FVector EnemyLocation = FVector::ZeroVector;

  StateMachine* stateMachine;
};
  