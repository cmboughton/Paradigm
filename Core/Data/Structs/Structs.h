// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Data/Enums/Enums.h"
#include "Structs.generated.h"

class AWeapons;
class AEnemyCharacter;
class ASpawnPoints;

USTRUCT(BlueprintType)
struct FExperienceOrb
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience", meta = (ToolTip = "The amount of experience gained."))
	float Experience = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience", meta = (ToolTip = "The amount of ultimate ability experience gained."))
	float UltimateExperience = 20.f;
};

USTRUCT(BlueprintType)
struct FEnemyType
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies", meta = (ToolTip = "The the Enemy Character reference."))
	TSoftClassPtr<AEnemyCharacter> EnemyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies", meta = (ToolTip = "The weight of which it should spawn. The weight is calculated by adding all of the weight from all elements of the array and rolling in the range."))
	float Weight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Conditions", meta = (ToolTip = "How the enemy should spawn. If fixed spawn is selected, a point will need to be assigned or it will spawn randomly."))
	ESpawnType SpawnType = ESpawnType::FixedSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Conditions", meta = (ToolTip = "The reference to the spawn points."))
	TArray<TSoftClassPtr<ASpawnPoints>> SpawnPoints;
};

USTRUCT(BlueprintType)
struct FWeaponUpgrades
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name of the upgrade that will be displayed."))
	FName UniqueName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The description of the upgrade that will be displayed."), meta = (MultiLine = true))
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The weight of this upgrade change to roll when called. The weight is caluculated by adding all upgrades to a pool and rolling."))
	float RollWeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The rarity of the upgrade."))
	EUpgradeRarity UpgradeRarity = EUpgradeRarity::Basic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The enum of the upgrade that take affect."))
	EWeaponUpgradeType WeaponUpgrade = EWeaponUpgradeType::PercentDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The value of the upgrade that take affect."))
	float WeaponUpgradeValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "This upgrade is Single Use and will be removed from the pool once selected."))
	bool bSingleUse = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "This upgrade is category based upgrade. All weapons with same weapon type will recieve this upgrade."))
	bool bCategoryUpgrade = false;
};

USTRUCT(BlueprintType)
struct FUpgradeManager
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TSoftObjectPtr<AWeapons> WeaponReference;

	UPROPERTY()
	TArray<FWeaponUpgrades> WeaponUpgrades;

	UPROPERTY()
	int CurrentUpgradeLevel = 0;

	UPROPERTY()
	EWeaponType WeaponType = EWeaponType::Mechanical;

	UPROPERTY()
	bool bIsWeaponUnlock = false;
};

USTRUCT(BlueprintType)
struct FUpgradeCommunication
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TSoftObjectPtr<AWeapons> WeaponReference;

	UPROPERTY()
	FWeaponUpgrades WeaponUpgrades;

	UPROPERTY()
	EWeaponType WeaponType = EWeaponType::Mechanical;

	UPROPERTY()
	bool bIsWeaponUnlock = false;
};