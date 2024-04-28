// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeProjectile.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"

void AGrenadeProjectile::DestroyProjectile()
{
	TArray<FHitResult> AllActorsHit = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), AffectRadius * 10);
	for (FHitResult ActorHit : AllActorsHit)
	{
		if (ActorHit.GetActor())
		{
			if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
			{
				ApplyDamage(ActorHit);
			}
		}
	}
	Super::DestroyProjectile();
}

void AGrenadeProjectile::TraceCheck(const float DeltaTime)
{
	Super::TraceCheck(DeltaTime);

	if(!ActorsHit.IsEmpty())
	{
		DestroyProjectile();
	}
}


