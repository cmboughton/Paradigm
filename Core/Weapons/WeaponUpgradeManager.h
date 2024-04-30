// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons.h"
#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"
#include "WeaponUpgradeManager.generated.h"



UCLASS()
class PARADIGM_IQ_API AWeaponUpgradeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponUpgradeManager();

	UFUNCTION()
	void AddUpgrades(const FUpgradeManager Upgrade);

	UFUNCTION()
	void RollUpgrades(const int RollAmount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|References", meta = (ToolTip = "Reference to the Weapon Upgrade widget to create."))
	TSubclassOf<UUserWidget> WeaponUpgradeWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Weapons."))
	TSoftObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY()
	TArray<FUpgradeManager> UpgradesAvailable;

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

public:

	UFUNCTION()
	void UpgradeSelected(FUpgradeCommunication Upgrade);

	UPROPERTY()
	TArray<int> UpgradeQueManager;
};
