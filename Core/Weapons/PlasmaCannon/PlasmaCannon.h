// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "PlasmaCannon.generated.h"

UCLASS()
class PARADIGM_IQ_API APlasmaCannon : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

	UFUNCTION()
	void SphereTraceDamage(FVector EndLoc);

	UPROPERTY()
	float PlasmaDelay = 0.f;

	UPROPERTY()
	bool bIsPlasmaDelay = true;

	UPROPERTY()
	AActor* SelectedEnemy;

	UPROPERTY()
	TArray<AActor*> SelectedEnemies;

	UPROPERTY()
	FRotator RotationalDirection;

	UPROPERTY()
	TArray<FRotator> RotationalDirections;

	UPROPERTY()
	FRotator RandomRot;

	UPROPERTY()
	TArray<FRotator> RandomRots;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;
};
