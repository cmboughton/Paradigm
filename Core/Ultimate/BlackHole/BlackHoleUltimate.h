// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Ultimate/UltimateAbility.h"
#include "BlackHoleUltimate.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API ABlackHoleUltimate : public AUltimateAbility
{
	GENERATED_BODY()

protected:

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The force the Black Hole applies onto the actor hit."), meta = (ClampMin = 150.f))
	float GravitationalForce = 0.f;

	UPROPERTY()
	float PullDurationTracker = Duration / 30;

	UPROPERTY()
	float PullDurationDelay = 0.5f;

	UPROPERTY()
	TArray<AEnemyCharacter*> EnemiesPulled;
};
