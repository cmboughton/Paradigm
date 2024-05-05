// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons.h"

#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Weapons/WeaponUpgradeManager.h"
#include "Paradigm_IQ/Core/Character/EnemyCharacter/EnemyCharacter.h"

struct FWeaponsDataTable;

// Sets default values
AWeapons::AWeapons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapons::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (const UDataTable* WeaponsDataTableHardRef = WeaponsDataTable.LoadSynchronous())
	{
		if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(RowName, "Weapons Data Table Not set up. Weapons.cpp", true))
		{
			FireRate = WeaponsData->FireRate;
			Damage = WeaponsData->Damage;
			//UE_LOGFMT(LogTemp, Warning, "FireRate: {0}", FireRate);

			AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponUpgradeManager::StaticClass());
			UpgradeManagerRef = Cast<AWeaponUpgradeManager>(FoundManager);
			if(UpgradeManagerRef)
			{
				FUpgradeManager WeaponUpgrades = FUpgradeManager(this, WeaponsData->WeaponUpgrades, 0, WeaponsData->WeaponType, false);
				UpgradeManagerRef->AddUpgrades(WeaponUpgrades);
			}
		}
	}

}


void AWeapons::WeaponTriggered(const float DeltaTime)
{
}

// Called every frame
void AWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bShouldUseBulletDelay)
	{
		if (FireRateTracker <= 0)
		{
			if (BulletDelayTracker <= 0)
			{
				WeaponTriggered(DeltaTime);
			}
			else
			{
				BulletDelayTracker -= DeltaTime;
			}
		}	
		else
		{
			FireRateTracker -= DeltaTime;
		}
	}
	else
	{
		if (FireRateTracker <= 0)
		{
			WeaponTriggered(DeltaTime);
			///FireRateTracker = FireRate;
		}
		else
		{
			FireRateTracker -= DeltaTime;
		}
	}
}

void AWeapons::UpgradeWeapon(const FWeaponUpgrades& WeaponUpgrades)
{
	switch (WeaponUpgrades.WeaponUpgrade)
	{
	case EWeaponUpgradeType::PercentDamage:

		Damage *= (1 + WeaponUpgrades.WeaponUpgradeValue);
		break;

	case EWeaponUpgradeType::PercentAffectRadius:

		AffectRadius *= (1 + WeaponUpgrades.WeaponUpgradeValue);
		break;

	case EWeaponUpgradeType::PercentFireRate:

		FireRate /= (1 + WeaponUpgrades.WeaponUpgradeValue);
		break;

	case EWeaponUpgradeType::TriggerAmount:

		TriggerAmount += WeaponUpgrades.WeaponUpgradeValue;
		break;

	case EWeaponUpgradeType::SpecialUpgrade1:

		SpecialUpgrade1();
		break;

	case EWeaponUpgradeType::SpecialUpgrade2:

		SpecialUpgrade2();
		break;

	case EWeaponUpgradeType::SpecialUpgrade3:

		SpecialUpgrade3();
		break;
	}
}

TArray<AActor*> AWeapons::FindClosestEnemies(const float DistanceCheck, const FVector& Origin) const
{
	TArray<AActor*> EnemiesFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemiesFound);
	TArray<AActor*> ClosestEnemy;
	if (!EnemiesFound.IsEmpty())
	{
		for (AActor* EnemiesDetected : EnemiesFound)
		{
			if (FMath::Sqrt(FVector::DistSquared(EnemiesDetected->GetActorLocation(),Origin)) <= DistanceCheck)
			{
				ClosestEnemy.Add(EnemiesDetected);
			}
		}
	}
	return ClosestEnemy;
}

AActor* AWeapons::FindClosestEnemy(const FVector& Origin) const
{
	TArray<AActor*> EnemiesFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemiesFound);
	AActor* ClosestEnemy = nullptr;
	if (!EnemiesFound.IsEmpty())
	{
		for (AActor* EnemiesDetected : EnemiesFound)
		{
			if(ClosestEnemy)
			{
				if (FMath::Sqrt(FVector::DistSquared(EnemiesDetected->GetActorLocation(), Origin)) < FMath::Sqrt(FVector::DistSquared(ClosestEnemy->GetActorLocation(), Origin)))
				{
					ClosestEnemy = EnemiesDetected;
				}
			}
			else
			{
				ClosestEnemy = EnemiesDetected;
			}
		}
	}
	return ClosestEnemy;
}

