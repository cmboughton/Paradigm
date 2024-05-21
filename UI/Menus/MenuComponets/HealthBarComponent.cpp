// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

void UHealthBarComponent::UpdateHealthBar(const float& CurrentHealth, const float& MaxHealth)
{
	HealthPB->SetPercent(UKismetMathLibrary::NormalizeToRange(CurrentHealth, 0, MaxHealth));
}
