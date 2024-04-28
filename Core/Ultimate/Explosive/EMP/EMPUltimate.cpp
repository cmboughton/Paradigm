// Fill out your copyright notice in the Description page of Project Settings.


#include "EMPUltimate.h"

void AEMPUltimate::UltimateAbilityStart()
{
	Super::UltimateAbilityStart();
	Explosion(0.f, true, false, AffectGrowthModifier, AffectRadius);
	UltimateAbilityFinish();
}