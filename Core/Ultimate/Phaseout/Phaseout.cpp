// Fill out your copyright notice in the Description page of Project Settings.


#include "Phaseout.h"

#include "GameFramework/CharacterMovementComponent.h"

void APhaseout::UltimateAbilityStart()
{
	Super::UltimateAbilityStart();

	PreviousMoveSpeed = PlayerCharacter->GetCharacterMovement()->GetMaxSpeed();
	PlayerCharacter->UpdateMovementSpeed(PlayerCharacter->GetCharacterMovement()->GetMaxSpeed() * (1 + MovementSpeedIncPercent));
	PlayerCharacter->SetInvulnerability(InvulnerabilityDuration);
	PlayerCharacter->AddThrusters(ThrusterNiagaraSystem);
}

void APhaseout::UltimateAbilityFinish()
{
	PlayerCharacter->UpdateMovementSpeed(PreviousMoveSpeed);

	if (const UDataTable* ShipDataTableHardRef = PlayerCharacter->GetShipDataTable().LoadSynchronous())
	{
		if (const FShipsDataTable* ShipData = ShipDataTableHardRef->FindRow<FShipsDataTable>(PlayerCharacter->GetSelectedShipName(), "Ships Data Table Not set up", true))
		{
			PlayerCharacter->AddThrusters(ShipData->ThrusterNiagaraSystem);
		}
	}

	Super::UltimateAbilityFinish();
}
