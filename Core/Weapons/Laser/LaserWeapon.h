// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "LaserWeapon.generated.h"

class ADettachableLaser;
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

	UFUNCTION()
	void SpawnDetachableLasers() const;

	UPROPERTY()
	bool bSpawnLasers = true;

	UPROPERTY()
	TArray<UStaticMeshComponent*> LaserMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Reference", meta = (ToolTip = "The static mesh that the laser will be."))
	UStaticMesh* LaserMeshRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The chance that the lasers will detach."))
	float DetachableChance = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Reference", meta = (ToolTip = "Reference to the Detachable Laser BP."))
	TSubclassOf<ADettachableLaser> DetachableLaserRef;

	UPROPERTY()
	FRotator SweepTracker;

	UPROPERTY()
	float LaserLengthTracker = 0.f;

	UPROPERTY()
	TArray<FHitResult> ActorsHit;

	UPROPERTY()
	bool bIsRetracting = false;

	UPROPERTY()
	bool bRollDetachable = true;

	UPROPERTY()
	float DetachableLaserTracker = 0.f;
};
