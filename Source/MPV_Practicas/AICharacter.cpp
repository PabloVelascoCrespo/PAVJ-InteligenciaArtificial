// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "params/params.h"
#include "path/path.h"
#include "debug/debugdraw.h"
#include "Seek.h"
#include "Arrive.h"
#include "AlignToMovement.h"
#include "PathFollowing.h"
#include "obstacle/obstacle.h"
#include "ObstacleAvoidance.h"
#include "PathFollowingWithObstacleAvoidance.h"
#include "pathfinder/pathfinder.h"
#include "navmesh/navmesh.h"
#include "GeomTools.h"

// Sets default values
AAICharacter::AAICharacter()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  //PathfinderSystem = MakeShared<Pathfinder>();

  m_NavMesh = MakeShared<NavMesh>();
}

void AAICharacter::Attack()
{
  UE_LOG(LogTemp, Display, TEXT("ATTACK!"));
}

bool AAICharacter::CanSeePlayer()
{
  UE_LOG(LogTemp, Display, TEXT("CAN WE SEE THE PLAYER?"));
    return false;
}

bool AAICharacter::IsDead()
{
  UE_LOG(LogTemp, Display, TEXT("AM I DEAD?"));
    return false;
}

bool AAICharacter::IsEnemyInRange()
{
  UE_LOG(LogTemp, Display, TEXT("IS ENEMY IN RANGE?"));
    return false;
}

void AAICharacter::MoveTowardsEnemy()
{
  UE_LOG(LogTemp, Display, TEXT("MOVING TOWARDS ENEMY"));
}

void AAICharacter::SetImage(const char* imagepath)
{
  UE_LOG(LogTemp, Display, TEXT("SETING NEW IMAGE"));
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
  Super::BeginPlay();
  // m_steering = new Seek(this); // PRACTICA 1
  //m_movement_steering = new Arrive(this); // PRACTICA 2
  m_movement_steering = new PathFollowing(this); // PRACTICA 3
  //m_movement_steering = new ObstacleAvoidance(this); // PRACTICA 4

  //m_pathFollowing = new PathFollowing(this); // PRACTICA 5
  //m_obstacleAvoidance = new ObstacleAvoidance(this); // PRACTICA 5

  //m_movement_steering = new PathFollowingWithObstacleAvoidance(this, m_pathFollowing, m_obstacleAvoidance); // PRACTICA 5
  m_rotation_steering = new AlignToMovement(this); // PRACTICA 2

  ReadParams("params.xml", m_params);
  //ReadPath("path.xml", m_path);
  //ReadObstacles("obstacles.xml", m_obstacles);

  //PathfinderSystem->LoadMap("map.txt", "cost.txt"); PRACTICA 6
  ReadNavMesh("navmesh.xml", *m_NavMesh);
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
  //if (m_pathFollowing)
  //{
  //	delete m_pathFollowing;
  //	m_pathFollowing = nullptr;
  //}
  //if (m_obstacleAvoidance)
  //{
  //	delete m_obstacleAvoidance;
  //	m_obstacleAvoidance = nullptr;
  //}
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
  //FVector2D grid = WorldToGrid(mousePosition);
  //PathfinderSystem->SetStart(grid);

  StartingPosition = mousePosition;
}

void AAICharacter::OnClickedRight(const FVector& mousePosition)
{
  //m_params.targetPosition = mousePosition;

  //FVector dir = (mousePosition - GetActorLocation()).GetSafeNormal();
  //float angle = FMath::RadiansToDegrees(atan2(dir.Z, dir.X));
  //m_params.targetRotation = angle;
  //Path.Empty();
  //FVector2D grid = WorldToGrid(mousePosition);
  //PathfinderSystem->SetGoal(grid);
  //PathfinderSystem->FindPath();
  //for (FVector2D PathPoint : PathfinderSystem->GetPath())
  //{
  //  Path.Add(GridToWorld(PathPoint));
  //}
  EndingPosition = mousePosition;
  Path = m_NavMesh->CalculatePath(StartingPosition, EndingPosition);
  m_path.points = Path;
}

