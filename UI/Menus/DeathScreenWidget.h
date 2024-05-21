// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UDeathScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	void OnButtonClicked();

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeIn;

public:

	FORCEINLINE UWidgetAnimation*			GetFadeIn()			const { return FadeIn; }
};
