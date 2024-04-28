// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Ultimate/UltimateAbility.h"
#include "Explosive.generated.h"

UCLASS()
class PARADIGM_IQ_API AExplosive : public AUltimateAbility
{
	GENERATED_BODY()


protected:

	UFUNCTION()
	void Explosion(const float DeltaTime, const bool bShouldExplode, const bool ShouldGrow, const float GrowthModifier, const float Radius);

	UPROPERTY()
	float CurrentRadius = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Varaiables|Stats", meta = (ToolTip = "The explosion expanding modifier."), meta = (ClampMin = 500.f))
	float AffectGrowthModifier = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Varaiables|Stats", meta = (ToolTip = "The max radius the explosion can reach."))
	float AffectRadius = 10000.f;
};
