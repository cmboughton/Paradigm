// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "WeaponUpgradeWidget.generated.h"

struct FWeaponUpgrades;
/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UWeaponUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* WeaponCardsHb;

	UPROPERTY()
	TArray<FUpgradeCommunication> WeaponUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|References", meta = (ToolTip = "Reference to the Weapon Upgrade card widget to create."))
	TSubclassOf<UUserWidget> WeaponUpgradeCardWidget = nullptr;

	virtual void NativeConstruct() override;


public:

	UFUNCTION()
	void PauseGame(const bool PauseStatus) const;

	UFUNCTION()
	void AddWidget(const bool WidgetStatus);

	FORCEINLINE void	SetUpgradesToAdd(const TArray<FUpgradeCommunication>& UpgradesAdded) { WeaponUpgrades = UpgradesAdded; }
};
