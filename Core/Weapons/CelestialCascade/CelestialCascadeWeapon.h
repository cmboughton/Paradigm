// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "CelestialCascadeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API ACelestialCascadeWeapon : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

	virtual AActor* FindClosestEnemy(const FVector& Origin) const override;

	UPROPERTY()
	TArray<AActor*> EnemiesToDamage;
};
