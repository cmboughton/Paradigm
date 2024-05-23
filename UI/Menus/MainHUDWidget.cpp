// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDWidget.h"

#include "DeathScreenWidget.h"
#include "IconWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MenuComponets/WeaponUpgrade/WeaponUpgradeWidget.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"
#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

    UltimateButton->OnClicked.AddDynamic(this, &UMainHUDWidget::OnButtonClicked);
}

void UMainHUDWidget::OnButtonClicked()
{
    if(PlayerCharacter)
    {
        PlayerCharacter->Ultimate();
    }
    else
    {
	    if(IsValid(PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))))
	    {
            PlayerCharacter->Ultimate();
	    }
    }
}

void UMainHUDWidget::ActivateDeathWidget() const
{
    WBDeathScreenWidget->SetVisibility(ESlateVisibility::Visible);
    WBDeathScreenWidget->PlayAnimation(WBDeathScreenWidget->GetFadeIn());
}

void UMainHUDWidget::DisplayUpgrades(const bool& bShouldDisplay, const TArray<FUpgradeCommunication> UpgradesAdded) const
{
    (bShouldDisplay) ? WBWeaponUpgradeWidget->SetVisibility(ESlateVisibility::Visible) : WBWeaponUpgradeWidget->SetVisibility(ESlateVisibility::Collapsed);
    WBWeaponUpgradeWidget->AddWidget(bShouldDisplay, UpgradesAdded);
}

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

    if (CurrentUltimateXP != UltimateXP)
    {
        constexpr float LerpSpeed = 3.0f;
        CurrentUltimateXP = FMath::Lerp(CurrentUltimateXP, UltimateXP, InDeltaTime * LerpSpeed);
    }
	LeftProgress->GetDynamicMaterial()->SetScalarParameterValue("Percentage", CurrentUltimateXP);
	RightProgress->GetDynamicMaterial()->SetScalarParameterValue("Percentage", CurrentUltimateXP);
}

void UMainHUDWidget::AddIcon(UTexture2D* Icon, const EIconType& IconType) const
{
    if (IconWidget)
    {
        UIconWidget* WidgetInstance = CreateWidget<UIconWidget>(GetWorld(), IconWidget);

        switch (IconType)
        {
        case EIconType::WeaponIcon:
            
            WeaponIconsHB->RemoveChildAt(0);
            WidgetInstance->SetIconTexture(Icon);
            WeaponIconsHB->AddChild(WidgetInstance);
            break;

        case EIconType::PassiveIcon:

            PassiveIconsHB->RemoveChildAt(0);
            WidgetInstance->SetIconTexture(Icon);
            PassiveIconsHB->AddChild(WidgetInstance);
            break;
        }
    }
}

void UMainHUDWidget::SetUpDefaultIcons(const EIconType& IconType) const
{
    if (IconWidget)
    {
        UIconWidget* WidgetInstance = CreateWidget<UIconWidget>(GetWorld(), IconWidget);

        switch (IconType)
        {
        case EIconType::WeaponIcon:

            WidgetInstance->SetIconTexture(DefaultIcon);
            WeaponIconsHB->AddChild(WidgetInstance);
            break;

        case EIconType::PassiveIcon:

            WidgetInstance->SetIconTexture(DefaultIcon);
            PassiveIconsHB->AddChild(WidgetInstance);
            break;
        }
    }
}
