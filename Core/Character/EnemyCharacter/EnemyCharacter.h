// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "Paradigm_IQ/Core/Data/Enums/Enums.h"
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

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The Damage of this actor."))
	float Damage = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The attack range of this actor."))
	float AttackRange = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The Experience and Ultimate experience that is dropped on death."))
	struct FExperienceOrb ExperienceStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|References", meta = (ToolTip = "Blueprint ref to experience blueprint."))
	TSubclassOf<class AExperience> ExperienceOrb = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "Collectables that can drop"))
	FCollectableStruct CollectableLootTable;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

public:

	UFUNCTION()
	void UpdateStats(const float& GrowthModifier);
};
