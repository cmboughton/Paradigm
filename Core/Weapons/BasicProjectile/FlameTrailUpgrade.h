// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlameTrailUpgrade.generated.h"


UCLASS()
class PARADIGM_IQ_API AFlameTrailUpgrade : public AActor
{
	GENERATED_BODY()

protected:

	AFlameTrailUpgrade();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

	UPROPERTY()
	float AffectRadius = 0.f;

	UPROPERTY()
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The duration before this Actor gets destroyed."))
	float DestroyDuration = 4.f;

public:	

	FORCEINLINE void	SetDamage			(const float DamageValue)				{ Damage = DamageValue; }
	FORCEINLINE void	SetAffectRadius		(const float ExplosionRadiusValue)		{ AffectRadius = ExplosionRadiusValue; }
	FORCEINLINE void	SetStartLocation	(const FVector StartLocationValue)		{ StartLocation = StartLocationValue; }
	FORCEINLINE void	SetEndLocation		(const FVector EndLocationValue)		{ EndLocation = EndLocationValue; }

};
