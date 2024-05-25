// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponStatsComponent.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UWeaponStatsComponent : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name;

public:

	FORCEINLINE UImage*						GetIcon()								const { return Icon;  }
	FORCEINLINE UTextBlock*					GetTextName()							const { return Name; }
};
