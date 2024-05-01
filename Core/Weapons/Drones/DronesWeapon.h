// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "DronesWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API ADronesWeapon : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

	UPROPERTY()
	float RadialDistance = 0.f;

	UPROPERTY()
	bool InitialSpawn = false;

	UPROPERTY()
	TArray<FHitResult> ActorsHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "If the Drones should sweep or not."))
	bool bShouldSweep = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "If Duration (in seconds) that the Drones wills stay active."))
	float DroneDuration = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "If the Drones should sweep or not."))
	float RotationSpeed = 50.f;

	UPROPERTY()
	float DroneDurationTracker = 0.f;

	UPROPERTY()
	FRotator SweepRotation = FRotator(0.f, 0.f, 0.f);

	UPROPERTY()
	FRotator SweepTracker;

	UPROPERTY()
	float DroneDelayTracker = 0.f;
};
