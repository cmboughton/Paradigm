// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AProjectileWeapon : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

	UPROPERTY()
	int BulletTracker = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The Distance to check if projects should target enemies or shoot randomly."))
	float EnemyDistanceCheck = 1000.f;

};
