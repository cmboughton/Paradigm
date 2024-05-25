// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtraLife.h"

#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"

void AExtraLife::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter->SetHasExtraLife(true);
}
