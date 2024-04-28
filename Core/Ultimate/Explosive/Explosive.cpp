// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Logging/StructuredLog.h"


void AExplosive::Explosion(const float DeltaTime, const bool bShouldExplode, const bool ShouldGrow, const float GrowthModifier, const float Radius)
{
	if(bShouldExplode)
	{
		if(ShouldGrow)
		{
			CurrentRadius += (DeltaTime * GrowthModifier);
			if(CurrentRadius <= Radius)
			{
				TArray<FHitResult> ActorsHit;
				ActorsHit.Empty();
				ActorsHit = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), CurrentRadius);
				UE_LOGFMT(LogTemp, Warning, "ExplosionRadius: {0}", CurrentRadius);

				for(const FHitResult ActorHit : ActorsHit)
				{
					if (ActorHit.GetActor())
					{
						if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
						{
							IEnemyInterface::Execute_ChangeCharacterState(ActorHit.GetActor(), EnemyState);
							//UE_LOGFMT(LogTemp, Warning, "HasInterface: {0}", ActorHit.GetActor()->GetName());
						}
					}
				}
			}
			else
			{
				UltimateAbilityFinish();
			}
		}
		else
		{
			TArray<FHitResult> ActorsHit;
			ActorsHit.Empty();
			ActorsHit = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), Radius);
			UE_LOGFMT(LogTemp, Warning, "ExplosionRadius: {0}", Radius);

			for (const FHitResult ActorHit : ActorsHit)
			{
				if (ActorHit.GetActor())
				{
					if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
					{
						IEnemyInterface::Execute_ChangeCharacterState(ActorHit.GetActor(), EnemyState);
						//UE_LOGFMT(LogTemp, Warning, "HasInterface: {0}", ActorHit.GetActor()->GetName());
					}
				}
			}
		}
	}

}
