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

USTRUCT(BlueprintType)
struct FEnemySpawnerStartTriggers
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0), meta = (ToolTip = "The this variable is modified, this variable will determine when these group of enemeis should spawn based on the Overall Score."))
	int ScoreTrigger = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0), meta = (ToolTip = "The this variable is modified, this variable will determine when these group of enemeis should spawn based on the Score Modifier."))
	int ScoreModifierTrigger = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0), meta = (ToolTip = "The this variable is modified, this variable will determine when these group of enemeis should spawn. (In Seconds)"))
	float GameTimeDuration = 0.f;
};

USTRUCT(BlueprintType)
struct FEnemySpawnerEndTriggers
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0), meta = (ToolTip = "The this variable is modified, this variable will determine when these group of enemeis should spawn. It will track how many of these type of enemies spawned and will be used to compare to this variable."))
	int EnemiesSpawned = 0;

	UPROPERTY()
	int EnemiesSpawnedTracker = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0), meta = (ToolTip = "The this variable is modified, this variable will determine when these group of enemeis should spawn. (In Seconds)"))
	float GameTimeDuration = 0.f;
};

USTRUCT(BlueprintType)
struct FEnemySpawnerModifier
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "Reference to the blueprint of the enemy that will Spawn."))
	TSoftClassPtr<AEnemyCharacter> EnemyReference = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "How the enemies should spawn."))
	ESpawnerType SpawnType = ESpawnerType::Scatter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The Duration in seconds until these enemies spawn again."))
	float SpawnRate = 5.f;

	UPROPERTY()
	float SpawnRateTracker = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "These Triggers are checked and if they are modified then they will need to be met to START the spawning of this enemy."))
	FEnemySpawnerStartTriggers TriggersToStartSpawning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "These Triggers are checked and if they are modified then they will need to be met to STOP the spawning of this enemy."))
	FEnemySpawnerEndTriggers TriggersToEndSpawning;
};

USTRUCT(BlueprintType)
struct FEnemySpawnerGrowthModifier
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "This is the Trigger which determines when the enemies in the Enemy Array will be able to start spawning if their conditions are met."))
	float GrowthModifierTrigger = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "This is this struct that holds all of the Enemies that will spawn if the Growth Modifier Trigger is met and all of the conditions within the struct are met."))
	TArray<FEnemySpawnerModifier> EnemyConditions;
};

USTRUCT(BlueprintType)
struct FEnemySpawnerStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The Growth Modifier is used to determine how quick enemies get strong. Lower = Slower | Higher = Faster"))
	float GrowthModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The formula the Growth Modifier will use."))
	EGrowthModifierType GrowthType = EGrowthModifierType::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The formula the Growth Modifier will use."))
	TArray<FEnemySpawnerGrowthModifier> EnemyModifiers;
};

USTRUCT(BlueprintType)
struct FCollectableRoll
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "Blueprint ref to Collectbale to spawn."))
	TSoftClassPtr<class ACollectable> Collectable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The weight that will determine the probability of this collectable being dropped."))
	float RollWeight = 0.f;
};

USTRUCT(BlueprintType)
struct FCollectableStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The drop chance that will determine if the actor will spawn a Collectable based on the roll weight."))
	float DropChance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The Collectbale loot table that can drop."))
	TArray<FCollectableRoll> Collectables;
};