// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"
#include "GrenadeProjectile.generated.h"

UCLASS()
class PARADIGM_IQ_API AGrenadeProjectile : public AProjectile
{
	GENERATED_BODY()
protected:

	virtual void DestroyProjectile() override;

	virtual void TraceCheck(const float DeltaTime) override;
};
