// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyDrones.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AEnemyDrones : public AEnemyCharacter
{
	GENERATED_BODY()

protected:

	virtual void Tick(const float DeltaSeconds) override;

	UFUNCTION()
	void RotateDroneForward(const float& DeltaTime) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The duration that the drone rotates in a specific location before moving to a new one."))
	float RotationDuration = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The speed of the rotation."))
	float RotationSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The distance the drones rotate around origin point."))
	float RotationRadius = 150.f;

	UPROPERTY()
	FRotator RotationTracker;
};
