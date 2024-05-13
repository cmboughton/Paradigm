// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneLaserProjectile.h"

void ADroneLaserProjectile::TraceCheck(const float& DeltaTime)
{
	Super::TraceCheck(DeltaTime);
	ApplyDamage(ActorsHit);
}
