// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Ultimate/Explosive/Explosive.h"
#include "EMPUltimate.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AEMPUltimate : public AExplosive
{
	GENERATED_BODY()

protected:
	
	virtual void UltimateAbilityStart() override;
};
