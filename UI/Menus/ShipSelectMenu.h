// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShipSelectMenu.generated.h"

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

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* ShipSelectGrid;

	UPROPERTY(meta = (BindWidget))
	class UImage* ShipRender;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ships."))
	TSoftObjectPtr<UDataTable> ShipDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Reference to the Ship Select BP"))
	TSubclassOf<UUserWidget> ShipSelectRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Reference to Ship Render Material"))
	UMaterial* ShipRenderMat;

public:

	UFUNCTION()
	void DisplayShip(const FName& RowName) const;
};
