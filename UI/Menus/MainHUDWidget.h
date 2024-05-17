// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Paradigm_IQ/Core/Data/Enums/Enums.h"
#include "MainHUDWidget.generated.h"

class UIconWidget;
/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	static FString AddCommasToInt(const int& NumberToAddCommas);

	virtual void NativeTick(const FGeometry& MyGeometry, const float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreMultiplier;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* XPProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* WeaponIconsHB;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* PassiveIconsHB;

	UPROPERTY(meta = (BindWidget))
	class UImage* LeftProgress;

	UPROPERTY(meta = (BindWidget))
	class UImage* RightProgress;

	UPROPERTY()
	float ScoreText = 0.f;

	UPROPERTY()
	int ScoreMultiplierText = 0;

	UPROPERTY()
	int ExperienceTracker;

	UPROPERTY()
	int CurrentLevel;

	UPROPERTY()
	int NextLevelReq;

	UPROPERTY()
	float CurrentUltimateXP = 0.f;

	UPROPERTY()
	float UltimateXP = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Reference to BP_IconWidget."))
	TSubclassOf<UUserWidget> IconWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Refernece to the defualt Icon Texture."))
	UTexture2D* DefaultIcon = nullptr;

public:

	UFUNCTION()
	void AddIcon(UTexture2D* Icon, const EIconType& IconType) const;

	UFUNCTION()
	void SetUpDefaultIcons(const EIconType& IconType) const;

public:

	FORCEINLINE void						SetScore(const float Value)						{ ScoreText = Value; }
	FORCEINLINE void						SetScoreMultiplier(const float MultiValue)		{ ScoreMultiplierText = MultiValue; }
	FORCEINLINE void						SetCurrentXP(const int XPValue)					{ ExperienceTracker = XPValue; }
	FORCEINLINE void						SetCurrentLevel(const int LvlValue)				{ CurrentLevel = LvlValue; }
	FORCEINLINE void						SetNextLevelReq(const int NextLvlValue)			{ NextLevelReq = NextLvlValue; }
	FORCEINLINE void						SetUltimateXP(const float UltimateValue)		{ UltimateXP = UltimateValue; }
};
