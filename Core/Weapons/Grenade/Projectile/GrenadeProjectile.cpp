// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeProjectile.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"

void AGrenadeProjectile::DestroyProjectile()
{
	TArray<FHitResult> AllActorsHit = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), AffectRadius * 10);
	ApplyDamage(AllActorsHit);
	if (bSpecialUpgrade1)
	{
		for(int i = 0; i < BouncingBettySpawn; i++)
		{
			if (GrenadeProjectile)
			{
				const FTransform SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), this->GetActorLocation() + FVector(0.f, 0.f, 100.f), FVector(1.f, 1.f, 1.f));
				AGrenadeProjectile* GrenadeSpawn = GetWorld()->SpawnActorDeferred<AGrenadeProjectile>(GrenadeProjectile, SpawnTransform);
				GrenadeSpawn->SetIsArcImpulse(true);
				GrenadeSpawn->SetImpulse(60.f);
				GrenadeSpawn->SetAffectRadius(AffectRadius / 2);
				GrenadeSpawn->SetDamage(Damage / 2);
				GrenadeSpawn->FinishSpawning(SpawnTransform);
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


