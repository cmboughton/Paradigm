// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Ultimate/UltimateAbility.h"
#include "Phaseout.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API APhaseout : public AUltimateAbility
{
	GENERATED_BODY()

protected:

	virtual void UltimateAbilityStart() override;
	virtual void UltimateAbilityFinish() override;

	UPROPERTY()
	float PreviousMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The amount to increase the movement speed in %."))
	float MovementSpeedIncPercent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The duration of Invulnerability."))
	float InvulnerabilityDuration = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The Niagra for thruster. That changes when player invulnerable."))
	UNiagaraSystem* ThrusterNiagaraSystem;
};
