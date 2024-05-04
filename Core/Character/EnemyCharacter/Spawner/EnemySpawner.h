// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"
#include "EnemySpawner.generated.h"

UCLASS()
class PARADIGM_IQ_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

protected:

	AEnemySpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "Struct of the Enemy Spawner."))
	FEnemySpawnerStruct EnemySpawner;

	UPROPERTY()
	float RealTimeTracker = 0.f;

	UPROPERTY()
	float StatUpdateTimeTracker = 0.f;

	UPROPERTY()
	float GrowthTracker = 0.1f;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;
};
