// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/Character/EnemyCharacter/EnemyCharacter.h"
#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"
#include "SpawnPoints/SpawnPoints.h"


struct FEnemiesDataTable;

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
			const FSpawnPointsInfo SpawnEdgeInfo = FSpawnPointsInfo(SpawnPoint->GetActorLocation(), Bounds.BoxExtent, Bounds.Origin);
			const FSpawnPointsInfo SpawnCornerInfo = FSpawnPointsInfo(SpawnPoint->GetActorLocation(),FVector(0, 0, 0), FVector(0, 0, 0));
			switch (SpawnPoint->SpawnType)
			{
			case ESpawnerType::Edge:

				SpawnPointsEdge.Add(SpawnEdgeInfo);
				break;

			case ESpawnerType::Corner:

				SpawnPointsCorner.Add(SpawnCornerInfo);
				break;

			case ESpawnerType::Scatter:
				break;

			case ESpawnerType::SpecificLocation:
				break;
			}
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
					if (PlayerCharacter)
					{
						if (SpawnerModifier.TriggersToStartSpawning.ScoreTrigger <= PlayerCharacter->GetScore() &&
							SpawnerModifier.TriggersToStartSpawning.ScoreModifierTrigger <= PlayerCharacter->GetScoringModifier() &&
							SpawnerModifier.TriggersToStartSpawning.GameTimeDuration <= RealTimeTracker)
						{
							FTransform SpawnTransform;
							FVector RandomSpawnLocation;
							FSpawnPointsInfo EdgeSpawnZone;
							FVector CornerSpawnLocation;
							if (!SpawnerModifier.EnemySpawnLocation.IsEmpty())
							{
								const int Roll = FMath::RandRange(0, SpawnerModifier.EnemySpawnLocation.Num() - 1);
								if (SpawnerModifier.EnemySpawnLocation.IsValidIndex(Roll))
								{
									RandomSpawnLocation = SpawnerModifier.EnemySpawnLocation[Roll];
								}
							}
							if (!SpawnPointsEdge.IsEmpty())
							{
								const int Roll = FMath::RandRange(0, SpawnPointsEdge.Num() - 1);
								if (SpawnPointsEdge.IsValidIndex(Roll))
								{
									EdgeSpawnZone = SpawnPointsEdge[Roll];
								}
							}
							if(!SpawnPointsCorner.IsEmpty())
							{
								const int Roll = FMath::RandRange(0, SpawnPointsCorner.Num() - 1);
								if(SpawnPointsCorner.IsValidIndex(Roll))
								{
									CornerSpawnLocation = SpawnPointsCorner[Roll].SpawnPointLocation;
								}
							}
							switch (SpawnerModifier.SpawnType)
							{
							case ESpawnerType::Corner:

								for (int i = 0; i < SpawnerModifier.SpawnAmount; i++)
								{
									SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), FVector(CornerSpawnLocation.X + FMath::RandRange(-CornerScatterDist, CornerScatterDist), CornerSpawnLocation.Y + FMath::RandRange(-CornerScatterDist, CornerScatterDist), CornerSpawnLocation.Z), FVector(1.f, 1.f, 1.f));
									SpawnEnemies(1, SpawnerModifier.EnemyRowName, SpawnTransform);
								}
								break;

							case ESpawnerType::Edge:

								for (int i = 0; i < SpawnerModifier.SpawnAmount; i++)
								{
									SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), FVector(FMath::RandRange((EdgeSpawnZone.BoxOrigin - EdgeSpawnZone.BoxExtent).X, (EdgeSpawnZone.BoxOrigin + EdgeSpawnZone.BoxExtent).X), FMath::RandRange((EdgeSpawnZone.BoxOrigin - EdgeSpawnZone.BoxExtent).Y, (EdgeSpawnZone.BoxOrigin + EdgeSpawnZone.BoxExtent).Y), SpawnTransform.GetTranslation().Z), FVector(1.f, 1.f, 1.f));
									SpawnEnemies(1, SpawnerModifier.EnemyRowName, SpawnTransform);
								}
								break;

							case ESpawnerType::Scatter:

								if (PlayerCharacter)
								{
									for (int i = 0; i < SpawnerModifier.SpawnAmount; i++)
									{
										SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetRandomPointNearOrigin(PlayerCharacter->GetActorLocation(), ScatterMinDist, ScatterMaxDist), FVector(1.f, 1.f, 1.f));
										SpawnEnemies(1, SpawnerModifier.EnemyRowName, SpawnTransform);
									}
								}
								break;

							case ESpawnerType::SpecificLocation:

								SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), RandomSpawnLocation, FVector(1.f, 1.f, 1.f));
								SpawnEnemies(SpawnerModifier.SpawnAmount, SpawnerModifier.EnemyRowName, SpawnTransform);
								break;

							default:
								if (PlayerCharacter)
								{
									SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetRandomPointNearOrigin(PlayerCharacter->GetActorLocation(), ScatterMinDist, ScatterMaxDist), FVector(1.f, 1.f, 1.f));
									SpawnEnemies(SpawnerModifier.SpawnAmount, SpawnerModifier.EnemyRowName, SpawnTransform);
								}
								break;
							}

							SpawnerModifier.TriggersToEndSpawning.EnemiesSpawnedTracker += SpawnerModifier.SpawnAmount;
						}
						else
						{
							continue;
						}
					}
					else
					{
						PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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
void AEnemySpawner::SpawnEnemies(const int AmountToSpawn, const FName& RowName, const FTransform& SpawnTransform) const
{
	if(EnemyDataTable != nullptr)
	{
		if (const FEnemiesDataTable* ShipData = EnemyDataTable->FindRow<FEnemiesDataTable>(RowName, "Enemy DT not set up in EnemySpawner.cpp", true))
		{
			if(TSubclassOf<AEnemyCharacter> EnemyToSpawn = ShipData->EnemyClass.LoadSynchronous())
			{
				for(int i = 0; i < AmountToSpawn; i++)
				{
					if(AEnemyCharacter* EnemySpawned = GetWorld()->SpawnActorDeferred<AEnemyCharacter>(EnemyToSpawn, SpawnTransform))
					{
						for (auto Stats : ShipData->Stats)
						{
							switch (Stats.Key)
							{
							case EEnemyStatsType::Damage:

								EnemySpawned->SetDamage(Stats.Value);
								break;

							case EEnemyStatsType::AttackRange:

								EnemySpawned->SetAttackRange(Stats.Value);
								break;

							case EEnemyStatsType::CritChance:

								EnemySpawned->SetCritChance(Stats.Value);
								break;

							case EEnemyStatsType::CritDamage:

								EnemySpawned->SetCritDamage(Stats.Value);
								break;

							case EEnemyStatsType::Health:

								EnemySpawned->SetMaxHealth(Stats.Value);
								break;

							case EEnemyStatsType::MovementSpeed:

								EnemySpawned->GetCharacterMovement()->MaxWalkSpeed = Stats.Value;
								break;

							case EEnemyStatsType::Default:
								break;
							}
						}
						EnemySpawned->SetCollectableLootTable(ShipData->DropTable);
						EnemySpawned->SetExperienceStruct(ShipData->Experience);
						EnemySpawned->SetScore(ShipData->Score);
						EnemySpawned->GetBaseModel()->SetStaticMesh(ShipData->ShipMesh);

						EnemySpawned->UpdateStats(GrowthTracker / 400);
						EnemySpawned->FinishSpawning(SpawnTransform);
					}
				}
			}
		}
	}
}
