// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanicEcho.h"

#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "Engine/DamageEvents.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"


void AArcanicEcho::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(CooldownTracker > 0)
	{
		CooldownTracker -= DeltaTime;
	}
	if (bWasHit)
	{
		if (CooldownTracker <= 0)
		{
			if (CurrentRadius <= AffectRadius)
			{
				CurrentRadius += (DeltaTime * GrowthModifier);
				const TArray<FHitResult> ActorsHit = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), CurrentRadius);
				for (const FHitResult& ActorHit : ActorsHit)
				{
					if (ActorHit.GetActor())
					{
						if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
						{
							const FVector ActorLocation = ActorHit.GetActor()->GetActorLocation();
							const FPointDamageEvent DamageEvent(Damage, ActorHit, ActorLocation, nullptr);
							ActorHit.GetActor()->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
						}
					}
				}
			}
			else
			{
				CooldownTracker = Cooldown;
				CurrentRadius = 0.f;
			}
		}
		else
		{
			bWasHit = false;
		}
	}
}

void AArcanicEcho::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter->SetArcanicEchoRef(this);
}
