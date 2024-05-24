// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"
#include "Paradigm_IQ/UI/Menus/MenuComponets/WeaponUpgrade/WeaponUpgradeWidget.h"
#include "WeaponUpgradeManager.generated.h"



UCLASS()
class PARADIGM_IQ_API AWeaponUpgradeManager : public AActor
{
	GENERATED_BODY()

#pragma region Public Functions
public:	

	UFUNCTION()
	void AddUpgrades(const FUpgradeManager& Upgrade, const FUpgradeManager& SpecialUpgrade);

	UFUNCTION()
	void RollUpgrades(const int RollAmount);

	UFUNCTION()
	void UpgradeSelected(const FUpgradeCommunication& Upgrade);

#pragma endregion

#pragma region protected Functions
protected:

	AWeaponUpgradeManager();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void UpgradeCategory(const FUpgradeCommunication& Upgrade);

	UFUNCTION()
	void UpgradeSingleUse(const FUpgradeCommunication& Upgrade);

	UFUNCTION()
	TArray<FUpgradeManager> SetUpSpecialUpgrades();

	UFUNCTION()
	TArray<FUpgradeManager> SetUpWeaponUnlocks() const;

	UFUNCTION()
	TArray<FUpgradeManager> SetUpPassiveUnlocks() const;

#pragma endregion


#pragma region Protected Variables
protected:

	UPROPERTY()
	TArray<int> UpgradeQueManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|References", meta = (ToolTip = "Reference to the Weapon Upgrade widget to create."))
	TSubclassOf<UUserWidget> WeaponUpgradeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Weapons."))
	TSoftObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Passives."))
	TSoftObjectPtr<UDataTable> PassivesDataTable;

	UPROPERTY()
	TArray<FUpgradeManager> UpgradesAvailable;

	UPROPERTY()
	TArray<FUpgradeManager> SpecialWeaponUpgrades;

	UPROPERTY()
	float RollRange = 0.f;

	UPROPERTY()
	float CurrentRollTracker = 0.f;

	UPROPERTY()
	TArray<FUpgradeCommunication> UpgradesSelected;

	UPROPERTY()
	bool bUpgradeActive = false;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

#pragma endregion

#pragma region Getters and Setters
public:

	FORCEINLINE void					SetUpgradeQueManager			(const int& UpgradeAmount)			{ UpgradeQueManager.Add(UpgradeAmount); }

#pragma endregion
};
