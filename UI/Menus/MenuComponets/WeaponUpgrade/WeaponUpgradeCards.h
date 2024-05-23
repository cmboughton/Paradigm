// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "WeaponUpgradeCards.generated.h"

class UWeaponUpgradeWidget;
/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UWeaponUpgradeCards : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UPROPERTY()
	FUpgradeCommunication WeaponUpgradeStats;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UpgradeName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UpgradeDescription;

	UPROPERTY(meta = (BindWidget))
	class UButton* CardButton;

	UFUNCTION()
	void OnButtonClicked();

public:

	FORCEINLINE void	SetUpgradeStats	(const FUpgradeCommunication& StatsAdded)					{ WeaponUpgradeStats = StatsAdded; }
};
