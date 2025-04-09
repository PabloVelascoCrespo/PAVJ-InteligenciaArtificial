// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "params/params.h"
#include "debug/debugdraw.h"
#include "Seek.h"
#include "Arrive.h"

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
	m_steering = new Arrive(this); // PRACTICA 2
	current_linear_velocity = FVector(0.0f, 0.0f, 0.0f);
	ReadParams("params.xml", m_params);
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	current_angle = GetActorAngle();

	Accelerations acc;
	acc = m_steering->GetSteering();

	FVector current_acceleration = acc.linear_acceleration * DeltaTime;
	current_linear_velocity += current_acceleration;
	if (current_linear_velocity.Length() > m_params.max_velocity)
	{
		current_linear_velocity = current_linear_velocity.GetSafeNormal() * m_params.max_velocity;
	}
	current_linear_velocity += current_acceleration;
	FVector charPos = GetActorLocation() + current_linear_velocity * DeltaTime;
	SetActorLocation(charPos);
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
	TArray<FVector> Points =
	{
		FVector(0.f, 0.f, 0.f),
		FVector(100.f, 0.f, 0.f),
		FVector(100.f, 0.f, 100.f),
		FVector(100.f, 0.f, 100.f),
		FVector(0.f, 0.f, 100.f)
	};

	SetPath(this, TEXT("follow_path"), TEXT("path"), Points, 5.0f, PathMaterial);

	SetCircle(this, TEXT("targetPosition"), m_params.targetPosition, m_params.arrive_radius);
	FVector dir(cos(FMath::DegreesToRadians(m_params.targetRotation)), 0.0f, sin(FMath::DegreesToRadians(m_params.targetRotation)));
	SetArrow(this, TEXT("targetRotation"), dir, 80.0f);
	
	SetArrow(this, TEXT("linear_velocity"), current_linear_velocity, current_linear_velocity.Length());
	SetArrow(this, TEXT("linear_acceleration"), m_steering->GetSteering().linear_acceleration, m_steering->GetSteering().linear_acceleration.Length());

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
