// Fill out your copyright notice in the Description page of Project Settings.


#include "SideCannonsProjectile.h"

/**
 * @brief This method is called when the game starts or when spawned.
 * 
 * If the projectile is a spawner, it will generate a number of projectiles
 * based on the `TriggerAmount` property. Each projectile will be spawned
 * at a location determined by the `SpawnLocationTracker` variable.
 * 
 * Each spawned projectile will have its properties set based on the properties
 * of the spawner projectile.
 */
void ASideCannonsProjectile::BeginPlay()
{
	Super::BeginPlay();
	if(bIsSpawner)
	{
		int SpawnLocationTracker = 0;
		for (int i = 0; i < TriggerAmount - 1; i++)
		{
			SpawnLocationTracker = (FMath::Pow(-1.f, i + 1) * (AffectRadius * 2) * ((i + 2) / 2 ));
			const FTransform BulletSpawnLocation = FTransform(this->GetActorRotation(), FVector(this->GetActorLocation().X + SpawnLocationTracker, this->GetActorLocation().Y, this->GetActorLocation().Z), FVector(1.f, 1.f, 1.f));
			if (BaseProjectile)
			{
				ASideCannonsProjectile* ProjectileSpawn = GetWorld()->SpawnActorDeferred<ASideCannonsProjectile>(BaseProjectile, BulletSpawnLocation);
				ProjectileSpawn->SetDamage(Damage);
				ProjectileSpawn->SetAffectRadius(AffectRadius);
				ProjectileSpawn->SetSpecialUpgrade1(bSpecialUpgrade1);
				ProjectileSpawn->SetSpecialUpgrade2(bSpecialUpgrade2);
				ProjectileSpawn->SetSpecialUpgrade3(bSpecialUpgrade3);
				ProjectileSpawn->SetTriggerAmount(TriggerAmount);
				ProjectileSpawn->SetDestroyDuration(DestroyDuration);
				ProjectileSpawn->SetIsSpawner(false);
				ProjectileSpawn->FinishSpawning(BulletSpawnLocation);
			}
		}
	}
}

void ASideCannonsProjectile::TraceCheck(const float& DeltaTime)
{
	Super::TraceCheck(DeltaTime);

	ApplyDamage(ActorsHit);

}
