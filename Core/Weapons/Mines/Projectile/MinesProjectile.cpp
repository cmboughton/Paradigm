// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesProjectile.h"

void AMinesProjectile::TraceCheck(const float DeltaTime)
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
