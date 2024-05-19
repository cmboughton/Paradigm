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

#pragma region Protected Functions
protected:

	AEnemySpawner();

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;

	UFUNCTION()
	static FVector GetRandomPointNearOrigin(const FVector& Origin, const float MinDistance, const float MaxDistance);

	UFUNCTION()
	void SpawnEnemies(const int AmountToSpawn, const TSubclassOf<AEnemyCharacter> ActorToSpawn, const FTransform& SpawnTransform) const;

#pragma endregion

#pragma region Protected Variables
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The Minimum distance that enemies will spawn from the player for scatter spawn."))
	float ScatterMinDist = 800;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "The Maximum distance that enemies will spawn from the player for scatter spawn."))
	float ScatterMaxDist = 1800;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "A randomized distance to spawn enemies seperated from eachother at corner spawn."))
	float CornerScatterDist = 200;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles", meta = (ToolTip = "This value divides the Growth Modifier tracker. Lower = Stats increase faster | Higher = Stats increase slower."))
	float StatMultiplier = 400;

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

	UPROPERTY()
	TArray<FSpawnPointsInfo> SpawnPointsEdge;

	UPROPERTY()
	TArray<FSpawnPointsInfo> SpawnPointsCorner;

#pragma endregion
};
