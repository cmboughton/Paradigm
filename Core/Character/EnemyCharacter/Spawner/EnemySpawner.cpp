// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/Character/EnemyCharacter/EnemyCharacter.h"



// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RealTimeTracker += DeltaTime;


	switch(EnemySpawner.GrowthType)
	{
	case EGrowthModifierType::Linear:
		GrowthTracker += (DeltaTime * EnemySpawner.GrowthModifier);
		UE_LOGFMT(LogTemp, Warning, "Growth Tracker: {0}", GrowthTracker);
		break;
	case EGrowthModifierType::Exponential:
		GrowthTracker = GrowthTracker * FMath::Pow((1 + EnemySpawner.GrowthModifier), DeltaTime);
		UE_LOGFMT(LogTemp, Warning, "Growth Tracker: {0}", GrowthTracker);
		break;
	}

	// This needs to be reworked. When enemies are spawned they need their stats to be set rather than updating them periodically.
	// This will have to be part of the spawning section rather than its own.
	if(StatUpdateTimeTracker <= 0)
	{
		StatUpdateTimeTracker = 30.f;
		TArray<AActor*> AllEnemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), AllEnemies);
		for (AActor* Enemy : AllEnemies)
		{
			if (AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(Enemy))
			{
				EnemyChar->UpdateStats(GrowthTracker);
			}
		}
		AllEnemies.Empty();
	}

	for (FEnemySpawnerGrowthModifier EnemySpawnerMod : EnemySpawner.EnemyModifiers)
	{
		if(GrowthTracker >= EnemySpawnerMod.GrowthModifierTrigger)
		{
			bool bCanSpawnTracker = false;
			for (FEnemySpawnerModifier SpawnerModifier: EnemySpawnerMod.EnemyConditions)
			{
				if(SpawnerModifier.SpawnRateTracker <= 0)
				{
					if(SpawnerModifier.TriggersToStartSpawning.ScoreTrigger > 0 || SpawnerModifier.TriggersToStartSpawning.ScoreModifierTrigger > 0 || SpawnerModifier.TriggersToStartSpawning.GameTimeDuration > 0)
					{
						if(SpawnerModifier.TriggersToStartSpawning.ScoreTrigger <= PlayerCharacter->GetScore() && SpawnerModifier.TriggersToStartSpawning.ScoreModifierTrigger <= PlayerCharacter->GetScoringModifier() && SpawnerModifier.TriggersToStartSpawning.GameTimeDuration <= RealTimeTracker)
						{
							
						}
						else
						{
							continue;
						}
					}
					SpawnerModifier.SpawnRateTracker = SpawnerModifier.SpawnRate;

				}
				else
				{
					SpawnerModifier.SpawnRateTracker -= DeltaTime;
				}
			}
		}
	}
}