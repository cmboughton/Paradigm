// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Passives/Passives.h"
#include "ArcanicEcho.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AArcanicEcho : public APassives
{
	GENERATED_BODY()

protected:

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UPROPERTY()
	bool bWasHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables", meta = (ToolTip = "The Cooldown for when the damage can occur. (in seconds)"))
	float Cooldown = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (ToolTip = "The eexpanding modifier."), meta = (ClampMin = 500.f))
	float GrowthModifier = 3000.f;

	UPROPERTY()
	float CooldownTracker = 0.f;

	UPROPERTY()
	float CurrentRadius = 0.f;

	UPROPERTY()
	float Damage = 0.f;

public:

	FORCEINLINE void	SetDamage		(const float DamageValue)			{ Damage  = DamageValue; }
	FORCEINLINE void	SetWasHit		(const bool  WasHitValue)			{ bWasHit = WasHitValue; }
};