TArray<FHitResult> AWeapons::LineTrace(const FVector ActorStartLocation, const FVector ActorEndLocation)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Init(PlayerCharacter, 1);
	TArray<FHitResult> AllActorsHit;
	UKismetSystemLibrary::LineTraceMulti(	GetWorld(), 
											ActorStartLocation,
											ActorEndLocation,
											ETraceTypeQuery::TraceTypeQuery1,
											false, 
											ActorsToIgnore, 
											EDrawDebugTrace::ForOneFrame,
											AllActorsHit,
											true, 
											FLinearColor::Red,
											FLinearColor::Green,
											5.f);
	return AllActorsHit;
}

TArray<FHitResult> AWeapons::SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Init(PlayerCharacter, 1);
	TArray<FHitResult> AllActorsHit;
	UKismetSystemLibrary::SphereTraceMulti(	GetWorld(),
											ActorStartLocation,
											ActorEndLocation,
											TraceRadius,
											ETraceTypeQuery::TraceTypeQuery1,
											false,
											ActorsToIgnore,
											EDrawDebugTrace::ForOneFrame,
											AllActorsHit,
											true,
											FLinearColor::Red,
											FLinearColor::Green,
											5.f);
	return TArray<FHitResult>(AllActorsHit);
}

void AWeapons::ApplyDamage(const TArray<FHitResult>& AllActorsHit)
{
	if(!AllActorsHit.IsEmpty())
	{
		for (FHitResult ActorHit : AllActorsHit)
		{
			if (ActorHit.GetActor())
			{
				if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
				{
					AActor* EnemyHit = ActorHit.GetActor();
					const FVector ActorLocation = ActorHit.GetActor()->GetActorLocation();
					const FPointDamageEvent DamageEvent(Damage, ActorHit, ActorLocation, nullptr);
					EnemyHit->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
				}
			}
		}
	}
}

#pragma region Spawn Projectile
/**
 * @brief Spawns a projectile in the game world.
 *
 * This method creates a projectile of the type specified by the `Projectile` class member.
 * The projectile is spawned at the location and orientation specified by the `Transform` parameter.
 * The properties of the projectile (damage, effect radius, special upgrades, trigger amount) are set according to the corresponding properties of the weapon.
 * The projectile is then added to the game world and begins its lifecycle.
 *
 * @param Transform The location and orientation at which to spawn the projectile.
 * @return The class of the spawned projectile.
 */
TSubclassOf<AProjectile> AWeapons::SpawnProjectile(const FTransform& Transform) const
{
	if (Projectile)
	{
		AProjectile* ProjectileSpawn = GetWorld()->SpawnActorDeferred<AProjectile>(Projectile, Transform);
		ProjectileSpawn->SetDamage(Damage);
		ProjectileSpawn->SetAffectRadius(AffectRadius);
		ProjectileSpawn->SetSpecialUpgrade1(bSpecialUpgrade1Proj);
		ProjectileSpawn->SetSpecialUpgrade2(bSpecialUpgrade2Proj);
		ProjectileSpawn->SetSpecialUpgrade3(bSpecialUpgrade3Proj);
		ProjectileSpawn->SetTriggerAmount(TriggerAmount);
		ProjectileSpawn->FinishSpawning(Transform);
	}
	return Projectile;
}
#pragma endregion

void AWeapons::SpecialUpgrade1()
{
	bSpecialUpgrade1Proj = true;
}

void AWeapons::SpecialUpgrade2()
{
	bSpecialUpgrade2Proj = true;
}

void AWeapons::SpecialUpgrade3()
{
	bSpecialUpgrade3Proj = true;
}

#pragma region Get Random Point Near Origin
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
FVector AWeapons::GetRandomPointNearOrigin(const FVector& Origin, const float MinDistance, const float MaxDistance)
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
#pragma endregion
