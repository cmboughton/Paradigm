// Fill out your copyright notice in the Description page of Project Settings.


#include "SideCannons.h"

void ASideCannons::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);
	float TempVar = TempVarSave + 10.f;
	TempVarSave = TempVar;
	const FRotator SpawnRotation = FRotator(0.f, 360 / SpawnLocations, 0.f);

	for(int j = 0; j < SpawnLocations; j++)
	{
		const FRotator RotationalDirection = FRotator(0.f, (SpawnRotation.Yaw * (j + 1)) + TempVarSave, 0.f);
		const FTransform BulletSpawnLocation = FTransform(FRotator(0, RotationalDirection.Yaw, 0.f), FVector(this->GetActorLocation()), FVector(1.f, 1.f, 1.f));

		if (SCProjectile != nullptr)
		{
			ASideCannonsProjectile* ProjectileSpawn = GetWorld()->SpawnActorDeferred<ASideCannonsProjectile>(SCProjectile, BulletSpawnLocation);
			ProjectileSpawn->SetDamage(Damage);
			ProjectileSpawn->SetAffectRadius(AffectRadius);
			ProjectileSpawn->SetSpecialUpgrade1(bSpecialUpgrade1);
			ProjectileSpawn->SetSpecialUpgrade2(bSpecialUpgrade2);
			ProjectileSpawn->SetSpecialUpgrade3(bSpecialUpgrade3);
			ProjectileSpawn->SetTriggerAmount(TriggerAmount);
			ProjectileSpawn->FinishSpawning(BulletSpawnLocation);
		}
	}
	FireRateTracker = FireRate;
}
