// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipStatsComponent.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UShipStatsComponent::NativeConstruct()
{
	Super::NativeConstruct();

	StatText->SetText(StatName);
	StatPB->SetPercent(StatValuePB);
}
