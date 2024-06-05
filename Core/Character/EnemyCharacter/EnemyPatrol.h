// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyPatrol.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AEnemyPatrol : public AEnemyCharacter
{
	GENERATED_BODY()

protected:

	virtual void Tick(const float DeltaTime) override;

	virtual void BeginPlay() override;

	UPROPERTY()
	bool bLocationTracker = false;

	UPROPERTY()
	FVector CurrentMoveToLoc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables", meta = (ToolTip = "The how fast the ship should move between points."))
	float MovementSpeedModifier = 0.15f;
};
