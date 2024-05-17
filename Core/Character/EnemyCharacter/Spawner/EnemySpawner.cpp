// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/Character/EnemyCharacter/EnemyCharacter.h"
#include "SpawnPoints/SpawnPoints.h"


AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	TArray<AActor*> SpawnPointsRef;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoints::StaticClass(), SpawnPointsRef);
	for (AActor* PointRef : SpawnPointsRef)
	{
		if(const ASpawnPoints* SpawnPoint = Cast<ASpawnPoints>(PointRef))
		{
			const FBoxSphereBounds Bounds = SpawnPoint->EdgeSpawnerZone->Bounds;
			const FSpawnPointsInfo SpawnInfo = FSpawnPointsInfo(SpawnPoint->GetActorLocation(), Bounds.BoxExtent, Bounds.Origin);
			SpawnPoints.Add(SpawnInfo);
			PointRef->Destroy();
		}
	}
	
}

void AEnemySpawner::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	RealTimeTracker += DeltaTime;

	switch(EnemySpawner.GrowthType)
	{
	case EGrowthModifierType::Linear:
		GrowthTracker += (DeltaTime * EnemySpawner.GrowthModifier);
		break;
	case EGrowthModifierType::Exponential:
		GrowthTracker = GrowthTracker * FMath::Pow((1 + EnemySpawner.GrowthModifier), DeltaTime);
		break;
	}

	for (FEnemySpawnerGrowthModifier& EnemySpawnerMod : EnemySpawner.EnemyModifiers)
	{
		if(GrowthTracker >= EnemySpawnerMod.GrowthModifierTrigger)
		{
			for (FEnemySpawnerModifier& SpawnerModifier: EnemySpawnerMod.EnemyConditions)
			{
				if ((SpawnerModifier.TriggersToEndSpawning.EnemiesSpawnedTracker >= SpawnerModifier.TriggersToEndSpawning.EnemiesSpawned && SpawnerModifier.TriggersToEndSpawning.EnemiesSpawned > 0) ||
					(SpawnerModifier.TriggersToEndSpawning.GameTimeDuration <= RealTimeTracker && SpawnerModifier.TriggersToEndSpawning.GameTimeDuration > 0))
				{
					continue;
				}
				if(SpawnerModifier.SpawnRateTracker <= 0)
				{
					if(	SpawnerModifier.TriggersToStartSpawning.ScoreTrigger <= PlayerCharacter->GetScore() && 
						SpawnerModifier.TriggersToStartSpawning.ScoreModifierTrigger <= PlayerCharacter->GetScoringModifier() && 
						SpawnerModifier.TriggersToStartSpawning.GameTimeDuration <= RealTimeTracker)
					{
						FTransform SpawnTransform;
						FVector RandomSpawnLocation;
						FSpawnPointsInfo EdgeSpawnZone;
						if(!SpawnerModifier.EnemySpawnLocation.IsEmpty())
						{
							const int Roll = FMath::RandRange(0, SpawnerModifier.EnemySpawnLocation.Num() - 1);
							if(SpawnerModifier.EnemySpawnLocation.IsValidIndex(Roll))
							{
								RandomSpawnLocation = SpawnerModifier.EnemySpawnLocation[Roll];
							}
						}
						if(!SpawnPoints.IsEmpty())
						{
							const int Roll = FMath::RandRange(0, SpawnPoints.Num() - 1);
							if(SpawnPoints.IsValidIndex(Roll))
							{
								EdgeSpawnZone = SpawnPoints[Roll];
							}
						}
						switch (SpawnerModifier.SpawnType)
						{
						case ESpawnerType::Corner:

							if (const TSubclassOf<AEnemyCharacter>& EnemyToSpawn = SpawnerModifier.EnemyReference.LoadSynchronous())
							{
								for (int i = 0; i < SpawnerModifier.SpawnAmount; i++)
								{
									SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), FVector(RandomSpawnLocation.X + FMath::RandRange(-CornerScatterDist, CornerScatterDist), RandomSpawnLocation.Y + FMath::RandRange(-CornerScatterDist, CornerScatterDist), RandomSpawnLocation.Z), FVector(1.f, 1.f, 1.f));
									SpawnEnemies(1, EnemyToSpawn, SpawnTransform);
								} 
							}
							break;

						case ESpawnerType::Edge:
							
							if (const TSubclassOf<AEnemyCharacter>& EnemyToSpawn = SpawnerModifier.EnemyReference.LoadSynchronous())
							{
								for (int i = 0; i < SpawnerModifier.SpawnAmount; i++)
								{
									SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), FVector(FMath::RandRange((EdgeSpawnZone.BoxOrigin - EdgeSpawnZone.BoxExtent).X, (EdgeSpawnZone.BoxOrigin + EdgeSpawnZone.BoxExtent).X), FMath::RandRange((EdgeSpawnZone.BoxOrigin - EdgeSpawnZone.BoxExtent).Y, (EdgeSpawnZone.BoxOrigin + EdgeSpawnZone.BoxExtent).Y), SpawnTransform.GetTranslation().Z), FVector(1.f, 1.f, 1.f));
									SpawnEnemies(1, EnemyToSpawn, SpawnTransform);
								}
							}
							
							break;

						case ESpawnerType::Scatter:

							if(PlayerCharacter)
							{
								if(const TSubclassOf<AEnemyCharacter>& EnemyToSpawn = SpawnerModifier.EnemyReference.LoadSynchronous())
								{
									for (int i = 0; i < SpawnerModifier.SpawnAmount; i++)
									{
										SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetRandomPointNearOrigin(PlayerCharacter->GetActorLocation(), ScatterMinDist, ScatterMaxDist), FVector(1.f, 1.f, 1.f));
										SpawnEnemies(1, EnemyToSpawn, SpawnTransform);
									}
								}
							}
							break;

						case ESpawnerType::SpecificLocation:

							SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), RandomSpawnLocation, FVector(1.f, 1.f, 1.f));
							if (const TSubclassOf<AEnemyCharacter>& EnemyToSpawn = SpawnerModifier.EnemyReference.LoadSynchronous())
							{
								SpawnEnemies(SpawnerModifier.SpawnAmount, EnemyToSpawn, SpawnTransform);
							}
							break;

						default:
							if (PlayerCharacter)
							{
								SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetRandomPointNearOrigin(PlayerCharacter->GetActorLocation(), ScatterMinDist, ScatterMaxDist), FVector(1.f, 1.f, 1.f));
								if (const TSubclassOf<AEnemyCharacter>& EnemyToSpawn = SpawnerModifier.EnemyReference.LoadSynchronous())
								{
									SpawnEnemies(SpawnerModifier.SpawnAmount, EnemyToSpawn, SpawnTransform);
								}
							}
							break;
						}

						SpawnerModifier.TriggersToEndSpawning.EnemiesSpawnedTracker += SpawnerModifier.SpawnAmount;
					}
					else
					{
						continue;
					}
					
					SpawnerModifier.SpawnRateTracker = SpawnerModifier.SpawnRate / EnemySpawner.GrowthModifier;
				}
				else
				{
					SpawnerModifier.SpawnRateTracker -= DeltaTime;
				}
			}
		}
	}
}

