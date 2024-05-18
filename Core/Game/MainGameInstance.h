// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	FName SelectedShip;

public:

	FORCEINLINE FName			GetSelectedShip()							const { return SelectedShip; }
	FORCEINLINE void			SetSelectedShip(const FName& ShipValue)			  { SelectedShip = ShipValue; }
};
