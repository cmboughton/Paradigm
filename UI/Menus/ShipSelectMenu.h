// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShipSelectMenu.generated.h"

struct FShipsDataTable;
class UShipSelect;
/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UShipSelectMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	void SetShipStats(const FName& DTRowName) const;

	UFUNCTION()
	void SetShipWeapon(const FShipsDataTable& ShipData) const;

	UFUNCTION()
	void SetShipUltimate(const FShipsDataTable& ShipData) const;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* ShipSelectGrid;

	UPROPERTY(meta = (BindWidget))
	class UImage* ShipRender;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ShipStatsVB;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* WeaponUltimateHB;

	UPROPERTY(meta = (BindWidget))
	class UWeaponStatsComponent* WBWeaponStatsComponent;

	UPROPERTY(meta = (BindWidget))
	class UWeaponStatsComponent* WBUltimateStatsComponent;

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ships."))
	TSoftObjectPtr<UDataTable> ShipDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Weapons."))
	TSoftObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ultimates."))
	TSoftObjectPtr<UDataTable> UltimateDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Reference to the Ship Select BP"))
	TSubclassOf<UUserWidget> ShipSelectRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Reference to the Ship Stat BP"))
	TSubclassOf<UUserWidget> ShipStatsRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Reference to the Weapon Stats Component BP"))
	TSubclassOf<UUserWidget> WeaponStatsRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Reference to the Ultimate Stats Component BP"))
	TSubclassOf<UUserWidget> UltimateStatsRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Reference to Ship Render Material"))
	UMaterial* ShipRenderMat;

public:

	UFUNCTION()
	void DisplayShip(const FName& RowName) const;
};
