// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"

void ABaseProjectile::TraceCheck(const float& DeltaTime)
{
	Super::TraceCheck(DeltaTime);
	if (bSpecialUpgrade2)
	{
		if(!ActorsHit.IsEmpty())
		{
			bool bEnemyHit = false;
			for (FHitResult ActorHit : ActorsHit)
			{
				if (ActorHit.GetActor())
				{
					if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
					{
						bEnemyHit = true;
					}
				}
			}
			if(bEnemyHit)
			{
				const TArray<FHitResult> ActorHit = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), AffectRadius * 5);
				ApplyDamage(ActorHit);
				DestroyProjectile();
			}
		}
	}
	else
	{
		ApplyDamage(ActorsHit);
	}
}

void ABaseProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bSpecialUpgrade1)
	{
		EndLocation = this->GetActorLocation() - FVector(0.f, 0.f, 90.f);
		const TArray<FHitResult> TrailActorsHit = SphereTrace(StartLocation, EndLocation, AffectRadius * 2);
		ApplyDamage(TrailActorsHit);
	}
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = this->GetActorLocation() - FVector(0.f, 0.f, 90.f);
}

/**
 * @brief Destroys the projectile and spawns a Flame Trail Upgrade if the special upgrade 1 is active.
 * 
 * This method is responsible for handling the destruction of the projectile. If the special upgrade 1 is active, 
 * it spawns a Flame Trail Upgrade at the current actor's transform, sets its properties, and finishes its spawning.
 * After handling the special upgrade, it calls the parent class's DestroyProjectile method.
 */
void ABaseProjectile::DestroyProjectile()
{
	if(bSpecialUpgrade1)
	{
		if (FlameTrailUpgrade)
		{
			AFlameTrailUpgrade* FlameTrailSpawn = GetWorld()->SpawnActorDeferred<AFlameTrailUpgrade>(FlameTrailUpgrade, this->GetActorTransform());
			FlameTrailSpawn->SetAffectRadius(AffectRadius * 2);
			FlameTrailSpawn->SetDamage(Damage);
			FlameTrailSpawn->SetEndLocation(EndLocation);
			FlameTrailSpawn->SetStartLocation(StartLocation);
			FlameTrailSpawn->FinishSpawning(this->GetActorTransform());
		}
	}
	Super::DestroyProjectile();
}
