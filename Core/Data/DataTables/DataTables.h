// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Paradigm_IQ/Core/Data/Enums/Enums.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "DataTables.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The Health of the ship."))
	float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The base movement speed of the ship."))
	float MovementSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name of the Row from the Ultimate Ability Data Table."))
	FName UltimateAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name of the Row from the Weapons Data Table."))
	FName BaseWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name of the Row from the Weapons Data Table."))
	FName Weapon1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name of the Row from the Weapons Data Table."))
	FName Weapon2;
};

USTRUCT(BlueprintType)
struct FUltimatesDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name to the Ultimate Ability."))
	FName UltimateAbilityName;

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
struct FEnemyStageSpawner : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Conditions", meta = (ToolTip = "The time(in seconds) until this group of enemies will spawn."))
	float TimeCondition = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Conditions", meta = (ToolTip = "If the total points gained is higher than Points Condition then these enemies will spawn."))
	float PointsCondition = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies", meta = (ToolTip = "The the spawn category these enemies fit into."))
	EEnemyClass EnemyClassType = EEnemyClass::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies", meta = (ToolTip = "An array of the enemies that will be added to the spawning point if conditions are meet."))
	TArray<FEnemyType> EnemyTypes;
};

USTRUCT(BlueprintType)
struct FWeaponsDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The name of the weapon."))
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The weapon actor that is to be spawned."))
	TSoftClassPtr<AWeapons> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The type of weapon. Used for category based upgrades."))
	EWeaponType WeaponType = EWeaponType::Mechanical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The time in seconds between each shot."), meta = (ClampMin = 0.1f))
	float FireRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The damage the weapon does."))
	float Damage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|AutoShooter", meta = (ToolTip = "The upgrades that this weapon can have."))
	TArray<FWeaponUpgrades> WeaponUpgrades;
};