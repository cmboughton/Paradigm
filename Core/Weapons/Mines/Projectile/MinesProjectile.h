// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"
#include "MinesProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AMinesProjectile : public AProjectile
{
	GENERATED_BODY()

#pragma region Protected Functions
protected:

	virtual void TraceCheck(const float& DeltaTime) override;

#pragma endregion

#pragma region Protected Variables
protected:

	UPROPERTY()
	bool bCheckForCollision = true;

	UPROPERTY()
	bool bReadyToDestroy = false;

#pragma endregion
};
