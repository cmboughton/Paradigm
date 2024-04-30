// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesWeapon.h"

void AMinesWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	if (BulletTracker < TriggerAmount)
	{
		BulletTracker++;
		BulletDelayTracker = (FireRate / TriggerAmount) * 0.5;
		SpawnProjectile(this->GetActorTransform());
	}
	else
	{
		FireRateTracker = FireRate;
		BulletTracker = 0;
	}
}
