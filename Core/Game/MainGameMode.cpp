// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"

void AMainGameMode::SpawnPlayer() const
{
	const FTransform SpawnLocation = FTransform(FRotator(0.f, 0.f, 0.f), FVector(0.f, 0.f, 0.f), FVector(1.f, 1.f, 1.f));
	if(PlayerCharacterRef)
	{
		APlayerCharacter* PlayerCharacter = GetWorld()->SpawnActorDeferred<APlayerCharacter>(PlayerCharacterRef,SpawnLocation);
		if(const UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			PlayerCharacter->SetSelectedShipName(MainGameInstance->GetSelectedShip());
			UE_LOGFMT(LogTemp, Warning, "Ship Row Name: {0}", MainGameInstance->GetSelectedShip());
		}
		PlayerCharacter->FinishSpawning(SpawnLocation);
	}
}
