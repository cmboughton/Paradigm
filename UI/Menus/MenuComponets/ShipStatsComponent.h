// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShipStatsComponent.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UShipStatsComponent : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StatText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StatPB;

	UPROPERTY()
	FText StatName;

	UPROPERTY()
	float StatValue;

	UPROPERTY()
	float StatValuePB;

public:

	FORCEINLINE void						SetStatName(const FText& StatNameValue)				{ StatName = StatNameValue; }
	FORCEINLINE void						SetStatValue(const float& StatsValue)				{ StatValue = StatsValue; }
	FORCEINLINE void						SetStatValuePB(const float& StatsPBValue)			{ StatValuePB = StatsPBValue; }
};
