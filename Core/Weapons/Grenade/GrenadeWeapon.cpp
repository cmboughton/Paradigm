// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeWeapon.h"
#include "Paradigm_IQ/Core/Weapons/Grenade/Projectile/GrenadeProjectile.h"

void AGrenadeWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	if (GrenadeCountTracker < TriggerAmount)
	{
		FTransform GrenadeSpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), this->GetActorLocation() + FVector(0.f, 0.f, 100.f), FVector(1.f, 1.f, 1.f));
		SpawnProjectile(GrenadeSpawnTransform);

		GrenadeCountTracker++;
		BulletDelayTracker = (FireRate / TriggerAmount) * 0.5;
	}
	else
	{
		GrenadeCountTracker = 0;
		FireRateTracker = FireRate;
	}
}
