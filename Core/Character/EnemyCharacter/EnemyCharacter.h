// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "Paradigm_IQ/Core/Data/Enums/Enums.h"
#include "Paradigm_IQ/Core/Character/BaseCharacter.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PARADIGM_IQ_API AEnemyCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

protected:

	AEnemyCharacter();

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ChangeCharacterState_Implementation(const ECharacterState CharacterState) override;

	virtual void Death() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The Score of this actor."))
	float Score = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The Experience and Ultimate experience that is dropped on death."))
	struct FExperienceOrb ExperienceStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|References", meta = (ToolTip = "Blueprint ref to experience blueprint."))
	TSubclassOf<class AExperience> ExperienceOrb = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|References", meta = (ToolTip = "Blueprint ref to weapon upgrade blueprint."))
	TSubclassOf<class AWeaponUpgradeCollectable> WeaponUpgradeCollectable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The chance to drop a upgrade collectable. Rolls between 0 and 1."))
	float UpgradeDropChance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|References", meta = (ToolTip = "Blueprint ref to attract orb blueprint."))
	TSubclassOf<class AAttractOrb> AttractOrbCollectable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The chance to drop a Attract Orb collectable. Rolls between 0 and 1."))
	float AttractOrbDropChance = 0.f;

};
