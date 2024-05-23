// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	FName ShipRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Reference", meta = (ToolTip = "A Reference to the Player Character BP."))
	TSubclassOf<APawn> PlayerCharacterRef;

public:

	UFUNCTION(BlueprintCallable)
	void SpawnPlayer() const;

	FORCEINLINE void			SetShipRowName(const FName& ShipTextValue)			{ ShipRowName = ShipTextValue; }
};
