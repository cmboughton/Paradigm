// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthCollectable.h"

#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"

void AHealthCollectable::Collected()
{
	if(PlayerCharacter)
	{
		Super::Collected();
		PlayerCharacter->UpdateHealth(AddedHealth);
	}
	else
	{
		PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Collected();
	}
}
