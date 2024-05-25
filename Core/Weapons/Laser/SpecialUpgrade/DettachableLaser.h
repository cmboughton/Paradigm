// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DettachableLaser.generated.h"

UCLASS()
class PARADIGM_IQ_API ADettachableLaser : public AActor
{
	GENERATED_BODY()

protected:

	ADettachableLaser();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetLaserTransform(const int& MeshIndex, const FTransform& LaserTransform);

	UFUNCTION()
	TArray<FHitResult> LineTrace(const FVector& ActorStartLocation, const FVector& ActorEndLocation);

	UFUNCTION()
	void ApplyDamage(const TArray<FHitResult>& AllActorsHit);

	UPROPERTY()
	FRotator SweepTracker;

	UPROPERTY()
	float LaserLengthTracker = 0.f;

	UPROPERTY()
	TArray<FHitResult> ActorsHit;

	UPROPERTY()
	int TriggerAmount = 1;

	UPROPERTY()
	float Damage = 0.f;

	UPROPERTY(meta = (ClampMin = 0.1f))
	float FireRate = 0.f;

	UPROPERTY()
	TArray<UStaticMeshComponent*> LaserMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Reference", meta = (ToolTip = "The static mesh that the laser will be."))
	UStaticMesh* LaserMeshRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The duration the lasers last for."))
	float LaserDuration = 10.f;

public:

	FORCEINLINE void						SetTriggerAmount(const int& TriggerAmountValue)			{ TriggerAmount = TriggerAmountValue; }
	FORCEINLINE void						SetDamage(const int& DamageValue)						{ Damage = DamageValue; }
	FORCEINLINE void						SetFireRate(const int& FireRateValue)					{ FireRate = FireRateValue; }
	FORCEINLINE void						SetLaserLength(const int& LaserLengthValue)				{ LaserLengthTracker = LaserLengthValue; }
};
