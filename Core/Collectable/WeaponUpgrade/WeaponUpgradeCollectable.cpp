// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUpgradeCollectable.h"

#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Weapons/WeaponUpgradeManager.h"

void AWeaponUpgradeCollectable::Collected(APlayerCharacter* PlayerCharacterRef)
{
	Super::Collected(PlayerCharacterRef);

	AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponUpgradeManager::StaticClass());

	if (AWeaponUpgradeManager* UpgradeManagerRef = Cast<AWeaponUpgradeManager>(FoundManager))
	{
		UpgradeManagerRef->UpgradeQueManager.Add(UpgradeRolls);
	}
}
