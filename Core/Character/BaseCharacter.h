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

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseModel;

protected:

	ABaseCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;

	virtual float TakeDamage(const float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius);

	UFUNCTION()
	virtual void Death();

public:

	UFUNCTION()
	void UpdateMovementSpeed(const float Speed) const;

protected:

	UPROPERTY()
	float DamageImmunity = 0.f;

	UPROPERTY()
	ECharacterState CurrentState = ECharacterState::Normal;

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The Max Health of this Character."))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly)
	int ScoringModifier = 1;

public:	

	FORCEINLINE float					GetHealth()					const { return CurrentHealth; }
	FORCEINLINE float					GetMaxHealth()				const { return MaxHealth; }
	FORCEINLINE ECharacterState			GetCharacterState()			const { return CurrentState; }
	FORCEINLINE UStaticMeshComponent*	GetBaseModel()				const { return BaseModel; }

	FORCEINLINE void					SetCharacterState			(const ECharacterState CharacterState)				{ CurrentState = CharacterState; }
};
