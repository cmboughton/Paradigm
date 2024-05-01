// Fill out your copyright notice in the Description page of Project Settings.


#include "SideCannons.h"

void ASideCannons::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	const FRotator SpawnRotation = FRotator(0.f, 360 / SpawnLocations, 0.f);
	for(int j = 0; j < SpawnLocations; j++)
	{
		const FRotator RotationalDirection = FRotator(0.f, (SpawnRotation.Yaw * (j + 1)) + 90, 0.f);
		const FTransform BulletSpawnLocation = FTransform(FRotator(0, RotationalDirection.Yaw, 0), FVector(this->GetActorLocation()), FVector(1.f, 1.f, 1.f));
		if (SCProjectile)
		{
			ASideCannonsProjectile* ProjectileSpawn = GetWorld()->SpawnActorDeferred<ASideCannonsProjectile>(SCProjectile, BulletSpawnLocation);
			ProjectileSpawn->SetDamage(Damage);
			ProjectileSpawn->SetAffectRadius(AffectRadius);
			ProjectileSpawn->SetSpecialUpgrade1(bSpecialUpgrade1Proj);
			ProjectileSpawn->SetSpecialUpgrade2(bSpecialUpgrade2Proj);
			ProjectileSpawn->SetSpecialUpgrade3(bSpecialUpgrade3Proj);
			ProjectileSpawn->SetTriggerAmount(TriggerAmount);
			ProjectileSpawn->FinishSpawning(BulletSpawnLocation);
		}
	}
	FireRateTracker = FireRate;
}
