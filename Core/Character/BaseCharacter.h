// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PARADIGM_IQ_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Public Functions
public:

	UFUNCTION()
	void UpdateMovementSpeed(const float Speed) const;

	UFUNCTION()
	virtual void UpdateHealth(const float& HealthIn);

#pragma endregion

#pragma region Protected Functions
protected:

	ABaseCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;

	virtual float TakeDamage(const float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius);

	UFUNCTION()
	virtual void Death();

	UFUNCTION()
	void SpawnActor(const TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform) const;

#pragma endregion

#pragma region Protected Variables
protected:

	UPROPERTY()
	class UStaticMeshComponent* BaseModel;

	UPROPERTY()
	float DamageImmunity = 0.f;

	UPROPERTY()
	ECharacterState CurrentState = ECharacterState::Normal;

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	int ScoringModifier = 1;

	UPROPERTY()
	float Score = 0.f;

#pragma endregion

#pragma region Getters and Setters
public:	

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int						GetScore()					const { return Score; }
	FORCEINLINE ECharacterState			GetCharacterState()			const { return CurrentState; }
	FORCEINLINE UStaticMeshComponent*	GetBaseModel()				const { return BaseModel; }

	FORCEINLINE void					SetCharacterState			(const ECharacterState CharacterState)				{ CurrentState = CharacterState; }
	FORCEINLINE void					SetBaseModel(UStaticMeshComponent* NewMesh) { BaseModel = NewMesh; }
	FORCEINLINE void					SetScore(const float& NewScore) { Score = NewScore; }

#pragma endregion
};
