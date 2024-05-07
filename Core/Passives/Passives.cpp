// Fill out your copyright notice in the Description page of Project Settings.


#include "Passives.h"

#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Weapons/WeaponUpgradeManager.h"

APassives::APassives()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APassives::BeginPlay()
{
	Super::BeginPlay();

	/*if (const UDataTable* PassivesDataTableHardRef = PassivesDataTable.LoadSynchronous())
	{
		if (const FPassivesDataTable* PassivesData = PassivesDataTableHardRef->FindRow<FPassivesDataTable>(RowName, "Passives Data Table Not set up. Passives.cpp", true))
		{
			AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponUpgradeManager::StaticClass());
			UpgradeManagerRef = Cast<AWeaponUpgradeManager>(FoundManager);
			if (UpgradeManagerRef)
			{
				FUpgradeManager PassiveUpgrades = FUpgradeManager(this, PassivesData->PassiveUpgrades, 0, PassivesData->WeaponType, false);
				UpgradeManagerRef->AddUpgrades(PassiveUpgrades);
			}
		}
	}*/
}

void APassives::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

