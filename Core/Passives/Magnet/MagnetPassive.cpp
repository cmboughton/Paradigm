// Fill out your copyright notice in the Description page of Project Settings.


#include "MagnetPassive.h"

#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"

void AMagnetPassive::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(UpdateDelayTracker <= 0)
	{
		PlayerCharacter->SetPickUpRadius(PlayerCharacter->GetPickUpRadius() * (1 + PickUpRadiusModifier));
		UpdateDelayTracker = UpdateDelay;
	}
	else
	{
		UpdateDelayTracker -= DeltaTime;
	}
}
