// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UDeathScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenuButton->OnClicked.AddDynamic(this, &UDeathScreenWidget::OnButtonClicked);
}

void UDeathScreenWidget::OnButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}