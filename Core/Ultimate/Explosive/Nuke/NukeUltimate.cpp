// Fill out your copyright notice in the Description page of Project Settings.


#include "NukeUltimate.h"

void ANukeUltimate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Explosion(DeltaTime, true, true, AffectGrowthModifier, AffectRadius);
}
