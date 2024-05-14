// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Logging/StructuredLog.h"
#include "Projectile.generated.h"


class APlayerCharacter;

UCLASS()
class PARADIGM_IQ_API AProjectile : public AActor
{
	GENERATED_BODY()

#pragma region Componnents
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BulletMesh;

#pragma endregion

#pragma region Protected Stats
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The duration before this Actor gets destroyed."))
	float DestroyDuration = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The intial speed of this actor."))
	float ProjectileSpeed = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The scale of how much gravity will affect this actor."))
	float GravityScale = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The mass of the mesh."))
	float MeshMass = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "If this projectile should receive damage back from enemies based on how much damage it did to actor hit."))
	bool bShouldTakeDamage = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "Apply an arced impulse on the projectile, giving it a lobbing feel."))
	bool bIsArcImpulse = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The imupulse to apply is bIsArcImpulse is true."))
	float Impulse = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "This will auto size the mesh based on AffectRadius."))
	bool bAutoSizeMesh = true;

	UPROPERTY()
	float Damage = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float AffectRadius = 0.f;

	UPROPERTY()
	int TriggerAmount = 0;

	UPROPERTY()
	TArray<FHitResult> ActorsHit;

	UPROPERTY()
	AActor* PlayerCharacter = nullptr;

	UPROPERTY()
	bool bSpecialUpgrade1 = false;

	UPROPERTY()
	bool bSpecialUpgrade2 = false;

	UPROPERTY()
	bool bSpecialUpgrade3 = false;

	UPROPERTY()
	bool bSpecialUpgrade4 = false;

	UPROPERTY()
	bool bSpecialUpgrade5 = false;

#pragma endregion

#pragma region Protected Functions
protected:

	AProjectile();

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual float TakeDamage(const float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void DestroyProjectile();

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector& ActorStartLocation, const FVector& ActorEndLocation, const float& TraceRadius);

	UFUNCTION()
	void ApplyDamage(const TArray<FHitResult>& AllActorsHit);

	UFUNCTION()
	virtual void TraceCheck(const float& DeltaTime);

	virtual void Tick(const float DeltaTime) override;

#pragma endregion

#pragma region Setters and Getters
public:	

	FORCEINLINE void	SetDamage				(const float DamageValue)					{ Damage = DamageValue; }
	FORCEINLINE void	SetAffectRadius			(const float ExplosionRadiusValue)			{ AffectRadius = ExplosionRadiusValue; }
	FORCEINLINE void	SetSpecialUpgrade1		(const bool SpecialUpgradeValue)			{ bSpecialUpgrade1 = SpecialUpgradeValue; }
	FORCEINLINE void	SetSpecialUpgrade2		(const bool SpecialUpgradeValue)			{ bSpecialUpgrade2 = SpecialUpgradeValue; }
	FORCEINLINE void	SetSpecialUpgrade3		(const bool SpecialUpgradeValue)			{ bSpecialUpgrade3 = SpecialUpgradeValue; }
	FORCEINLINE void	SetSpecialUpgrade4		(const bool SpecialUpgradeValue)			{ bSpecialUpgrade4 = SpecialUpgradeValue; }
	FORCEINLINE void	SetSpecialUpgrade5		(const bool SpecialUpgradeValue)			{ bSpecialUpgrade5 = SpecialUpgradeValue; }
	FORCEINLINE void	SetIsArcImpulse			(const bool IsArcImpulseValue)				{ bIsArcImpulse = IsArcImpulseValue; }
	FORCEINLINE void	SetImpulse				(const float ImpulseValue)					{ Impulse = ImpulseValue; }
	FORCEINLINE void	SetTriggerAmount		(const int TriggerValue)					{ TriggerAmount = TriggerValue; }
	FORCEINLINE void	SetDestroyDuration		(const float DestroyDurationValue)			{ DestroyDuration = DestroyDurationValue; }
	FORCEINLINE void	SetPlayerCharacter		(AActor* PlayerCharRef)						{ PlayerCharacter = PlayerCharRef; }
#pragma endregion
};
