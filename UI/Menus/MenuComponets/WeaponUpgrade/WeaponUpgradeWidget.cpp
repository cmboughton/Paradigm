// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUpgradeWidget.h"

#include "WeaponUpgradeCards.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "Logging/StructuredLog.h"


void UWeaponUpgradeWidget::PauseGame(const bool PauseStatus) const
{
	UGameplayStatics::SetGamePaused(GetWorld(), PauseStatus);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(PauseStatus);
}

void UWeaponUpgradeWidget::AddWidget(const bool WidgetStatus, const TArray<FUpgradeCommunication> WeaponUpgrades) const
{
	if(WidgetStatus)
	{
		if (!WeaponUpgrades.IsEmpty())
		{
			WeaponCardsHb->ClearChildren();
			for (const FUpgradeCommunication& WeaponUpgrade : WeaponUpgrades)
			{
				if (WeaponUpgradeCardWidget)
				{
					if (UWeaponUpgradeCards* WidgetInstance = CreateWidget<UWeaponUpgradeCards>(GetWorld(), WeaponUpgradeCardWidget))
					{
						WidgetInstance->SetUpgradeStats(WeaponUpgrade);
						WidgetInstance->SetPadding(FMargin(10.f, 0.f, 10.f, 0.f));
						WeaponCardsHb->AddChild(WidgetInstance);
					}
				}
				//UE_LOGFMT(LogTemp, Warning, "Card Added");
			}
			PauseGame(WidgetStatus);
		}
	}
	else
	{
		PauseGame(WidgetStatus);
	}
}