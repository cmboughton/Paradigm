// Fill out your copyright notice in the Description page of Project Settings.


#include "IconWidget.h"
#include "Components/Image.h"

void UIconWidget::NativeConstruct()
{
	Icon->SetBrushFromTexture(IconTexture);
	Icon->SetDesiredSizeOverride(FVector2D(32.f,32.f));
}
