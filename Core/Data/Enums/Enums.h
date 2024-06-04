// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Normal						UMETA(DisplayName = "Normal"),
	Stunned						UMETA(DisplayName = "Stunned"),
	Invulnerable				UMETA(DisplayName = "Invulnerable"),
	Death						UMETA(DisplayName = "Death"),
};

UENUM(BlueprintType)
enum class EEnemyClass : uint8
{
	Normal						UMETA(DisplayName = "Normal"),
	Elite						UMETA(DisplayName = "Elite"),
	Boss						UMETA(DisplayName = "Boss"),
};

UENUM(BlueprintType)
enum class EDamageComType : uint8
{
	SphereOverlap				UMETA(DisplayName = "Sphere Overlap"),
	MeshHit						UMETA(DisplayName = "Mesh Hit"),
};

UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	Mechanical					UMETA(DisplayName = "Mechanical"),
	Energy						UMETA(DisplayName = "Energy"),
	Explosive					UMETA(DisplayName = "Explosive"),
};

UENUM(BlueprintType)
enum class EWeaponUpgradeType : uint8
{
	PercentDamage				UMETA(DisplayName = "Percent Damage"),
	PercentFireRate				UMETA(DisplayName = "Percent Fire Rate"),
	PercentAffectRadius			UMETA(DisplayName = "Percent Affect Radius"),
	TriggerAmount				UMETA(DisplayName = "Trigger Amount"),
	SpecialUpgrade1				UMETA(DisplayName = "Special Upgrade 1"),
	SpecialUpgrade2				UMETA(DisplayName = "Special Upgrade 2"),
	SpecialUpgrade3				UMETA(DisplayName = "Special Upgrade 3"),
	SpecialUpgrade4				UMETA(DisplayName = "Special Upgrade 4"),
	SpecialUpgrade5				UMETA(DisplayName = "Special Upgrade 5"),
};

UENUM(BlueprintType)
enum class EUpgradeRarity : uint8
{
	Basic						UMETA(DisplayName = "Basic"),
	Prime						UMETA(DisplayName = "Prime"),
	Cosmic						UMETA(DisplayName = "Cosmic"),
};

UENUM(BlueprintType)
enum class EGrowthModifierType : uint8
{
	Linear						UMETA(DisplayName = "Linear"),
	Exponential					UMETA(DisplayName = "Exponential"),
};

UENUM(BlueprintType)
enum class ESpawnerType : uint8
{
	Corner						UMETA(DisplayName = "Corner"),
	Edge						UMETA(DisplayName = "Edge"),
	Scatter						UMETA(DisplayName = "Scatter"),
	SpecificLocation			UMETA(DisplayName = "Specific Location"),
};

UENUM(BlueprintType)
enum class EIconType : uint8
{
	WeaponIcon					UMETA(DisplayName = "Weapon Icon"),
	PassiveIcon					UMETA(DisplayName = "Passive Icon"),
};

UENUM(BlueprintType)
enum class EPlayerStatsType : uint8
{
	Default						UMETA(DisplayName = "Default"),
	Health						UMETA(DisplayName = "Health"),
	HealthRegen					UMETA(DisplayName = "Health Regen"),
	DamageModifier				UMETA(DisplayName = "Damage Modifier"),
	CritChanceModifier			UMETA(DisplayName = "Crit Chance Modifier"),
	CritDamageModifier			UMETA(DisplayName = "Crit Damage Modifier"),
	MovementSpeed				UMETA(DisplayName = "Movement Speed"),
};

UENUM(BlueprintType)
enum class EEnemyStatsType : uint8
{
	Default						UMETA(DisplayName = "Default"),
	AttackRange					UMETA(DisplayName = "Attack Range"),
	Health						UMETA(DisplayName = "Health"),
	Damage						UMETA(DisplayName = "Damage"),
	CritChance					UMETA(DisplayName = "Crit Chance"),
	CritDamage					UMETA(DisplayName = "Crit Damage"),
	MovementSpeed				UMETA(DisplayName = "Movement Speed"),
};