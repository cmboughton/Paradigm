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
	void SpawnEnemies(const int AmountToSpawn, const FName& RowName, const FTransform& SpawnTransform) const;

	UFUNCTION()
	TSubclassOf<AEnemyCharacter> FindEnemyClass(const FName& RowName) const;

#pragma endregion

#pragma region Protected Variables
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ships."))
	UDataTable* EnemyDataTable = nullptr;

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

	UPROPERTY()
	TArray<FPatrolPointStruct> PatrolPoints;

	UPROPERTY()
	FPatrolPointStruct PatrolPoint = FPatrolPointStruct(FVector(0.f, 0.f, 0.f), FVector(0.f, 0.f, 0.f));

#pragma endregion
};
