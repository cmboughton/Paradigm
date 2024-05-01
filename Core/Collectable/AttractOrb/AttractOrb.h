// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Collectable/Collectable.h"
#include "AttractOrb.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AAttractOrb : public ACollectable
{
	GENERATED_BODY()

protected:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TArray<AActor*> ActorsFound;
};
