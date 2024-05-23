// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUpgradeCards.h"

#include "WeaponUpgradeWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/Weapons/WeaponUpgradeManager.h"

void UWeaponUpgradeCards::NativeConstruct()
{
	UpgradeName->SetText(FText::FromString(WeaponUpgradeStats.WeaponUpgrades.UniqueName.ToString()));
	UpgradeDescription->SetText(FText::FromString(WeaponUpgradeStats.WeaponUpgrades.Description));

	switch (WeaponUpgradeStats.WeaponUpgrades.UpgradeRarity)
	{
	case EUpgradeRarity::Basic:
		CardButton->SetBackgroundColor(FLinearColor(0.33f, 0.33f, 0.33f, .8f));
		break;

	case EUpgradeRarity::Prime:
		CardButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 1.f, .8f));
		break;

	case EUpgradeRarity::Cosmic:
		CardButton->SetBackgroundColor(FLinearColor(0.5f, 0.1f, .6f, .8f));
		break;
	}

	CardButton->OnClicked.AddDynamic(this, &UWeaponUpgradeCards::OnButtonClicked);
}

void UWeaponUpgradeCards::OnButtonClicked()
{
	AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponUpgradeManager::StaticClass());

	if (AWeaponUpgradeManager* UpgradeManagerRef = Cast<AWeaponUpgradeManager>(FoundManager))
	{
		UpgradeManagerRef->UpgradeSelected(WeaponUpgradeStats);
	}
}