/**
 * @brief Generates a random point near a given origin within a specified distance range.
 *
 * This function generates a random point in the vicinity of a given origin point. 
 * The distance of the generated point from the origin is within a specified minimum and maximum range.
 *
 * @param Origin The origin point from which the random point is to be generated.
 * @param MinDistance The minimum distance from the origin that the generated point can be.
 * @param MaxDistance The maximum distance from the origin that the generated point can be.
 * @return Returns the generated random point as an FVector.
 */
FVector AEnemySpawner::GetRandomPointNearOrigin(const FVector& Origin, const float MinDistance, const float MaxDistance)
{
	// Create a random stream for generating random values
	const FRandomStream RandStream(FMath::Rand());

	// Generate a random direction vector with a random magnitude
	FVector RandomDirection = RandStream.VRand().GetSafeNormal() * RandStream.FRandRange(MinDistance, MaxDistance);

	FVector RandomPoint = FVector(Origin.X + RandomDirection.X, Origin.Y + RandomDirection.Y, Origin.Z);

	while (FVector::DistSquared(Origin, RandomPoint) < MinDistance * MinDistance)
	{
		RandomDirection = RandStream.VRand().GetSafeNormal() * RandStream.FRandRange(MinDistance, MaxDistance);
		RandomPoint = FVector(Origin.X + RandomDirection.X, Origin.Y + RandomDirection.Y, Origin.Z);
	}

	//UE_LOGFMT(LogTemp, Warning, "Random EnemySpawned at {0}", RandomPoint.Z);
	return RandomPoint;
}

/**
 * @brief Spawns a specified amount of enemies in the game world.
 * 
 * This function spawns a specified amount of enemies of a specified type at a specified location and orientation in the game world.
 * 
 * @param AmountToSpawn The number of enemies to spawn.
 * @param ActorToSpawn The type of enemy to spawn.
 * @param SpawnTransform The location and orientation at which to spawn the enemies.
 */
void AEnemySpawner::SpawnEnemies(const int AmountToSpawn, const TSubclassOf<AEnemyCharacter> ActorToSpawn, const FTransform& SpawnTransform) const
{
	for(int i = 0; i < AmountToSpawn; i++)
	{
		if(AEnemyCharacter* EnemySpawned = GetWorld()->SpawnActorDeferred<AEnemyCharacter>(ActorToSpawn, SpawnTransform))
		{
			EnemySpawned->UpdateStats(GrowthTracker / 400);
			EnemySpawned->FinishSpawning(SpawnTransform);
		}
	}
}
