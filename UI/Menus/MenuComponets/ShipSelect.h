// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShipSelect.generated.h"

class UShipSelectMenu;
/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UShipSelect : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ShipName;

	UPROPERTY(meta = (BindWidget))
	class UButton* ShipButton;

	UPROPERTY()
	FName ShipText;

	UPROPERTY()
	FName ShipRowName;

	UPROPERTY()
	UShipSelectMenu* ShipSelectRef;

public:

	FORCEINLINE void	SetShipText(const FName& ShipTextValue)					{ ShipText = ShipTextValue; }
	FORCEINLINE void	SetShipRowName(const FName& ShipTextValue)				{ ShipRowName = ShipTextValue; }
	FORCEINLINE void	SetShipSelectRef(UShipSelectMenu* WidgetAdded)			{ ShipSelectRef = WidgetAdded; }
};
