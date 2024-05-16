// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IconWidget.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UIconWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY()
	UTexture2D* IconTexture;

	virtual void NativeConstruct() override;

public:

	FORCEINLINE void						SetIconTexture(UTexture2D* IconTextureValue)		{ IconTexture = IconTextureValue; }
};
