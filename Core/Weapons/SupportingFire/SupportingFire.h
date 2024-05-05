// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "SupportingFire.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API ASupportingFire : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

public:

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponTriggerBP(FVector SpawnLocation);
};
