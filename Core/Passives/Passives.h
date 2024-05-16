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

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius);
#pragma endregion

#pragma region Protected Variables

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Passives."))
	//TSoftObjectPtr<UDataTable> PassivesDataTable;

	UPROPERTY()
	AWeaponUpgradeManager* UpgradeManagerRef = nullptr;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The max radius that can reach."))
	float AffectRadius = 2000.f;

#pragma endregion

#pragma region Setters and Getters
public:

	FORCEINLINE void	SetPlayerCharacter(APlayerCharacter* PlayerCharRef) { PlayerCharacter = PlayerCharRef; }

#pragma endregion
};
