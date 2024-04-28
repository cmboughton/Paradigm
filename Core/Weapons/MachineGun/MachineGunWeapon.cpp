// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineGunWeapon.h"

#include "Paradigm_IQ/Core/Weapons/BasicProjectile/Projectile/BaseProjectile.h"

void AMachineGunWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	SpawnRotation = FRotator(SpawnRotation.Pitch, SpawnRotation.Yaw + 8, SpawnRotation.Roll);
	if (BulletTracker < TriggerAmount)
	{
		BulletTracker++;
		const FTransform BulletSpawnLocation = FTransform(SpawnRotation, FVector(this->GetActorLocation()), FVector(1.f, 1.f, 1.f));
		SpawnProjectile(BulletSpawnLocation);
		
	}
	else
	{
		FireRateTracker = FireRate;
		BulletTracker = 0;
	}
}
