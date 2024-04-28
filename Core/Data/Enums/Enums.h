// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Normal						UMETA(DisplayName = "Normal"),
	Stunned						UMETA(DisplayName = "Stunned"),
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
enum class ESpawnType : uint8
{
	RandomSpawn					UMETA(DisplayName = "Random Spawn"),
	FixedSpawn					UMETA(DisplayName = "Fixed Spawn"),
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
};

UENUM(BlueprintType)
enum class EUpgradeRarity : uint8
{
	Basic						UMETA(DisplayName = "Basic"),
	Prime						UMETA(DisplayName = "Prime"),
	Cosmic						UMETA(DisplayName = "Cosmic"),
};