// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUpgradeManager.h"
#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"

AWeaponUpgradeManager::AWeaponUpgradeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponUpgradeManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (const UDataTable* WeaponsDataTableHardRef = WeaponsDataTable.LoadSynchronous())
	{
		TArray<FName> WeaponRows = WeaponsDataTableHardRef->GetRowNames();
		for (const FName WeaponRow: WeaponRows)
		{
			if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(WeaponRow, "Couldn't Add Weapon to Upgrade Pool", true))
			{
				if(!WeaponsData->bIsBaseWeapon)
				{
					//UE_LOGFMT(LogTemp, Warning, "Weapon not Base");
					TArray<FWeaponUpgrades> WeaponUnlocks;
					WeaponUnlocks.Add(FWeaponUpgrades(WeaponRow, WeaponsData->Description, WeaponsData->RollWeight, EUpgradeRarity::Basic, EWeaponUpgradeType::TriggerAmount, 0.f, true, false));
	;				UpgradesAvailable.Add(FUpgradeManager(nullptr, WeaponUnlocks, WeaponsData->SpecialUpgradeLevels, EWeaponType::Mechanical, true, false, false));
				}
			}
		}
	}
	if (const UDataTable* PassivesDataTableHardRef = PassivesDataTable.LoadSynchronous())
	{
		const TArray<int> EmptyArray;
		TArray<FName> PassiveRows = PassivesDataTableHardRef->GetRowNames();
		for (const FName PassiveRow : PassiveRows)
		{
			if (const FPassivesDataTable* PassivesData = PassivesDataTableHardRef->FindRow<FPassivesDataTable>(PassiveRow, "Couldn't Add Passives to Upgrade Pool", true))
			{
				TArray<FWeaponUpgrades> PassiveUnlocks;
				PassiveUnlocks.Add(FWeaponUpgrades(PassiveRow, PassivesData->Description, PassivesData->RollWeight, EUpgradeRarity::Basic, EWeaponUpgradeType::TriggerAmount, 0.f, true, false));
				UpgradesAvailable.Add(FUpgradeManager(nullptr, PassiveUnlocks, EmptyArray, EWeaponType::Mechanical, false, true, false));
				
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


/**
 * @brief Adds an upgrade to the available upgrades for the weapon.
 * 
 * @param Upgrade The upgrade to be added to the weapon.
 * @param SpecialUpgrade The Special Upgrade that will be added to the special upgrade array.
 */
void AWeaponUpgradeManager::AddUpgrades(const FUpgradeManager& Upgrade, const FUpgradeManager& SpecialUpgrade)
{
	UpgradesAvailable.Add(Upgrade);
	SpecialWeaponUpgrades.Add(SpecialUpgrade);
}

/**
 * @brief This method is responsible for rolling the upgrades for weapons.
 *
 * The method takes an integer parameter which represents the amount of rolls to be performed.
 * It then selects upgrades from the available upgrades based on their roll weight.
 * The selected upgrades are then added to the UpgradesSelected array.
 * If the UpgradesSelected array is not empty, a widget is created to display the selected upgrades.
 *
 * @param RollAmount The number of times the upgrade selection process should be performed.
 */
void AWeaponUpgradeManager::RollUpgrades(const int RollAmount)
{
	bUpgradeActive = true;
	TArray<FUpgradeManager> CurrentUpgrades = UpgradesAvailable;
	CurrentUpgrades.Append(SetUpSpecialUpgrades());
	UpgradesSelected.Empty();
	for(int i = 0; i < RollAmount; i++)
	{
		if(!CurrentUpgrades.IsEmpty())
		{
			/*for (auto CurrentUpgrade : CurrentUpgrades)
			{
				for (auto Upgrade : CurrentUpgrade.WeaponUpgrades)
				{
					UE_LOGFMT(LogTemp, Warning, "WeaponStartName: {0}", Upgrade.UniqueName);
				}
			}*/
			RollRange = 0;
			CurrentRollTracker = 0.f;
			for (FUpgradeManager UpgradeAvailable: CurrentUpgrades)
			{
				if(UpgradeAvailable.bIsPassiveUnlock || UpgradeAvailable.bIsWeaponUnlock)
				{
					if (UpgradeAvailable.WeaponUpgrades.IsValidIndex(0))
					{
						RollRange += UpgradeAvailable.WeaponUpgrades[0].RollWeight;
					}
				}
				else if(UpgradeAvailable.bIsSpecialUpgrade)
				{
					for (FWeaponUpgrades SpecialUpgrade : UpgradeAvailable.WeaponUpgrades)
					{
						RollRange += SpecialUpgrade.RollWeight;
					}
				}
				else
				{
					if(const AWeapons* WeaponRef = UpgradeAvailable.WeaponReference.LoadSynchronous())
					{
						if (UpgradeAvailable.WeaponUpgrades.IsValidIndex(WeaponRef->GetWeaponLevel()))
						{
							RollRange += UpgradeAvailable.WeaponUpgrades[WeaponRef->GetWeaponLevel()].RollWeight;
						}
					}
				}
			}
			const float Roll = FMath::RandRange(0.f, RollRange);
			int ArrayTracker = -1;
			for (FUpgradeManager UpgradeAvailable : CurrentUpgrades)
			{
				ArrayTracker++;
				if (UpgradeAvailable.bIsPassiveUnlock || UpgradeAvailable.bIsWeaponUnlock)
				{
					if (UpgradeAvailable.WeaponUpgrades.IsValidIndex(0))
					{
						CurrentRollTracker += UpgradeAvailable.WeaponUpgrades[0].RollWeight;
						/*UE_LOGFMT(LogTemp, Warning, "Roll: {0}", Roll);
						UE_LOGFMT(LogTemp, Warning, "RollMin: {0}", CurrentRollTracker - UpgradeAvailable.WeaponUpgrades[0].RollWeight);
						UE_LOGFMT(LogTemp, Warning, "RollMax: {0}", CurrentRollTracker);*/
						if (Roll <= CurrentRollTracker && Roll > CurrentRollTracker - UpgradeAvailable.WeaponUpgrades[0].RollWeight)
						{
							UpgradesSelected.Add(FUpgradeCommunication(UpgradeAvailable.WeaponReference, UpgradeAvailable.WeaponUpgrades[0], UpgradeAvailable.WeaponType, UpgradeAvailable.bIsWeaponUnlock, UpgradeAvailable.bIsPassiveUnlock, UpgradeAvailable.bIsSpecialUpgrade));
							if (CurrentUpgrades.IsValidIndex(ArrayTracker))
							{
								CurrentUpgrades.RemoveAt(ArrayTracker);
							}
							break;
						}
					}
				}
				else if (UpgradeAvailable.bIsSpecialUpgrade)
				{
					bool bShouldBreak = false;
					for (int j =0; j < UpgradeAvailable.WeaponUpgrades.Num(); j++)
					{
						CurrentRollTracker += UpgradeAvailable.WeaponUpgrades[j].RollWeight;
						/*UE_LOGFMT(LogTemp, Warning, "Roll: {0}", Roll);
						UE_LOGFMT(LogTemp, Warning, "RollMin: {0}", CurrentRollTracker - UpgradeAvailable.WeaponUpgrades[j].RollWeight);
						UE_LOGFMT(LogTemp, Warning, "RollMax: {0}", CurrentRollTracker);*/
						if (Roll <= CurrentRollTracker && Roll > CurrentRollTracker - UpgradeAvailable.WeaponUpgrades[j].RollWeight)
						{
							UpgradesSelected.Add(FUpgradeCommunication(UpgradeAvailable.WeaponReference, UpgradeAvailable.WeaponUpgrades[j], UpgradeAvailable.WeaponType, UpgradeAvailable.bIsWeaponUnlock, UpgradeAvailable.bIsPassiveUnlock, UpgradeAvailable.bIsSpecialUpgrade));
							if (CurrentUpgrades.IsValidIndex(ArrayTracker))
							{
								if(CurrentUpgrades[ArrayTracker].WeaponUpgrades.IsValidIndex(j))
								{
									CurrentUpgrades[ArrayTracker].WeaponUpgrades.RemoveAt(j);
								}
								//UE_LOGFMT(LogTemp, Warning, "Index removed: {0}", CurrentUpgrades[ArrayTracker].WeaponUpgrades[0].UniqueName);
							}
							bShouldBreak = true;
							break;
						}
					}
					if (bShouldBreak)
					{
						break;
					}
				}
				else
				{
					if (const AWeapons* WeaponRef = UpgradeAvailable.WeaponReference.LoadSynchronous())
					{
						if (UpgradeAvailable.WeaponUpgrades.IsValidIndex(WeaponRef->GetWeaponLevel()))
						{
							CurrentRollTracker += UpgradeAvailable.WeaponUpgrades[WeaponRef->GetWeaponLevel()].RollWeight;
							/*UE_LOGFMT(LogTemp, Warning, "Roll: {0}", Roll);
							UE_LOGFMT(LogTemp, Warning, "RollMin: {0}", CurrentRollTracker - UpgradeAvailable.WeaponUpgrades[WeaponRef->GetWeaponLevel()].RollWeight);
							UE_LOGFMT(LogTemp, Warning, "RollMax: {0}", CurrentRollTracker);*/
							if (Roll <= CurrentRollTracker && Roll > CurrentRollTracker - UpgradeAvailable.WeaponUpgrades[WeaponRef->GetWeaponLevel()].RollWeight)
							{
								UpgradesSelected.Add(FUpgradeCommunication(UpgradeAvailable.WeaponReference, UpgradeAvailable.WeaponUpgrades[WeaponRef->GetWeaponLevel()], UpgradeAvailable.WeaponType, UpgradeAvailable.bIsWeaponUnlock, UpgradeAvailable.bIsPassiveUnlock, UpgradeAvailable.bIsSpecialUpgrade));
								if (CurrentUpgrades.IsValidIndex(ArrayTracker))
								{
									CurrentUpgrades.RemoveAt(ArrayTracker);
								}
								break;
							}
						}
					}
				}
			}
			/*for (auto CurrentUpgrade : CurrentUpgrades)
			{
				for (auto Upgrade : CurrentUpgrade.WeaponUpgrades)
				{
					UE_LOGFMT(LogTemp, Warning, "WeaponEndName: {0}", Upgrade.UniqueName);
				}
			}*/
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

/**
 * @brief Upgrades the selected weapon or weapon category.
 *
 * This method is responsible for upgrading the selected weapon or weapon category based on the provided upgrade communication.
 * If the upgrade is a weapon unlock, it adds the weapon to the player's weapons and checks if the player is at max weapons.
 * If the player is at max weapons, all weapon unlock cards will be removed from the pool.
 * If the upgrade is a category upgrade, it upgrades all weapons of that category.
 * If an upgrade is marked as single use, it will be removed from the upgrade pool.
 * It also increments the upgrade cards level so the next upgrade can be added to the upgrade pool.
 *
 * @param Upgrade The upgrade communication containing the details of the upgrade.
 */
void AWeaponUpgradeManager::UpgradeSelected(const FUpgradeCommunication& Upgrade)
{
	if(Upgrade.bIsWeaponUnlock)
	{
		if (PlayerCharacter)
		{
			// Adds the weapon selected to the players weapons
			PlayerCharacter->AddWeapon(Upgrade.WeaponUpgrades.UniqueName);
			UE_LOGFMT(LogTemp, Warning, "Weapon Name: {0}", Upgrade.WeaponUpgrades.UniqueName);

			// Checks if the player is at max weapons. If so all weapon unlock cards will be removed from the pool.
			if (PlayerCharacter->GetWeaponsEquipped().Num() >= PlayerCharacter->GetMaxWeaponsEquipped())
			{
				UpgradesAvailable.RemoveAllSwap([&](const FUpgradeManager& UpgradeAvailable) {return UpgradeAvailable.bIsWeaponUnlock; });
			}
			else
			{
				UpgradeSingleUse(Upgrade);
			}
		}
		else
		{
			PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			UpgradeSelected(Upgrade);
		}
	}
	else if (Upgrade.bIsPassiveUnlock)
	{
		if (PlayerCharacter)
		{
			// Adds the weapon selected to the players weapons
			PlayerCharacter->AddPassive(Upgrade.WeaponUpgrades.UniqueName);

			// Checks if the player is at max weapons. If so all weapon unlock cards will be removed from the pool.
			if (PlayerCharacter->GetPassivesEquipped().Num() >= PlayerCharacter->GetMaxPassivesEquipped())
			{
				UpgradesAvailable.RemoveAllSwap([&](const FUpgradeManager& UpgradeAvailable) {return UpgradeAvailable.bIsPassiveUnlock; });
			}
			else
			{
				UpgradeSingleUse(Upgrade);
			}
		}
		else
		{
			PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			UpgradeSelected(Upgrade);
		}
	}
	else if (Upgrade.bIsSpecialUpgrade)
	{
		Upgrade.WeaponReference->UpgradeWeapon(Upgrade.WeaponUpgrades);

		for (int i = 0; i < SpecialWeaponUpgrades.Num(); i++)
		{
			if(SpecialWeaponUpgrades.IsValidIndex(i))
			{
				if(SpecialWeaponUpgrades[i].WeaponReference == Upgrade.WeaponReference)
				{
					if (const AWeapons* WeaponRef = SpecialWeaponUpgrades[i].WeaponReference.LoadSynchronous())
					{
						/*if(WeaponRef->GetSpecialUpgradeTracker() >= SpecialWeaponUpgrades[i].WeaponUpgrades.Num())
						{
							UE_LOGFMT(LogTemp, Warning, "Upgrade Tracker: {0} WeaponUpgradesNum: {1}", WeaponRef->GetSpecialUpgradeTracker(), SpecialWeaponUpgrades[i].WeaponUpgrades.Num());
							SpecialWeaponUpgrades.RemoveAllSwap([&](const FUpgradeManager& Weapon) {return Weapon.WeaponReference == WeaponRef; });
						}
						else
						{*/
							for(int j = 0; j < SpecialWeaponUpgrades[i].WeaponUpgrades.Num(); j++)
							{
								if(SpecialWeaponUpgrades[i].WeaponUpgrades.IsValidIndex(j))
								{
									if(SpecialWeaponUpgrades[i].WeaponUpgrades[j].UniqueName == Upgrade.WeaponUpgrades.UniqueName)
									{
										//UE_LOGFMT(LogTemp, Warning, "Upgrade Removed: {0}", SpecialWeaponUpgrades[i].WeaponUpgrades[j].UniqueName);
										SpecialWeaponUpgrades[i].WeaponUpgrades.RemoveAt(j);
									}
								}
							}
						
					}
				}
			}
		}
	}
	else
	{
		/**
		 *	Checks if the Upgrade is a Category Upgrade
		 *	If it is then it Upgrades all Weapons that of that Category
		 */
		if(Upgrade.WeaponUpgrades.bCategoryUpgrade)
		{
			UpgradeCategory(Upgrade);
		}
		else
		{
			Upgrade.WeaponReference->UpgradeWeapon(Upgrade.WeaponUpgrades);
		}

		/**
		 *	If an Upgrade is marked as Single Use. It will be removed from the upgrade pool.
		 */
		if(Upgrade.WeaponUpgrades.bSingleUse)
		{
			UpgradeSingleUse(Upgrade);
		}

		/**
		 *	Increments the Upgrade cards level so the next upgrade can be added to the upgrade pool.
		 */
		if(AWeapons* WeaponRef = Upgrade.WeaponReference.LoadSynchronous())
		{
			WeaponRef->SetWeaponLevel(WeaponRef->GetWeaponLevel() + 1);
			//UE_LOGFMT(LogTemp, Warning, "Weapon Level: {0}", WeaponRef->GetWeaponLevel());
		}
		/*for(int i = 0; i < UpgradesAvailable.Num(); i ++)
		{
			if(UpgradesAvailable.IsValidIndex(i))
			{
				if (UpgradesAvailable[i].WeaponReference == Upgrade.WeaponReference)
				{
					UpgradesAvailable[i].CurrentUpgradeLevel++;
					break;
				}
			}
		}*/
	}
	bUpgradeActive = false;
}

/**
 * @brief Upgrades the category of a weapon.
 *
 * This function iterates over all available upgrades and applies the specified upgrade to all weapons of the same type.
 *
 * @param Upgrade The upgrade to be applied. This includes the type of the weapon to be upgraded and the details of the upgrade.
 */
void AWeaponUpgradeManager::UpgradeCategory(const FUpgradeCommunication& Upgrade)
{
	for (FUpgradeManager CurrentWeapons : UpgradesAvailable)
	{
		if (CurrentWeapons.WeaponType == Upgrade.WeaponType)
		{
			if (CurrentWeapons.WeaponReference)
			{
				CurrentWeapons.WeaponReference->UpgradeWeapon(Upgrade.WeaponUpgrades);
			}
		}
	}
}

/**
 * @brief Upgrades a single use weapon.
 *
 * This method iterates through all available upgrades and applies the specified upgrade to the weapon.
 * If the upgrade is found and successfully applied, it is then removed from the list of available upgrades.
 *
 * @param Upgrade The upgrade to be applied. This is an instance of FUpgradeCommunication which contains the weapon reference and the details of the upgrade.
 */
void AWeaponUpgradeManager::UpgradeSingleUse(const FUpgradeCommunication& Upgrade)
{
	for(int i = 0; i < UpgradesAvailable.Num(); i++)
	{
		if(UpgradesAvailable.IsValidIndex(i))
		{
			if (UpgradesAvailable[i].WeaponReference == Upgrade.WeaponReference)
			{
				for(int j = 0; j < UpgradesAvailable[i].WeaponUpgrades.Num(); j++)
				{
					if (UpgradesAvailable[i].WeaponUpgrades.IsValidIndex(j))
					{
						if (UpgradesAvailable[i].WeaponUpgrades[j].UniqueName == Upgrade.WeaponUpgrades.UniqueName)
						{
							//UE_LOGFMT(LogTemp, Warning, "Weapon Ref: {0}", UpgradesAvailable[i].WeaponUpgrades[j].UniqueName);
							UpgradesAvailable[i].WeaponUpgrades.RemoveAt(j);
							return;
						}
					}
				}
			}
		}
	}
}

/**
 * @brief Sets up special upgrades for the weapon.
 *
 * This function iterates over the special weapon upgrades and checks if the weapon's special upgrade level is less than or equal to the weapon's level. 
 * If the condition is met, the special upgrade is added to the list of special upgrades. 
 * The function returns a list of special upgrades that meet the condition.
 *
 * @return TArray<FUpgradeManager> List of special upgrades that meet the condition.
 */
TArray<FUpgradeManager> AWeaponUpgradeManager::SetUpSpecialUpgrades()
{
	TArray<FUpgradeManager> SpecialUpgrades;

	for (const FUpgradeManager SpecialWeaponUpgrade : SpecialWeaponUpgrades)
	{
		if(const AWeapons* WeaponRef = SpecialWeaponUpgrade.WeaponReference.LoadSynchronous())
		{
			int index = 0;
			for (const auto WeaponUpgrade : SpecialWeaponUpgrade.WeaponUpgrades)
			{
				if((!SpecialWeaponUpgrade.WeaponUpgrades.IsEmpty()) && SpecialWeaponUpgrade.SpecialUpgradeLevels.IsValidIndex(WeaponRef->GetSpecialUpgradeTracker()))
				{
					if(SpecialWeaponUpgrade.SpecialUpgradeLevels[WeaponRef->GetSpecialUpgradeTracker()] <= WeaponRef->GetWeaponLevel())
					{
						SpecialUpgrades.Add(SpecialWeaponUpgrade);
						//UE_LOGFMT(LogTemp, Warning, "Special Upgrade Added: {0}", SpecialWeaponUpgrade.WeaponUpgrades[index].UniqueName);
						break;
					}
				}
				index++;
			}
		}
	}
	return SpecialUpgrades;
}