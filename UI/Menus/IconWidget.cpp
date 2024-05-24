// Fill out your copyright notice in the Description page of Project Settings.


#include "IconWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UIconWidget::NativeConstruct()
{
	Icon->SetBrushFromTexture(IconTexture);
	Icon->SetDesiredSizeOverride(FVector2D(32.f,32.f));

	if(LevelReqText > 0)
	{
		if(CurrentPlayerLevel >= LevelReqText)
		{
			LevelText->SetText(FText::FromString(""));
		}
		else
		{
			LevelText->SetText(FText::FromString(FString::FromInt(LevelReqText)));
		}
	}
	else
	{
		LevelText->SetText(FText::FromString(""));
	}
}
