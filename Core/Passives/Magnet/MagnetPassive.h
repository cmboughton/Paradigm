// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Passives/Passives.h"
#include "MagnetPassive.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AMagnetPassive : public APassives
{
	GENERATED_BODY()

protected:

	virtual void Tick(const float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Varaibles|Stats", meta = (ToolTip = "The time it takes for the Pick Up Radius to increase."))
	float UpdateDelay = 10.f;

	UPROPERTY()
	float UpdateDelayTracker = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Varaibles|Stats", meta = (ToolTip = "How much the Pick Up Radius increase everytime the UpdateDelay happens."))
	float PickUpRadiusModifier = 0.05f;
};	
