// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "ShipSelectMenu.h"
#include "Components/Button.h"
#include "Logging/StructuredLog.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	WBShipSelectMenu->SetVisibility(ESlateVisibility::Collapsed);

	StartButton->OnHovered.AddDynamic(this, &UMainMenu::OnButtonClicked);
}

void UMainMenu::OnButtonClicked()
{
	WBShipSelectMenu->SetVisibility(ESlateVisibility::Visible);
	StartButton->SetVisibility(ESlateVisibility::Collapsed);
}
