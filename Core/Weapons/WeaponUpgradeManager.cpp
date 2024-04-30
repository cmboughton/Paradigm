// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUpgradeManager.h"

#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/UI/Menus/MenuComponets/WeaponUpgrade/WeaponUpgradeWidget.h"
#include "Paradigm_IQ/Core/PlayerCharacter/PlayerCharacter.h"

// Sets default values
AWeaponUpgradeManager::AWeaponUpgradeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponUpgradeManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (const UDataTable* WeaponsDataTableHardRef = WeaponsDataTable.LoadSynchronous())
	{
		TArray<FName> WeaponRows = WeaponsDataTableHardRef->GetRowNames();
		for (FName WeaponRow: WeaponRows)
		{
			if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(WeaponRow, "Couldn't Add Weapon to Upgrade Pool", true))
			{
				if(!WeaponsData->bIsBaseWeapon)
				{
					TArray<FWeaponUpgrades> WeaponUnlocks;
					WeaponUnlocks.Add(FWeaponUpgrades(WeaponRow, WeaponsData->Description, WeaponsData->RollWeight, EUpgradeRarity::Basic, EWeaponUpgradeType::TriggerAmount, 0.f, false, false));
	;				UpgradesAvailable.Add(FUpgradeManager(nullptr, WeaponUnlocks, 0, EWeaponType::Mechanical, true));
					UE_LOGFMT(LogTemp, Warning, "Weapon Unlock: {0}", WeaponRow);
				}
			}
		}
	}
}

void AWeaponUpgradeManager::Tick(float DeltaSeconds)
{
	if(!UpgradeQueManager.IsEmpty())
	{
		if(!bUpgradeActive)
		{
			RollUpgrades(UpgradeQueManager[0]);
			UpgradeQueManager.RemoveAt(0);
		}
	}
}

void AWeaponUpgradeManager::AddUpgrades(const FUpgradeManager Upgrade)
{
	UpgradesAvailable.Add(Upgrade);
	
	//UE_LOGFMT(LogTemp, Warning, "Weapon {0} Upgrades Added", Upgrade.WeaponReference->GetName());
}

void AWeaponUpgradeManager::RollUpgrades(const int RollAmount)
{
	bUpgradeActive = true;
	TArray<FUpgradeManager> CurrentUpgrades = UpgradesAvailable;
	UpgradesSelected.Empty();
	for(int i = 0; i < RollAmount; i++)
	{
		if(!CurrentUpgrades.IsEmpty())
		{
			RollRange = 0;
			CurrentRollTracker = 0.f;
			for (FUpgradeManager UpgradeAvailable: CurrentUpgrades)
			{
				if (UpgradeAvailable.WeaponUpgrades.IsValidIndex(UpgradeAvailable.CurrentUpgradeLevel))
				{
					RollRange += UpgradeAvailable.WeaponUpgrades[UpgradeAvailable.CurrentUpgradeLevel].RollWeight;
					UE_LOGFMT(LogTemp, Warning, "Upgrade: {0}, Roll Range: {1}", UpgradeAvailable.WeaponUpgrades[UpgradeAvailable.CurrentUpgradeLevel].UniqueName, RollRange);
				}
			}

			float Roll = FMath::RandRange(0.f, RollRange);
			int ArrayTracker = -1;
			for (FUpgradeManager UpgradeAvailable : CurrentUpgrades)
			{
				ArrayTracker++;
				if(UpgradeAvailable.WeaponUpgrades.IsValidIndex(UpgradeAvailable.CurrentUpgradeLevel))
				{
					CurrentRollTracker += UpgradeAvailable.WeaponUpgrades[UpgradeAvailable.CurrentUpgradeLevel].RollWeight;
					
					if(Roll <= CurrentRollTracker && Roll > CurrentRollTracker - UpgradeAvailable.WeaponUpgrades[UpgradeAvailable.CurrentUpgradeLevel].RollWeight)
					{
						UpgradesSelected.Add(FUpgradeCommunication(UpgradeAvailable.WeaponReference, UpgradeAvailable.WeaponUpgrades[UpgradeAvailable.CurrentUpgradeLevel], UpgradeAvailable.WeaponType, UpgradeAvailable.bIsWeaponUnlock));
						UE_LOGFMT(LogTemp, Warning, "Roll : {0} UpgradeSelect: {1}", Roll, UpgradeAvailable.WeaponUpgrades[UpgradeAvailable.CurrentUpgradeLevel].UniqueName);
						break;

					}
				}
			}
			if (CurrentUpgrades.IsValidIndex(ArrayTracker))
			{
				CurrentUpgrades.RemoveAt(ArrayTracker);
			}
		}
	}
	if(!UpgradesSelected.IsEmpty())
	{
		if(WeaponUpgradeWidget)
		{
			if(UWeaponUpgradeWidget* WidgetInstance = CreateWidget<UWeaponUpgradeWidget>(GetWorld(), WeaponUpgradeWidget))
			{
				WidgetInstance->SetUpgradesToAdd(UpgradesSelected);
				WidgetInstance->AddToViewport();
			}
		}
	}
}

