// Fill out your copyright notice in the Description page of Project Settings.


#include "Experience.h"

#include "Paradigm_IQ/Core/Data/Structs/Structs.h"

void AExperience::Collected()
{
	Super::Collected();
	struct FExperienceOrb ExperienceStruct;
	ExperienceStruct.Experience = Experience;
	ExperienceStruct.UltimateExperience = UltimateExperience;
	PlayerCharacter->AddCollectable(ExperienceStruct);
}

void AExperience::SetUp(const FExperienceOrb& ExperienceStruct)
{
	Experience = ExperienceStruct.Experience;
	UltimateExperience = ExperienceStruct.UltimateExperience;
}
