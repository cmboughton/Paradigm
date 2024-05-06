// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Collectable/Collectable.h"
#include "HealthCollectable.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AHealthCollectable : public ACollectable
{
	GENERATED_BODY()

protected:

	virtual void Collected() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The health that will be gained when picked up."))
	float AddedHealth = 40.f;
};
