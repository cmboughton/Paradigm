// Fill out your copyright notice in the Description page of Project Settings.


#include "EMPUltimate.h"

void AEMPUltimate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Explosion(DeltaTime, true, false, ExplosiveGrowthModifier, ExplosiveRadius, ECharacterState::Stunned);
}