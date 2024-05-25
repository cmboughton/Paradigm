// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Passives/Passives.h"
#include "ExtraLife.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AExtraLife : public APassives
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
};
