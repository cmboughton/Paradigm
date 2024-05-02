// Fill out your copyright notice in the Description page of Project Settings.


#include "RailGunProjectile.h"

void ARailGunProjectile::TraceCheck(const float DeltaTime)
{
	Super::TraceCheck(DeltaTime);

	ApplyDamage(ActorsHit);
}
