// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UHealthBarComponent : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthPB;

public:

	void UpdateHealthBar(const float& CurrentHealth, const float& MaxHealth);
};
