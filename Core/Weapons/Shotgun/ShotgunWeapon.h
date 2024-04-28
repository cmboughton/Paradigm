// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "ShotgunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AShotgunWeapon : public AWeapons
{
	GENERATED_BODY()

protected:

	virtual void WeaponTriggered(const float DeltaTime) override;

};
