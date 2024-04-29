// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paradigm_IQ/Core/Data/Enums/Enums.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"
#include "Paradigm_IQ/Core/PlayerCharacter/PlayerCharacter.h"
#include "Logging/StructuredLog.h"
#include "UltimateAbility.generated.h"


UCLASS()
class PARADIGM_IQ_API AUltimateAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUltimateAbility();


	UFUNCTION(BlueprintImplementableEvent)
	void UltimateAbilityStartBP();


	UFUNCTION(BlueprintImplementableEvent)
	void UltimateAbilityFinishBP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void UltimateAbilityStart();

	UFUNCTION()
	virtual void UltimateAbilityFinish();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius);

	UFUNCTION()
	virtual TArray<FHitResult> LineTrace(const FVector ActorStartLocation, const FVector ActorEndLocation);

	UFUNCTION()
	void ApplyDamage(const FHitResult ActorHit);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ultimates."))
	TSoftObjectPtr<UDataTable> UltimatesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The name to the row in the data table of this Ultimate."))
	FName UltimateName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The max radius the explosion can reach."))
	float AffectRadius = 10000.f;

	UPROPERTY()
	float Damage = 0.f;

	UPROPERTY()
	float Duration = 0.f;

	UPROPERTY()
	ECharacterState CharacterState = ECharacterState::Normal;

	UPROPERTY()
	ECharacterState EnemyState = ECharacterState::Normal;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

public:	

};
