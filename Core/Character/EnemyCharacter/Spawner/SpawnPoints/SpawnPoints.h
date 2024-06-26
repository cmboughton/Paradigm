// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Paradigm_IQ/Core/Data/Enums/Enums.h"
#include "SpawnPoints.generated.h"

UCLASS()
class PARADIGM_IQ_API ASpawnPoints : public ATargetPoint
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* EdgeSpawnerZone;

	UPROPERTY(EditAnywhere, Category = "Variables")
	ESpawnerType SpawnType = ESpawnerType::Edge;

	UPROPERTY(EditAnywhere, Category = "Variables")
	AActor* PatrolPoint = nullptr;

protected:

	ASpawnPoints();
};
