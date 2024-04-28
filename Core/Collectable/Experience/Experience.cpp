// Fill out your copyright notice in the Description page of Project Settings.


#include "Experience.h"

#include "Paradigm_IQ/Core/Data/Structs/Structs.h"

void AExperience::Collected(APlayerCharacter* PlayerCharacterRef)
{
	Super::Collected(PlayerCharacterRef);
	struct FExperienceOrb ExperienceStruct;
	ExperienceStruct.Experience = Experience;
	ExperienceStruct.UltimateExperience = UltimateExperience;
	PlayerCharacterRef->AddCollectable(ExperienceStruct);
}

void AExperience::SetUp(FExperienceOrb ExperienceStruct)
{
	Experience = ExperienceStruct.Experience;
	UltimateExperience = ExperienceStruct.UltimateExperience;
}