void AWeaponUpgradeManager::UpgradeSelected(FUpgradeCommunication Upgrade)
{
	if(Upgrade.bIsWeaponUnlock)
	{
		PlayerCharacter->AddWeapon(Upgrade.WeaponUpgrades.UniqueName);
		UE_LOGFMT(LogTemp, Warning, "Weapons Equipped: {0}", PlayerCharacter->GetWeaponsEquipped().Num());
		if(PlayerCharacter->GetWeaponsEquipped().Num() >= PlayerCharacter->GetMaxWeaponsEquipped())
		{
			UpgradesAvailable.RemoveAllSwap([&](const FUpgradeManager& UpgradeAvailable) {return UpgradeAvailable.bIsWeaponUnlock; });
		}
		else
		{
			int index = 0;
			UE_LOGFMT(LogTemp, Warning, "Weapon Unlock Selected: {0}", Upgrade.WeaponUpgrades.UniqueName);
			for (FUpgradeManager UpgradeAvailable : UpgradesAvailable)
			{
				int index2 = 0;
				for (FWeaponUpgrades WeaponUpgrade: UpgradeAvailable.WeaponUpgrades)
				{
					if(WeaponUpgrade.UniqueName == Upgrade.WeaponUpgrades.UniqueName)
					{
						UpgradesAvailable[index].WeaponUpgrades.RemoveAt(index2);
					}
					index2++;
				}
				index++;
			}
		}
	}
	else
	{
		if(Upgrade.WeaponUpgrades.bCategoryUpgrade)
		{
			for(FUpgradeManager CurrentWeapons : UpgradesAvailable)
			{
				if(CurrentWeapons.WeaponType == Upgrade.WeaponType)
				{
					if(CurrentWeapons.WeaponReference)
					{
						CurrentWeapons.WeaponReference->UpgradeWeapon(Upgrade.WeaponUpgrades);
						break;
					}
				}
				//UE_LOGFMT(LogTemp, Warning, "Weapon: {0}", CurrentWeapons.WeaponReference->GetName());
			}
		}
		else
		{
			Upgrade.WeaponReference->UpgradeWeapon(Upgrade.WeaponUpgrades);
		}

		//if(Upgrade.WeaponUpgrades.bSingleUse)
		//{
		//	int index2 = 0;
		//	for (FUpgradeManager UpgradeAvailable : UpgradesAvailable)
		//	{
		//		if(UpgradeAvailable.WeaponReference == Upgrade.WeaponReference)
		//		{
		//			int index = 0;
		//			for (FWeaponUpgrades Upgrades : UpgradeAvailable.WeaponUpgrades)
		//			{
		//				if(Upgrades.UniqueName == Upgrade.WeaponUpgrades.UniqueName)
		//				{
		//					if(UpgradesAvailable.IsValidIndex(index2))
		//					{
		//						if(UpgradesAvailable[index2].WeaponUpgrades.IsValidIndex(index))
		//						{
		//							//UE_LOGFMT(LogTemp, Warning, "Upgrade Name {0}", UpgradesAvailable[index2].WeaponUpgrades[index].UniqueName);
		//							UpgradesAvailable[index2].WeaponUpgrades.RemoveAt(index);
		//							break;
		//						}
		//					}
		//				}
		//				index++;
		//			}
		//		}
		//		index2++;
		//	}
		//}
		int index = 0;
		for (FUpgradeManager UpgradeAvailable: UpgradesAvailable)
		{
			if (UpgradeAvailable.WeaponReference == Upgrade.WeaponReference)
			{
				if(UpgradesAvailable.IsValidIndex(index))
				{
					UpgradesAvailable[index].CurrentUpgradeLevel++;
					UE_LOGFMT(LogTemp, Warning, "Upgrade Name {0}, Lvl: {1}", UpgradeAvailable.WeaponReference->GetName(), UpgradeAvailable.CurrentUpgradeLevel);
					break;
				}
			}
			index++;
		}
	}

	//UE_LOGFMT(LogTemp, Warning, "Weapon Selected: {0} Upgrade Selected {1}", Upgrade.WeaponReference->GetName(), Upgrade.WeaponUpgrades.UniqueName);

	bUpgradeActive = false;
}