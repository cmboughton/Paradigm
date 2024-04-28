// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"

#include "Evaluation/IMovieSceneEvaluationHook.h"

void AShotgunWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	const AActor* SelectedActor = FindClosestEnemy();

	const FRotator RandomDirection = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);

	for(int i = 0; i < TriggerAmount; i++)
	{
		if(SelectedActor)
		{
			const FRotator RotationalDirection = FRotationMatrix::MakeFromX(SelectedActor->GetActorLocation() - this->GetActorLocation()).Rotator();
			const FTransform BulletSpawnLocation = FTransform(FRotator(0, RotationalDirection.Yaw + FMath::RandRange(-15.f, 15.f), 0), FVector(this->GetActorLocation()), FVector(1.f, 1.f, 1.f));
			SpawnProjectile(BulletSpawnLocation);
		}
		else
		{
			
			const FTransform BulletSpawnLocation = FTransform(FRotator(0, RandomDirection.Yaw + FMath::RandRange(-15.f, 15.f), 0), FVector(this->GetActorLocation()), FVector(1.f, 1.f, 1.f));
			SpawnProjectile(BulletSpawnLocation);
		}
	}
	FireRateTracker = FireRate;
}
