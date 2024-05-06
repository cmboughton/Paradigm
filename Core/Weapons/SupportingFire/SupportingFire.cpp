// Fill out your copyright notice in the Description page of Project Settings.


#include "SupportingFire.h"

void ASupportingFire::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	if(bSelectTargets)
	{
		EnemyLocation.Empty();
		RandomLocation.Empty();
		for (int i = 0; i < TriggerAmount; i++)
		{
			EnemiesFound = FindClosestEnemies(1000, this->GetActorLocation());
			if(!EnemiesFound.IsEmpty())
			{
				RandRoll = FMath::RandRange(0, EnemiesFound.Num() - 1);
				EnemyLocation.Add(EnemiesFound[RandRoll]->GetActorLocation());
			}
			RandomLocation.Add(GetRandomPointNearOrigin(this->GetActorLocation(), 800, 1200));
		}
		bSelectTargets = false;
	}

	for(int i = 0; i < TriggerAmount; i++)
	{
		if(EnemiesFound.IsValidIndex(i))
		{
			TArray<FHitResult> ActorsHit = SphereTrace(EnemyLocation[i], EnemyLocation[i], AffectRadius);
			ApplyDamage(ActorsHit);
			WeaponTriggerBP(EnemyLocation[i]);
		}
		else if(RandomLocation.IsValidIndex(i))
		{
			TArray<FHitResult> ActorsHit = SphereTrace(RandomLocation[i], RandomLocation[i], AffectRadius);
			ApplyDamage(ActorsHit);
			WeaponTriggerBP(RandomLocation[i]);
		}
	}

	if(ExplosionDelay <= 0)
	{
		FireRateTracker = FireRate;
		bSelectTargets = true;
		ExplosionDelay = 0.2f;
	}
	else
	{
		ExplosionDelay -= DeltaTime;
	}
}
