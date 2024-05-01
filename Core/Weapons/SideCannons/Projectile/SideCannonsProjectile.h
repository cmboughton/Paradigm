// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"
#include "SideCannonsProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API ASideCannonsProjectile : public AProjectile
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void TraceCheck(const float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "Reference to the Base Projectile Spawn."))
	TSubclassOf<ASideCannonsProjectile> BaseProjectile = nullptr;

	UPROPERTY()
	bool bIsSpawner = true;

public:

	FORCEINLINE void	SetIsSpawner	(const bool IsSpawnerValue) { bIsSpawner = IsSpawnerValue; }
};
