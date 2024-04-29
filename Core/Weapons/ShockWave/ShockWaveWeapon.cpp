// Fill out your copyright notice in the Description page of Project Settings.


#include "ShockWaveWeapon.h"

void AShockWaveWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	ShockWaveZone = AffectRadius * .35;

	if(ShockWaveRadiusTracker <= AffectRadius)
	{
		if(ShockWaveDelayTracker >= ShockWaveDelay)
		{
			if(ShockWaveRadiusTracker >= ShockWaveZone)
			{
				TArray<FHitResult> ActiveActorsHitInner	 = SphereTrace(this->GetActorLocation() - FVector(0.f, 0.f, 90.f), this->GetActorLocation() - FVector(0.f, 0.f, 90.f), ShockWaveRadiusTracker - ShockWaveZone + 200);
				TArray<FHitResult> ActiveActorsHitOuter	 = SphereTrace(this->GetActorLocation() - FVector(0.f, 0.f, 90.f), this->GetActorLocation() - FVector(0.f, 0.f, 90.f), ShockWaveRadiusTracker + 200);
				for (FHitResult ActorsHitOuter : ActiveActorsHitOuter)
				{
					if (ActorsHitOuter.GetActor())
					{
						if (ActorsHitOuter.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
						{
							bool bActorFound = false;
							for (FHitResult ActorsHitInner : ActiveActorsHitInner)
							{
								if(ActorsHitOuter.GetActor() == ActorsHitInner.GetActor())
								{
									bActorFound = true;
								}
							}
							if(!bActorFound)
							{
								ActorsHit.Add(ActorsHitOuter);
							}
						}
					}
				}
			}
			else
			{
				TArray<FHitResult> ActiveActorsHitOuter = SphereTrace(this->GetActorLocation() - FVector(0.f, 0.f, 90.f), this->GetActorLocation() - FVector(0.f, 0.f, 90.f), ShockWaveRadiusTracker + 200);
				for (FHitResult ActiveActorHit : ActiveActorsHitOuter)
				{
					if (ActiveActorHit.GetActor())
					{
						if(ActiveActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
						{
							ActorsHit.Add(ActiveActorHit);
						}
					}
				}
			}
			if (!ActorsHit.IsEmpty())
			{
				for (FHitResult ActorHit : ActorsHit)
				{
					if(ActorHit.GetActor())
					{
						//UE_LOGFMT(LogTemp, Warning, "ActorHit: {0} Damage: {1}",ActorHit.GetActor()->GetName(), Damage);
						ApplyDamage(ActorHit);
					}
				}
			}
			ActorsHit.Empty();

			if(ShockWaveDelayTracker >= ShockWaveZoneDelay)
			{
				ShockWaveDelayTracker = 0.f;
			}
			else
			{
				ShockWaveDelayTracker += DeltaTime;
			}
		}
		else
		{
			ShockWaveDelayTracker += DeltaTime;
			ShockWaveRadiusTracker += DeltaTime * 200;
		}
	}
	else
	{
		FireRateTracker = FireRate;
		ShockWaveRadiusTracker = 200.f;
	}
	//UE_LOGFMT(LogTemp, Warning, "ShockWaveTracker: {0}", ShockWaveRadiusTracker);
}
