// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipSelect.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Paradigm_IQ/UI/Menus/ShipSelectMenu.h"

void UShipSelect::NativeConstruct()
{
	Super::NativeConstruct();

	ShipName->SetText(FText::FromName(ShipText));

	ShipButton->OnClicked.AddDynamic(this, &UShipSelect::OnButtonClicked);
}

void UShipSelect::OnButtonClicked()
{
	ShipSelectRef->DisplayShip(ShipRowName);
}
