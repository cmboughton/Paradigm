// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUpgradeManager.h"

#include "Paradigm_IQ/UI/Menus/MenuComponets/WeaponUpgrade/WeaponUpgradeWidget.h"


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
				if (!UpgradeAvailable.WeaponUpgrades.IsEmpty())
				{
					for (FWeaponUpgrades WeaponUpgrades : UpgradeAvailable.WeaponUpgrades)
					{
						RollRange += WeaponUpgrades.RollWeight;
					}
				}
			}
			float Roll = FMath::RandRange(0.f, RollRange);
			int ArrayTracker2 = -1;
			for (FUpgradeManager UpgradeAvailable : CurrentUpgrades)
			{
				ArrayTracker2++;
				if(!UpgradeAvailable.WeaponUpgrades.IsEmpty())
				{
					int ArrayTracker = -1;
					for (FWeaponUpgrades WeaponUpgrades : UpgradeAvailable.WeaponUpgrades)
					{
						ArrayTracker++;
						CurrentRollTracker += WeaponUpgrades.RollWeight;
						if(Roll <= CurrentRollTracker && Roll > CurrentRollTracker - WeaponUpgrades.RollWeight)
						{
							UpgradesSelected.Add(FUpgradeCommunication(UpgradeAvailable.WeaponReference, WeaponUpgrades, UpgradeAvailable.WeaponType));
							if(CurrentUpgrades.IsValidIndex(ArrayTracker2))
							{
								if (CurrentUpgrades[ArrayTracker2].WeaponUpgrades.IsValidIndex(ArrayTracker))
								{
									CurrentUpgrades[ArrayTracker2].WeaponUpgrades.RemoveAt(ArrayTracker);
								}
							}
							//UE_LOGFMT(LogTemp, Warning, "Roll : {0} UpgradeSelect: {1}", Roll, WeaponUpgrades.UniqueName);
							//UE_LOGFMT(LogTemp, Warning, "Array Tracker: {0}, Array Tracker2: {1}", ArrayTracker, ArrayTracker2);
						}
					}
				}
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
	if(Upgrade.WeaponUpgrades.bCategoryUpgrade)
	{
		for(FUpgradeManager CurrentWeapons : UpgradesAvailable)
		{
			if(CurrentWeapons.WeaponType == Upgrade.WeaponType)
			{
				if(CurrentWeapons.WeaponReference)
				{
					CurrentWeapons.WeaponReference->UpgradeWeapon(Upgrade.WeaponUpgrades);
				}
			}
			//UE_LOGFMT(LogTemp, Warning, "Weapon: {0}", CurrentWeapons.WeaponReference->GetName());
		}
	}
	else
	{
		Upgrade.WeaponReference->UpgradeWeapon(Upgrade.WeaponUpgrades);
	}

	if(Upgrade.WeaponUpgrades.bSingleUse)
	{
		int index2 = 0;
		for (FUpgradeManager UpgradeAvailable : UpgradesAvailable)
		{
			if(UpgradeAvailable.WeaponReference == Upgrade.WeaponReference)
			{
				int index = 0;
				for (FWeaponUpgrades Upgrades : UpgradeAvailable.WeaponUpgrades)
				{
					if(Upgrades.UniqueName == Upgrade.WeaponUpgrades.UniqueName)
					{
						if(UpgradesAvailable.IsValidIndex(index2))
						{
							if(UpgradesAvailable[index2].WeaponUpgrades.IsValidIndex(index))
							{
								//UE_LOGFMT(LogTemp, Warning, "Upgrade Name {0}", UpgradesAvailable[index2].WeaponUpgrades[index].UniqueName);
								UpgradesAvailable[index2].WeaponUpgrades.RemoveAt(index);
							}
						}
					}
					index++;
				}
			}
			index2++;
		}
	}

	//UE_LOGFMT(LogTemp, Warning, "Weapon Selected: {0} Upgrade Selected {1}", Upgrade.WeaponReference->GetName(), Upgrade.WeaponUpgrades.UniqueName);

	bUpgradeActive = false;
}