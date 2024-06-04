// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Character/BaseCharacter.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;

UCLASS()
class PARADIGM_IQ_API AEnemyCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

protected:

	AEnemyCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ChangeCharacterState_Implementation(const ECharacterState CharacterState) override;

	virtual void Death() override;

	UFUNCTION()
	void ApplyBackDamage(float DamageAmount, AActor* DamageCauser);

	UFUNCTION()
	float CalculateDamage() const;

protected:

	UPROPERTY()
	float Damage = 75.f;

	UPROPERTY()
	float AttackRange = 100.f;

	UPROPERTY()
	float CritDamage = 0.f;

	UPROPERTY()
	float CritChance = 0.f;

	UPROPERTY()
	struct FExperienceOrb ExperienceStruct;

	UPROPERTY()
	TSubclassOf<class AExperience> ExperienceOrb = nullptr;

	UPROPERTY()
	FCollectableStruct CollectableLootTable;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY()
	bool bHasAttacked = false;

public:

	UFUNCTION()
	void UpdateStats(const float& GrowthModifier);

	FORCEINLINE void						SetMaxHealth(const float& NewMaxHealth)											{ MaxHealth = NewMaxHealth; }
	FORCEINLINE void						SetDamage(const float& NewDamage)												{ Damage = NewDamage; }
	FORCEINLINE void						SetAttackRange(const float& NewAttackRange)										{ AttackRange = NewAttackRange; }
	FORCEINLINE void						SetCritChance(const float& NewCritChance)										{ CritChance = NewCritChance; }
	FORCEINLINE void						SetCritDamage(const float& NewCritDamage)										{ CritDamage = NewCritDamage; }
	FORCEINLINE void						SetExperienceStruct(const FExperienceOrb& NewExperienceStruct)					{ ExperienceStruct = NewExperienceStruct; }
	FORCEINLINE void						SetCollectableLootTable(const FCollectableStruct& NewCollectable)				{ CollectableLootTable = NewCollectable; }
};
