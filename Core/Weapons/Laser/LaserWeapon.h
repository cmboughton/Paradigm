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

	UFUNCTION()
	void SetLaserTransform(const int& MeshIndex, const FTransform& LaserTransform);

	UPROPERTY()
	bool bSpawnLasers = true;

	UPROPERTY()
	TArray<UStaticMeshComponent*> LaserMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Reference", meta = (ToolTip = "The static mesh that the laser will be."))
	UStaticMesh* LaserMeshRef;

	UPROPERTY()
	bool bStartSweep = false;

	UPROPERTY()
	FRotator SweepTracker;

	UPROPERTY()
	float LaserLengthTracker = 0.f;

	UPROPERTY()
	TArray<FHitResult> ActorsHit;
};
