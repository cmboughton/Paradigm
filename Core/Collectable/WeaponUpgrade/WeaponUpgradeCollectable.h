// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Collectable/Collectable.h"
#include "WeaponUpgradeCollectable.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AWeaponUpgradeCollectable : public ACollectable
{
	GENERATED_BODY()

protected:

	virtual void Collected(APlayerCharacter* PlayerCharacterRef) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Varaiables|Stats", meta = (ToolTip = "The amount of upgrades the manager should roll."))
	int UpgradeRolls = 3;

};
