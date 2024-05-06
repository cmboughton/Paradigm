// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUpgradeCollectable.h"

#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Weapons/WeaponUpgradeManager.h"

/**
 * @brief This method is called when a weapon upgrade is collected by a player character.
 * 
 * @param PlayerCharacterRef A reference to the player character who collected the weapon upgrade.
 * 
 * The method performs the following steps:
 * 1. Calls the parent class's `Collected` method.
 * 2. Finds the `AWeaponUpgradeManager` in the game world.
 * 3. If the `AWeaponUpgradeManager` is found, it sets the upgrade queue manager with the upgrade rolls.
 */
void AWeaponUpgradeCollectable::Collected(APlayerCharacter* PlayerCharacterRef)
{
	Super::Collected(PlayerCharacterRef);

	AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponUpgradeManager::StaticClass());

	if (AWeaponUpgradeManager* UpgradeManagerRef = Cast<AWeaponUpgradeManager>(FoundManager))
	{
		UpgradeManagerRef->SetUpgradeQueManager(UpgradeRolls);
	}
}