void AAICharacter::DrawDebug()
{
  //SetPath(this, TEXT("follow_path"), TEXT("path"), m_path.points, 5.0f, PathMaterial);
  //SetCircle(this, TEXT("closest_point"), closestPoint, 10.0f, FLinearColor::Blue);

  //SetCircle(this, TEXT("targetPosition"), m_params.targetPosition, m_params.arrive_radius, FLinearColor::Yellow);
  FVector dir(cos(FMath::DegreesToRadians(m_params.targetRotation)), 0.0f, sin(FMath::DegreesToRadians(m_params.targetRotation)));
  SetArrow(this, TEXT("targetRotation"), dir, 80.0f);

  SetArrow(this, TEXT("linear_velocity"), current_linear_velocity, current_linear_velocity.Length());
  SetArrow(this, TEXT("linear_acceleration"), m_movement_steering->GetSteering().linear_acceleration, m_movement_steering->GetSteering().linear_acceleration.Length());

  //TArray<TArray<FVector>> Polygons = {
  //  { FVector(0.f, 0.f, 0.f), FVector(100.f, 0.f, 0.f), FVector(100.f, 0.f, 100.0f), FVector(0.f, 0.f, 100.0f) },
  //  { FVector(100.f, 0.f, 0.f), FVector(200.f, 0.f, 0.f), FVector(200.f, 0.f, 100.0f) }
  //};
  //SetPolygons(this, TEXT("navmesh"), TEXT("mesh"), Polygons, NavmeshMaterial);

  //for (int i = 0; i < m_obstacles.Num(); ++i)
  //{
  //	const Obstacle& obs = m_obstacles[i];
  //	FString name = FString::Printf(TEXT("obstacle%d"), i + 1);

  //	if (!m_MostThreateningObstacle)
  //	{
  //		SetCircle(this, name, obs.center, obs.radius, FLinearColor::White);
  //	}

  //	const bool isColliding = (obs.center == m_MostThreateningObstacle->center);

  //	if (isColliding)
  //	{
  //		SetCircle(this, name, obs.center, obs.radius, FLinearColor::Red);
  //	}
  //	else
  //	{
  //		SetCircle(this, name, obs.center, obs.radius, FLinearColor::White);
  //	}
  //}

  //const TArray<GridNode>& grid = PathfinderSystem->GetGrid();
  //const int rows = PathfinderSystem->GetRows();
  //const int cols = PathfinderSystem->GetCols();

  //const FVector Origin = FVector(grid[0].x, 0, grid[0].y);

  //const float LineSize = 2.f;
  //const float Margin = LineSize * 0.5f;

  //float maxCost = PathfinderSystem->GetMaxCost();

  //auto Idx = [cols](int x, int y) { return y * cols + x; };

  //for (int y = 0; y < rows; ++y)
  //{
  //  for (int x = 0; x < cols; ++x)
  //  {
  //    const GridNode& n = grid[Idx(x, y)];

  //    // --- color -----------------------------------------------------
  //    FColor Color;
  //    if (!n.walkable)
  //    {
  //      Color = FColor::Black;                 // muro
  //    }
  //    else
  //    {
  //      const float norm = 1.f - (n.cost / maxCost); // 1→verde, 0→rojo
  //      Color = FColor::MakeRedToGreenColorFromScalar(norm);
  //    }

  //    // --- esquinas en mundo -----------------------------------------
  //    const FVector CellOrigin = Origin + FVector(x * CellSize, 0.f,
  //      -y * CellSize); // plano X-Z

  //    const FVector TL = CellOrigin + FVector(Margin, 0, -Margin);
  //    const FVector TR = CellOrigin + FVector(CellSize - Margin, 0, -Margin);
  //    const FVector BL = CellOrigin + FVector(Margin, 0, -CellSize + Margin);
  //    const FVector BR = CellOrigin + FVector(CellSize - Margin, 0, -CellSize + Margin);

  //    // --- dibujar ----------------------------------------------------
  //    DrawDebugLine(GetWorld(), TL, TR, Color, false, -1.f, 0, LineSize);
  //    DrawDebugLine(GetWorld(), TR, BR, Color, false, -1.f, 0, LineSize);
  //    DrawDebugLine(GetWorld(), BR, BL, Color, false, -1.f, 0, LineSize);
  //    DrawDebugLine(GetWorld(), BL, TL, Color, false, -1.f, 0, LineSize);
  //  }
  //}

  SetPath(this, TEXT("follow_path"), TEXT("path"), Path, 5.0f, PathMaterial);
  SetPolygons(this, TEXT("navmesh"), TEXT("mesh"), m_NavMesh->polygons, NavmeshMaterial);
  SetCircle(this, TEXT("startingPosition"), StartingPosition, 50, FLinearColor::Green);
  SetCircle(this, TEXT("endingPosition"), EndingPosition, 50, FLinearColor::Red);

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

void AAICharacter::SetTargetRotation(float NewTargetRotation)
{
  m_params.targetRotation = NewTargetRotation;
}

const TArray<Obstacle>& AAICharacter::GetObstacles() const
{
  return m_obstacles;
}

void AAICharacter::SetCollidingObstacle(Obstacle* MostThreateningObstacle)
{
  m_MostThreateningObstacle = MostThreateningObstacle;
}

FVector2D AAICharacter::WorldToGrid(const FVector& WorldPos, const FVector& Orig)
{
  float gx = (WorldPos.X - Orig.X) / CellSize;
  float gy = -(WorldPos.Z - Orig.Z) / CellSize;

  // FloorToInt → índice entero de la celda
  return FVector2D(FMath::FloorToInt(gx),
    FMath::FloorToInt(gy));
}

FVector AAICharacter::GridToWorld(const FVector2D& GridPos,
  const FVector& Orig)
{
  return FVector((Orig.X + GridPos.X * CellSize) + CellSize / 2,
    Orig.Y,                              // Y plano
    -(Orig.Z + GridPos.Y * CellSize) - CellSize / 2);
}

int32 AAICharacter::GetContainingPolygonIndex(const FVector& Point, const TArray<TArray<FVector>>& Polygons)
{
  const FVector2D Test2D(Point.X, Point.Y);
  for (int32 PolyId = 0; PolyId < Polygons.Num(); ++PolyId)
  {
    TArray<FVector2D> Poly2D;
    Poly2D.Reserve(Polygons[PolyId].Num());
    for (const FVector& V : Polygons[PolyId])
    {
      Poly2D.Add({ V.X, V.Y });
    }

    if (FGeomTools2D::IsPointInPolygon(Test2D, Poly2D))
    {
      return PolyId;
    }
  }
  return INDEX_NONE;
}
