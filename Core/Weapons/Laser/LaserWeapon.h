// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "LaserWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API ALaserWeapon : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "If the lasers should sweep or not."))
	bool bShouldSweep = false;

	UPROPERTY()
	FRotator SweepRotation = FRotator(0.f, 0.f, 0.f);

	UPROPERTY()
	FRotator SweepTracker;

	UPROPERTY()
	float LaserLengthTracker = 0.f;

	UPROPERTY()
	float LaserEndDelayTracker = 0.f;

	UPROPERTY()
	TArray<FHitResult> ActorsHit;

};
