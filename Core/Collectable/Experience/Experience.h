// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Collectable/Collectable.h"
#include "Experience.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AExperience : public ACollectable
{
	GENERATED_BODY()

protected:

	virtual void Collected() override;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	float Experience = 0.f;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	float UltimateExperience = 0.f;

public:

	UFUNCTION()
	void SetUp(const FExperienceOrb& ExperienceStruct);
};
