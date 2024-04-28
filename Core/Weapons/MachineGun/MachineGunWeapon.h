// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "MachineGunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AMachineGunWeapon : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

	UPROPERTY()
	int BulletTracker = 0;

	UPROPERTY()
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
};
