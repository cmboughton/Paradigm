// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialCascadeWeapon.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Character/EnemyCharacter/EnemyCharacter.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"

void ACelestialCascadeWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	AActor* CurrentEnemy = this;
	EnemiesToDamage.Empty();
	for (int i = 0; i < TriggerAmount; i++)
	{
		if(AActor* EnemyFound = FindClosestEnemy(CurrentEnemy->GetActorLocation()))
		{
			if (FMath::Sqrt(FVector::DistSquared(CurrentEnemy->GetActorLocation(), EnemyFound->GetActorLocation())) <= 600)
			{
				CurrentEnemy = EnemyFound;
				EnemiesToDamage.Add(EnemyFound);
			}
		}
	}

	if (!EnemiesToDamage.IsEmpty())
	{
		FVector StartLoc = this->GetActorLocation();
		for (AActor* EnemyHit : EnemiesToDamage)
		{
			if (EnemyHit)
			{
				if (EnemyHit->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
				{
					const FVector ActorLocation = EnemyHit->GetActorLocation();
					const FHitResult HitResult;
					const FPointDamageEvent DamageEvent(Damage, HitResult, ActorLocation, nullptr);
					EnemyHit->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
					if (LightningSystem)
					{
						UNiagaraComponent* LightningSystemSpawned = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LightningSystem, FVector(0, 0, 0), FRotator(0, 0, 0), FVector(0, 0, 0), true, true, ENCPoolMethod::None, true);
						LightningSystemSpawned->SetVariableVec3("BeamStartPoint", StartLoc);
						LightningSystemSpawned->SetVariableVec3("BeamEndPoint", ActorLocation);
						UE_LOGFMT(LogTemp, Warning, "StartPoint: {0}, {1}, {2} EndPoint {3}, {4}, {5}", StartLoc.X, StartLoc.Y, StartLoc.Z, ActorLocation.X, ActorLocation.Y, ActorLocation.Z);
					}
				}
			}
			
			FVector EndLoc = EnemyHit->GetActorLocation();
			SpawnLightingBP(StartLoc, EndLoc);
			//SphereTrace(StartLoc, EndLoc, 20);
			StartLoc = EnemyHit->GetActorLocation();
		}
	}

	FireRateTracker = FireRate;
}

AActor* ACelestialCascadeWeapon::FindClosestEnemy(const FVector& Origin) const
{
	TArray<AActor*> EnemiesFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemiesFound);
	AActor* ClosestEnemy = nullptr;
	if (!EnemiesFound.IsEmpty())
	{
		for (const AActor* EnemyToDamage: EnemiesToDamage)
		{
			if(EnemyToDamage)
			{
				EnemiesFound.RemoveAllSwap([&](const AActor* EnemyToRemove) { return EnemyToRemove == EnemyToDamage; });
			}
		}
		for (AActor* EnemiesDetected : EnemiesFound)
		{
			if (ClosestEnemy)
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
