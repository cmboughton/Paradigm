// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDrones.h"

#include "Components/SceneComponent.h"

void AEnemyDrones::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(PlayerCharacter != nullptr && BaseModel != nullptr)
	{
		if (DistanceCheck(AttackRange))
		{
			ApplyDamage(PlayerCharacter);
			EnemyDeath(false, EDeathType::Normal);
		}
	}

	if(RotationDuration > 0)
	{
		RotationDuration -= DeltaSeconds;
		
		RotationTracker += FRotator(0.f, DeltaSeconds * RotationSpeed, 0.f);
		RotateDroneForward(DeltaSeconds);
	}
	else
	{
		EnemyDeath(false, EDeathType::Explosive);
	}
}

/**
 * @brief Rotates the drones forward.
 *
 * This function rotates each drone in the forward direction based on its index and the elapsed time.
 * The rotation is calculated based on the drone's yaw and the sweep tracker. The drone's location is then updated
 * and its rotation is interpolated towards the calculated rotation.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void AEnemyDrones::RotateDroneForward(const float& DeltaTime) const
{
	if (BaseModel)
	{
		const FRotator NewLaserRot = FRotator(0.f, 0.f, 0.f) + RotationTracker;
		const FVector EndLocation = this->GetActorLocation() + NewLaserRot.Vector() * RotationRadius;
		BaseModel->SetWorldLocation(EndLocation);

		const FVector DroneLocation = BaseModel->GetComponentLocation();
		const FVector VectorToDrone = DroneLocation - this->GetActorLocation();
		FVector TangentVector(-VectorToDrone.Y, VectorToDrone.X, 0.0f);
		TangentVector.Normalize();
		const FRotator Rotation = TangentVector.Rotation();

		constexpr float LerpSpeed = 5.0f;
		const FRotator LerpedRotation = FMath::Lerp(BaseModel->GetRelativeRotation(), Rotation, DeltaTime * LerpSpeed);
		BaseModel->SetRelativeRotation(LerpedRotation);
	}
}