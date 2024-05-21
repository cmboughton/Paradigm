// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "RailGunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API ARailGunWeapon : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

	UPROPERTY()
	int BulletTracker = 0;
};
