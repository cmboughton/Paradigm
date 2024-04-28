// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Data/Structs/Structs.h"
#include "../Data/Enums/Enums.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PARADIGM_IQ_API AEnemyCharacter : public APawn, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	UFUNCTION()
	float DealDamage(const float InHealth, const float DamageValue);

	UFUNCTION()
	void Death();

	virtual void ChangeCharacterState_Implementation(const ECharacterState CharacterState) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "Health of this actor."))
	float Health = 75.f;

	UPROPERTY()
	float CurrentHealth = 0.f;

	UPROPERTY()
	float DamageImmunity = 0.1f;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The current state the character is in."));
	ECharacterState CurrentCharacterState = ECharacterState::Normal;

public:	
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
