// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"
#include "Paradigm_IQ/Core/Weapons/BasicProjectile/FlameTrailUpgrade.h"
#include "BaseProjectile.generated.h"



UCLASS()
class PARADIGM_IQ_API ABaseProjectile : public AProjectile
{
	GENERATED_BODY()

#pragma region Protected Functions
protected:

	virtual void TraceCheck(const float& DeltaTime) override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void DestroyProjectile() override;

#pragma endregion

#pragma region Protected Variables
protected:

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "Reference to the Flame Trail BP to spawn."))
	const TSubclassOf<AFlameTrailUpgrade> FlameTrailUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "Flame Trail chance."))
	float FlameTrailChance = 10.f;

	UPROPERTY()
	bool bHasFlameTrailUpgrade = false;

#pragma endregion
};
