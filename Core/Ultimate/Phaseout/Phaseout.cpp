// Fill out your copyright notice in the Description page of Project Settings.


#include "Phaseout.h"

#include "GameFramework/CharacterMovementComponent.h"

void APhaseout::UltimateAbilityStart()
{
	Super::UltimateAbilityStart();

	PreviousMoveSpeed = PlayerCharacter->GetCharacterMovement()->GetMaxSpeed();
	PlayerCharacter->UpdateMovementSpeed(PlayerCharacter->GetCharacterMovement()->GetMaxSpeed() * (1 + MovementSpeedIncPercent));
	PlayerCharacter->SetInvulnerability(InvulnerabilityDuration);
}

void APhaseout::UltimateAbilityFinish()
{
	PlayerCharacter->UpdateMovementSpeed(PreviousMoveSpeed);

	Super::UltimateAbilityFinish();
}
