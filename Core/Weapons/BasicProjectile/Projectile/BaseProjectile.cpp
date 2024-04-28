// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"

void ABaseProjectile::TraceCheck(const float DeltaTime)
{
	Super::TraceCheck(DeltaTime);
	if (!ActorsHit.IsEmpty())
	{
		for (FHitResult ActorHit : ActorsHit)
		{
			if(ActorHit.GetActor())
			{
				if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
				{
					//UE_LOGFMT(LogTemp, Warning, "Actor Hit: {0}", ActorHit.GetActor()->GetName());
					ApplyDamage(ActorHit);
				}
			}
		}
	}
}