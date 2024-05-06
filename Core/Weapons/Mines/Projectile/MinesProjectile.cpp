// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesProjectile.h"

/**
 * @brief Performs a trace check for the projectile.
 *
 * This method is responsible for checking if the projectile has hit any actors. If the projectile is set to check for collision and has hit any actors, it stops simulating physics, doubles its affect radius, and stops checking for collision. If the projectile is not set to check for collision and has hit any actors, it checks if the hit actors implement the UEnemyInterface. If they do, it performs a sphere trace around the projectile's location with a radius four times the affect radius, applies damage to the actors hit by the sphere trace, and sets the projectile to be ready for destruction. If the projectile is ready for destruction, it destroys the projectile.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void AMinesProjectile::TraceCheck(const float& DeltaTime)
{
	Super::TraceCheck(DeltaTime);

	if(bCheckForCollision)
	{
		if (!ActorsHit.IsEmpty())
		{
			BulletMesh->SetSimulatePhysics(false);
			AffectRadius = AffectRadius * 2;
			bCheckForCollision = false;
		}
	}
	else
	{
		if(!ActorsHit.IsEmpty())
		{
			for (FHitResult ActorHit : ActorsHit)
			{
				if (ActorHit.GetActor())
				{
					if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
					{
						TArray<FHitResult> ActorsToDamage = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), AffectRadius * 4);
						ApplyDamage(ActorsToDamage);
						bReadyToDestroy = true;
					}
				}
			}
		}
	}
	if(bReadyToDestroy)
	{
		DestroyProjectile();
	}
}
