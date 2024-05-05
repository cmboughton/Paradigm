// Fill out your copyright notice in the Description page of Project Settings.


#include "SupportingFire.h"

void ASupportingFire::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	for(int i = 0; i < TriggerAmount; i++)
	{
		TArray<AActor*> EnemiesFound = FindClosestEnemies(1000, this->GetActorLocation());
		if(!EnemiesFound.IsEmpty())
		{
			const int RandRoll = FMath::RandRange(0, EnemiesFound.Num() - 1);
			TArray<FHitResult> ActorsHit = SphereTrace(EnemiesFound[RandRoll]->GetActorLocation(), EnemiesFound[RandRoll]->GetActorLocation(), AffectRadius);
			ApplyDamage(ActorsHit);
			WeaponTriggerBP(EnemiesFound[RandRoll]->GetActorLocation());
		}
		else
		{
			const FVector RandomLocation = GetRandomPointNearOrigin(this->GetActorLocation(), 800, 1200);
			TArray<FHitResult> ActorsHit = SphereTrace(RandomLocation, RandomLocation, AffectRadius);
			ApplyDamage(ActorsHit);
			WeaponTriggerBP(RandomLocation);
		}
	}
	FireRateTracker = FireRate;
}
