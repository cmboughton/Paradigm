// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DamageEvents.h"
#include "Paradigm_IQ/Core/Data/Enums/Enums.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/PlayerCharacter/PlayerCharacter.h"
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

#pragma endregion

#pragma region Protected Functions

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	UFUNCTION()
	virtual void DestroyProjectile();

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius);

	UFUNCTION()
	void ApplyDamage(const FHitResult ActorHit);

	UFUNCTION()
	virtual void TraceCheck(const float DeltaTime);

#pragma endregion
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Public Setters

	FORCEINLINE void	SetDamage				(const float DamageValue)					{ Damage = DamageValue; }
	FORCEINLINE void	SetAffectRadius		(const float ExplosionRadiusValue)			{ AffectRadius = ExplosionRadiusValue; }

#pragma endregion
};
