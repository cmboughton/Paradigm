// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "DataTables.generated.h"

class APassives;
class AUltimateAbility;
class AEnemyCharacter;
class ASpawnPoints;
class AWeapons;

USTRUCT(BlueprintType)
struct FShipsDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name to the Ship."))
	FName ShipName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The static mesh model of the ship."))
	TSoftObjectPtr<UStaticMesh> ShipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "Ship Stats"))
	TArray<FStatsStruct> ShipStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name of the Row from the Ultimate Ability Data Table."))
	FName UltimateAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name of the Row from the Weapons Data Table."))
	FName BaseWeapon;
};

USTRUCT(BlueprintType)
struct FUltimatesDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name to the Ultimate Ability."))
	FName UltimateName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The Icon of the Ultimate."))
	UTexture2D* UltimateIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The Blueprint Reference to the Ultimate Ability."))
	TSoftClassPtr<AUltimateAbility> UltimateAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The amount of Ultimate Experience required."))
	float UltimateExperience = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The amount of Damage this ability will do to enemies hit."))
	float Damage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The time this Ultimate should last for."))
	float Duration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The state the Enemy will be put in."))
	ECharacterState EnemyState = ECharacterState::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The state the Player will be put in."))
	ECharacterState PlayerState = ECharacterState::Normal;
};

USTRUCT(BlueprintType)
struct FWeaponsDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "Debug to force an upgrade on start."))
	TArray<EWeaponUpgradeType> DeBugUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The name of the weapon."))
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The description of the weapon."), meta = (MultiLine = true))
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The Icon of the weapon."))
	UTexture2D* WeaponIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The weapon actor that is to be spawned."))
	TSoftClassPtr<AWeapons> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The type of weapon. Used for category based upgrades."))
	EWeaponType WeaponType = EWeaponType::Mechanical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The time in seconds between each shot."), meta = (ClampMin = 0.1f))
	float FireRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The damage the weapon does."))
	float Damage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "Is this weapon a Base Weapon of a ship and shouldnt be included in the obtainable Weapons from the Upgrade pool."))
	bool bIsBaseWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The weight of this weapon chance to roll when called. The weight is caluculated by adding all upgrades to a pool and rolling."))
	float RollWeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The upgrades that this weapon can have."))
	TArray<FWeaponUpgrades> WeaponUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The Special upgrades that this weapon can have."))
	TArray<FWeaponUpgrades> SpecialWeaponUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The levels to which Special upgrades that this weapon can be added to the weapon."))
	TArray<int> SpecialUpgradeLevels;
};

USTRUCT(BlueprintType)
struct FPassivesDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The name of the Passive."))
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The description of the Passive."), meta = (MultiLine = true))
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The Icon of the passive."))
	UTexture2D* PassiveIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The weapon actor that is to be spawned."))
	TSoftClassPtr<APassives> Passive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The weight of this Passive to roll when called. The weight is caluculated by adding all upgrades to a pool and rolling."))
	float RollWeight = 0.f;
};

USTRUCT(BlueprintType)
struct FEnemiesDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The name of the Enemy."))
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The class of the enemy that will be spawned."))
	TSoftClassPtr<AEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The static mesh to the enemy ship."))
	UStaticMesh* ShipMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The score of the Enemy."))
	float Score = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The stats of the Enemy."))
	TMap<EEnemyStatsType, float> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The experience of the Enemy drops."))
	FExperienceOrb Experience;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The collectable drops of the Enemy."))
	FCollectableStruct DropTable;
};