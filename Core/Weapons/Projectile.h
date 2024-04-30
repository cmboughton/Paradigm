// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DamageEvents.h"
#include "Paradigm_IQ/Core/Data/Enums/Enums.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/PlayerCharacter/PlayerCharacter.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "Projectile.generated.h"



UCLASS()
class PARADIGM_IQ_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Componnents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BulletMesh;


#pragma endregion

#pragma region Protected Stats

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

	UPROPERTY()
	float AffectRadius = 0.f;

	UPROPERTY()
	TArray<FHitResult> ActorsHit;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY()
	bool bSpecialUpgrade1 = false;

	UPROPERTY()
	bool bSpecialUpgrade2 = false;

	UPROPERTY()
	bool bSpecialUpgrade3 = false;

#pragma endregion

#pragma region Protected Functions

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	UFUNCTION()
	virtual void DestroyProjectile();

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius);

	UFUNCTION()
	void ApplyDamage(const TArray<FHitResult> AllActorsHit);

	UFUNCTION()
	virtual void TraceCheck(const float DeltaTime);

	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:	

#pragma region Public Setters

	FORCEINLINE void	SetDamage				(const float DamageValue)					{ Damage = DamageValue; }
	FORCEINLINE void	SetAffectRadius			(const float ExplosionRadiusValue)			{ AffectRadius = ExplosionRadiusValue; }
	FORCEINLINE void	SetSpecialUpgrade1		(const bool SpecialUpgrade1Value)			{ bSpecialUpgrade1 = SpecialUpgrade1Value; }
	FORCEINLINE void	SetSpecialUpgrade2		(const bool SpecialUpgrade1Value)			{ bSpecialUpgrade2 = SpecialUpgrade1Value; }
	FORCEINLINE void	SetSpecialUpgrade3		(const bool SpecialUpgrade1Value)			{ bSpecialUpgrade3 = SpecialUpgrade1Value; }
	FORCEINLINE void	SetIsArcImpulse			(const bool IsArcImpulseValue)				{ bIsArcImpulse = IsArcImpulseValue; }
	FORCEINLINE void	SetImpulse				(const float ImpulseValue)					{ Impulse = ImpulseValue; }

#pragma endregion
};
