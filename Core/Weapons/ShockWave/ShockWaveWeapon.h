// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "ShockWaveWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AShockWaveWeapon : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

	UPROPERTY()
	float ShockWaveZone = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The delay until the next shock wave forms."))
	float ShockWaveDelay = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The delay shock wave zone stays active."))
	float ShockWaveZoneDelay = 1.f;

	UPROPERTY()
	float ShockWaveRadiusTracker = 200.f;

	UPROPERTY()
	float ShockWaveDelayTracker = 0.f;

	UPROPERTY()
	TArray<FHitResult> ActorsHit;
};
