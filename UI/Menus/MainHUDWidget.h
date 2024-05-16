// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

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

public:

	FORCEINLINE void						SetScore(const float Value)						{ ScoreText = Value; }
	FORCEINLINE void						SetScoreMultiplier(const float MultiValue)		{ ScoreMultiplierText = MultiValue; }
	FORCEINLINE void						SetCurrentXP(const int XPValue)					{ ExperienceTracker = XPValue; }
	FORCEINLINE void						SetCurrentLevel(const int LvlValue)				{ CurrentLevel = LvlValue; }
	FORCEINLINE void						SetNextLevelReq(const int NextLvlValue)			{ NextLevelReq = NextLvlValue; }
};
