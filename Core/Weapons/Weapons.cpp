// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons.h"

#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Weapons/WeaponUpgradeManager.h"
#include "Paradigm_IQ/Core/Character/EnemyCharacter/EnemyCharacter.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "Engine/DamageEvents.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"

struct FWeaponsDataTable;

AWeapons::AWeapons()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapons::BeginPlay()
{
	Super::BeginPlay();

	if (const UDataTable* WeaponsDataTableHardRef = WeaponsDataTable.LoadSynchronous())
	{
		if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(RowName, "Weapons Data Table Not set up. Weapons.cpp", true))
		{
			FireRate = WeaponsData->FireRate;
			Damage = WeaponsData->Damage;
			if (!WeaponsData->DeBugUpgrade.IsEmpty())
			{
				for (const auto& DeBugUpgrades : WeaponsData->DeBugUpgrade)
				{
					const FWeaponUpgrades DeBugWeaponUpgrade = FWeaponUpgrades("Name", "Des", 0, EUpgradeRarity::Basic, DeBugUpgrades, 0.f, false, false);
					UpgradeWeapon(DeBugWeaponUpgrade);
				}
			}
			AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponUpgradeManager::StaticClass());
			UpgradeManagerRef = Cast<AWeaponUpgradeManager>(FoundManager);
			if(UpgradeManagerRef)
			{
				const FUpgradeManager WeaponUpgrades = FUpgradeManager(this, WeaponsData->WeaponUpgrades, WeaponsData->SpecialUpgradeLevels, WeaponsData->WeaponType, false, false, false);
				const FUpgradeManager SpecialWeaponUpgrades = FUpgradeManager(this, WeaponsData->SpecialWeaponUpgrades, WeaponsData->SpecialUpgradeLevels, WeaponsData->WeaponType,  false, false, true);
				UpgradeManagerRef->AddUpgrades(WeaponUpgrades, SpecialWeaponUpgrades);
				UE_LOGFMT(LogTemp, Warning, "Manager Found");
			}
		}
	}

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}


void AWeapons::WeaponTriggered(const float DeltaTime)
{
}


void AWeapons::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetCharacterState() == ECharacterState::Normal)
		{
			if (bShouldUseBulletDelay)
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
				}
				else
				{
					FireRateTracker -= DeltaTime;
				}
			}
		}
	}
	else
	{
		PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}

/**
 * @brief Upgrades the weapon based on the provided upgrade parameters.
 *
 * This function applies the upgrade specified in the `WeaponUpgrades` parameter to the weapon. The upgrade can be of various types.
 *
 * @param WeaponUpgrades A structure containing the details of the weapon upgrade to be applied.
 */
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

		bSpecialUpgrade1 = true;
		SpecialUpgradeTracker++;
		break;

	case EWeaponUpgradeType::SpecialUpgrade2:

		bSpecialUpgrade2 = true;
		SpecialUpgradeTracker++;
		break;

	case EWeaponUpgradeType::SpecialUpgrade3:

		bSpecialUpgrade3 = true;
		SpecialUpgradeTracker++;
		break;

	case EWeaponUpgradeType::SpecialUpgrade4:

		bSpecialUpgrade4 = true;
		SpecialUpgradeTracker++;
		break;

	case EWeaponUpgradeType::SpecialUpgrade5:

		bSpecialUpgrade5 = true;
		SpecialUpgradeTracker++;
		break;
	}
}

/**
 * @brief Finds the closest enemies within a specified distance from a given origin.
 *
 * @param DistanceCheck The maximum distance within which to find enemies.
 * @param Origin The origin point from which to measure the distance.
 *
 * @return Returns an array of actors representing the enemies found within the specified distance.
 *
 * This function first retrieves all actors of the AEnemyCharacter class. It then iterates over these actors,
 * calculating the distance from the origin to each actor's location. If this distance is less than or equal to
 * the specified DistanceCheck, the actor is added to the array of closest enemies. The function finally returns
 * this array.
 */
TArray<AActor*> AWeapons::FindClosestEnemies(const float& DistanceCheck, const FVector& Origin) const
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

/**
 * @brief Finds the closest enemy to a given origin point.
 *
 * This method searches for all actors of the AEnemyCharacter class in the world and determines the closest one to the provided origin point.
 * The distance is calculated using the Euclidean distance formula (square root of the sum of squared differences).
 *
 * @param Origin The origin point from which to find the closest enemy.
 * @return A pointer to the closest enemy actor. If no enemies are found, returns nullptr.
 */
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

/**
 * @brief Performs a line trace from the start location to the end location and returns all actors hit.
 * 
 * This function performs a line trace in the world and returns an array of all actors hit. 
 * The line trace starts from the ActorStartLocation and ends at the ActorEndLocation. 
 * The trace ignores the PlayerCharacter.
 * 
 * @param ActorStartLocation The start location of the line trace.
 * @param ActorEndLocation The end location of the line trace.
 * @return TArray<FHitResult> An array of all actors hit by the line trace.
 */
TArray<FHitResult> AWeapons::LineTrace(const FVector& ActorStartLocation, const FVector& ActorEndLocation)
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

/**
 * @brief Performs a sphere trace from a start location to an end location with a specified radius.
 *
 * This method performs a sphere trace in the world, starting from the ActorStartLocation and ending at the ActorEndLocation.
 * The trace has a radius specified by TraceRadius. The trace ignores the PlayerCharacter.
 * The method returns an array of FHitResult, containing all actors hit by the trace.
 *
 * @param ActorStartLocation The start location of the trace.
 * @param ActorEndLocation The end location of the trace.
 * @param TraceRadius The radius of the trace.
 * @return TArray<FHitResult> An array of FHitResult, containing all actors hit by the trace.
 */
TArray<FHitResult> AWeapons::SphereTrace(const FVector& ActorStartLocation, const FVector& ActorEndLocation, const float& TraceRadius)
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

/**
 * @brief Applies damage to all actors hit.
 *
 * This method iterates over all actors hit and applies damage to those that implement the UEnemyInterface.
 * The damage is applied in the form of a FPointDamageEvent, which is created for each actor hit.
 *
 * @param AllActorsHit A TArray of FHitResult representing all the actors hit.
 */
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
					const float UpdatedDamage = UpdateDamage();
					const FPointDamageEvent DamageEvent(UpdatedDamage, ActorHit, ActorLocation, nullptr);
					EnemyHit->TakeDamage(UpdatedDamage, DamageEvent, GetInstigatorController(), this);
				}
			}
		}
	}
}

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
		ProjectileSpawn->SetSpecialUpgrade1(bSpecialUpgrade1);
		ProjectileSpawn->SetSpecialUpgrade2(bSpecialUpgrade2);
		ProjectileSpawn->SetSpecialUpgrade3(bSpecialUpgrade3);
		ProjectileSpawn->SetSpecialUpgrade4(bSpecialUpgrade4);
		ProjectileSpawn->SetSpecialUpgrade5(bSpecialUpgrade5);
		ProjectileSpawn->SetTriggerAmount(TriggerAmount);
		ProjectileSpawn->SetPlayerCharacter(PlayerCharacter);
		ProjectileSpawn->FinishSpawning(Transform);
		return Projectile;
	}
	return nullptr;
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
	return RandomPoint;
}

float AWeapons::UpdateDamage()
{
	float NewDamage = Damage;

	NewDamage *= DamageModifier;
	return NewDamage;
}
