// Fill out your copyright notice in the Description page of Project Settings.


#include "Experience.h"

#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"

void AExperience::Collected()
{
	struct FExperienceOrb ExperienceStruct;
	ExperienceStruct.Experience = Experience;
	ExperienceStruct.UltimateExperience = UltimateExperience;
	if (PlayerCharacter)
	{
		Super::Collected();
		PlayerCharacter->AddCollectable(ExperienceStruct);
	}
	else
	{
		PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Collected();
	}
}

void AExperience::SetUp(const FExperienceOrb& ExperienceStruct)
{
	Experience = ExperienceStruct.Experience;
	UltimateExperience = ExperienceStruct.UltimateExperience;
}
