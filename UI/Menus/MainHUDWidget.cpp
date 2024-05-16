// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

FString UMainHUDWidget::AddCommasToInt(const int& NumberToAddCommas)
{
    FString NumberString = FString::FromInt(NumberToAddCommas);
    const int Length = NumberString.Len();

    // Check if commas need to be added
    if (Length <= 3)
    {
        return NumberString;
    }

    // Calculate the number of commas to insert
    const int NumCommas = (Length - 1) / 3;

    // Insert commas into the string
    for (int i = 1; i <= NumCommas; ++i)
    {
        const int Index = Length - i * 3;
        NumberString.InsertAt(Index, TEXT(","));
    }
    return NumberString;
}

void UMainHUDWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    const float GameTime    = UGameplayStatics::GetTimeSeconds(GetWorld());
    const int Hours         = FMath::Floor(GameTime / 3600);
    int TotalSeconds        = FMath::Floor(GameTime);
    TotalSeconds            = FMath::Floor(TotalSeconds % 3600);
    const int Minutes       = FMath::Floor(TotalSeconds / 60);
    const int Seconds       = FMath::Floor(TotalSeconds % 60);
    FString MinutesText;
    FString SecondsText;
    FString HoursText;

    (Seconds < 10) ? SecondsText = (FString("0").Append((FString::FromInt(Seconds)))) : SecondsText = (FString::FromInt(Seconds));
    (Minutes < 10) ? MinutesText = (FString("0").Append((FString::FromInt(Minutes)))) : MinutesText = (FString::FromInt(Minutes));
    (Hours   < 10) ? HoursText   = (FString("0").Append((FString::FromInt(Hours))))   : HoursText   = (FString::FromInt(Hours));
    (Hours   > 0)  ? TimeValue->SetText(FText::FromString(HoursText.Append(":").Append(MinutesText).Append(":").Append(SecondsText))) : TimeValue->SetText(FText::FromString(MinutesText.Append(":").Append(SecondsText)));

    ScoreValue->SetText(FText::FromString((AddCommasToInt(FMath::CeilToInt(ScoreText)))));
    ScoreMultiplier->SetText(FText::FromString(FString("x").Append(FString::FromInt(ScoreMultiplierText))));

    LevelText->SetText(FText::FromString(FString::FromInt(CurrentLevel)));
	XPProgressBar->SetPercent(UKismetMathLibrary::NormalizeToRange(ExperienceTracker, 0, NextLevelReq));
}
