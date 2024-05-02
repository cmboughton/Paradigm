// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"

void ABaseProjectile::TraceCheck(const float DeltaTime)
{
	Super::TraceCheck(DeltaTime);
	ApplyDamage(ActorsHit);

}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bSpecialUpgrade1)
	{
		EndLocation = this->GetActorLocation() - FVector(0.f, 0.f, 90.f);
		TArray<FHitResult> TrailActorsHit = SphereTrace(StartLocation, EndLocation, AffectRadius * 2);
		ApplyDamage(TrailActorsHit);
	}
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = this->GetActorLocation() - FVector(0.f, 0.f, 90.f);
}

void ABaseProjectile::DestroyProjectile()
{
	if(bSpecialUpgrade1)
	{
		if (FlameTrailUpgrade)
		{
			AFlameTrailUpgrade* FlameTrailSpawn = GetWorld()->SpawnActorDeferred<AFlameTrailUpgrade>(FlameTrailUpgrade, this->GetActorTransform());
			FlameTrailSpawn->SetAffectRadius(AffectRadius * 2);
			FlameTrailSpawn->SetDamage(Damage);
			FlameTrailSpawn->SetEndLocation(EndLocation);
			FlameTrailSpawn->SetStartLocation(StartLocation);
			FlameTrailSpawn->FinishSpawning(this->GetActorTransform());
		}
	}

	Super::DestroyProjectile();
}
