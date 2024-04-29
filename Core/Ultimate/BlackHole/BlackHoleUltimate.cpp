// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleUltimate.h"

#include "Paradigm_IQ/Core/EnemyCharacter/EnemyCharacter.h"

void ABlackHoleUltimate::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TArray<FHitResult> ActorsHit = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), AffectRadius);
	if(PullDurationTracker <= 0)
	{
		if (PullDurationDelay <= 0)
		{
			PullDurationDelay = 0.5f;
			PullDurationTracker = Duration / 30;
		}
		else
		{
			PullDurationDelay -= DeltaSeconds;
		}
		if(!EnemiesPulled.IsEmpty())
		{
			for (const AEnemyCharacter* EnemyPulled : EnemiesPulled)
			{
				if (EnemyPulled)
				{
					EnemyPulled->BaseModel->SetSimulatePhysics(false);
					//UE_LOGFMT(LogTemp, Warning, "Actor Reset: {0}", EnemyPulled->GetName());
				}
			}
			EnemiesPulled.Empty();
		}
	}
	else
	{
		for(FHitResult ActorHit : ActorsHit)
		{
			if (ActorHit.GetActor())
			{
				if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
				{
					FVector PullDirection = (this->GetActorLocation() - ActorHit.GetActor()->GetActorLocation() * 1 + ((Duration / 30) - PullDurationTracker));

					if(AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(ActorHit.GetActor()))
					{
						Enemy->BaseModel->SetSimulatePhysics(true);
						Enemy->BaseModel->AddForce(PullDirection * GravitationalForce);
						EnemiesPulled.AddUnique(Enemy);
					}
				}
			}
		}
		PullDurationTracker -= DeltaSeconds;
	}

	TArray<FHitResult> ActorsToDamage = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), AffectRadius / 3);
	for (FHitResult ActorToDamage : ActorsToDamage)
	{
		if (ActorToDamage.GetActor())
		{
			if (ActorToDamage.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
			{
				ApplyDamage(ActorToDamage);
			}
		}
	}
}
