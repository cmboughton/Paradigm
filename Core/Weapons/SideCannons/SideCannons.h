// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "Projectile/SideCannonsProjectile.h"
#include "SideCannons.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API ASideCannons : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

	//This variable cant be changed until the spawn direction is figured out.
	UPROPERTY()
	int SpawnLocations = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The Side Cannon Spawner that should be spawned."))
	TSubclassOf<ASideCannonsProjectile> SCProjectile = nullptr;
};
