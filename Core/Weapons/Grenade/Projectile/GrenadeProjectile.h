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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "Reference to the Grenade BP to spawn for the Bouncing Betty upgrade."))
	const TSubclassOf<AGrenadeProjectile> GrenadeProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The amount of grenades to spawn with Bouncing Betty upgrade."))
	int BouncingBettySpawn;
};
