// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"


void AProjectileWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	if (BulletTracker < TriggerAmount)
	{
		BulletTracker++;
		BulletDelayTracker = (FireRate / TriggerAmount) * 0.5;
		const TArray<AActor*> SelectedEnemy = FindClosestEnemies(EnemyDistanceCheck, this->GetActorLocation());
		if (!SelectedEnemy.IsEmpty())
		{
			const int randRoll = FMath::RandRange(0, SelectedEnemy.Num() - 1);
			if(SelectedEnemy.IsValidIndex(randRoll))
			{
				const FRotator RotationalDirection = FRotationMatrix::MakeFromX(SelectedEnemy[randRoll]->GetActorLocation() - this->GetActorLocation()).Rotator();
				const FTransform BulletSpawnLocation = FTransform(FRotator(0, RotationalDirection.Yaw, 0), FVector(this->GetActorLocation()), FVector(1.f, 1.f, 1.f));
				SpawnProjectile(BulletSpawnLocation);
			}
		}
		else
		{
			const FTransform BulletSpawnLocation = FTransform(FRotator(0, FMath::RandRange(0, 360), 0), FVector(this->GetActorLocation()), FVector(1.f, 1.f, 1.f));
			SpawnProjectile(BulletSpawnLocation);
		}
	}
	else
	{
		FireRateTracker = FireRate;
		BulletTracker = 0;
	}
}

