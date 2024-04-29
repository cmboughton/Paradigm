// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons.h"

#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Weapons/WeaponUpgradeManager.h"
#include "Paradigm_IQ/Core/EnemyCharacter/EnemyCharacter.h"

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
		if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(WeaponName, "Weapons Data Table Not set up", true))
		{
			FireRate = WeaponsData->FireRate;
			Damage = WeaponsData->Damage;
			//UE_LOGFMT(LogTemp, Warning, "FireRate: {0}", FireRate);

			AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponUpgradeManager::StaticClass());
			UpgradeManagerRef = Cast<AWeaponUpgradeManager>(FoundManager);
			if(UpgradeManagerRef)
			{
				FUpgradeManager WeaponUpgrades = FUpgradeManager(this, WeaponsData->WeaponUpgrades, WeaponsData->WeaponType);
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

void AWeapons::UpgradeWeapon(FWeaponUpgrades WeaponUpgrades)
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

TArray<AActor*> AWeapons::FindClosestEnemies(const float DistanceCheck)
{
	TArray<AActor*> EnemiesFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemiesFound);
	TArray<AActor*> ClosestEnemy;
	if (!EnemiesFound.IsEmpty())
	{
		for (AActor* EnemiesDetected : EnemiesFound)
		{
			if (FMath::Sqrt(FVector::DistSquared(EnemiesDetected->GetActorLocation(), this->GetActorLocation())) <= DistanceCheck)
			{
				ClosestEnemy.Add(EnemiesDetected);
			}
		}
	}
	return ClosestEnemy;
}

AActor* AWeapons::FindClosestEnemy()
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
				if (FMath::Sqrt(FVector::DistSquared(EnemiesDetected->GetActorLocation(), this->GetActorLocation())) < FMath::Sqrt(FVector::DistSquared(ClosestEnemy->GetActorLocation(), this->GetActorLocation())))
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

void AWeapons::ApplyDamage(const FHitResult ActorHit)
{
	AActor* EnemyHit = ActorHit.GetActor();
	const FVector ActorLocation = ActorHit.GetActor()->GetActorLocation();
	const FPointDamageEvent DamageEvent(Damage, ActorHit, ActorLocation, nullptr);
	EnemyHit->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
}

TSubclassOf<AProjectile> AWeapons::SpawnProjectile(FTransform Transform)
{
	if (Projectile)
	{
		AProjectile* ProjectileSpawn = GetWorld()->SpawnActorDeferred<AProjectile>(Projectile, Transform);
		ProjectileSpawn->SetDamage(Damage);
		ProjectileSpawn->SetAffectRadius(AffectRadius);
		ProjectileSpawn->SetSpecialUpgrade1(bSpecialUpgrade1Proj);
		ProjectileSpawn->SetSpecialUpgrade2(bSpecialUpgrade2Proj);
		ProjectileSpawn->SetSpecialUpgrade3(bSpecialUpgrade3Proj);
		ProjectileSpawn->FinishSpawning(Transform);
	}
	return Projectile;
}

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
