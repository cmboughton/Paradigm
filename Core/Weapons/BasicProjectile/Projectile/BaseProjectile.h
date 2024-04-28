// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"
#include "BaseProjectile.generated.h"

UCLASS()
class PARADIGM_IQ_API ABaseProjectile : public AProjectile
{
	GENERATED_BODY()

protected:

	virtual void TraceCheck(const float DeltaTime) override;

};
