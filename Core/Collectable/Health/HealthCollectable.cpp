// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthCollectable.h"

void AHealthCollectable::Collected()
{
	Super::Collected();

	PlayerCharacter->UpdateHealth(AddedHealth);
}
