// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Passives.generated.h"

class APlayerCharacter;
class AWeaponUpgradeManager;

UCLASS()
class PARADIGM_IQ_API APassives : public AActor
{
	GENERATED_BODY()

#pragma region Protected Functions
protected:

	APassives();

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;

#pragma endregion

#pragma region Protected Variables

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Passives."))
	//TSoftObjectPtr<UDataTable> PassivesDataTable;

	UPROPERTY()
	AWeaponUpgradeManager* UpgradeManagerRef = nullptr;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

#pragma endregion

#pragma region Setters and Getters
public:

	FORCEINLINE void	SetPlayerCharacter(APlayerCharacter* PlayerCharRef) { PlayerCharacter = PlayerCharRef; }

#pragma endregion
};
