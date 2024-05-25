// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

struct FUpgradeCommunication;
class APlayerCharacter;
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

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, const float InDeltaTime) override;

	UFUNCTION()
	void OnButtonClicked();

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

	UPROPERTY(meta = (BindWidget))
	class UButton* UltimateButton;

	UPROPERTY(meta = (BindWidget))
	class UDeathScreenWidget* WBDeathScreenWidget;

	UPROPERTY(meta = (BindWidget))
	class UWeaponUpgradeWidget* WBWeaponUpgradeWidget;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Weapons."))
	TSoftObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Passives."))
	TSoftObjectPtr<UDataTable> PassivesDataTable;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

public:

	UFUNCTION()
	void UpdateIcons() const;

	UFUNCTION()
	void ActivateDeathWidget() const;

	UFUNCTION()
	void DisplayUpgrades(const bool& bShouldDisplay, const TArray<FUpgradeCommunication> UpgradesAdded = TArray<FUpgradeCommunication>()) const;

public:

	FORCEINLINE void						SetScore(const float Value)								{ ScoreText = Value; }
	FORCEINLINE void						SetScoreMultiplier(const float MultiValue)				{ ScoreMultiplierText = MultiValue; }
	FORCEINLINE void						SetCurrentXP(const int XPValue)							{ ExperienceTracker = XPValue; }
	FORCEINLINE void						SetCurrentLevel(const int LvlValue)						{ CurrentLevel = LvlValue; }
	FORCEINLINE void						SetNextLevelReq(const int NextLvlValue)					{ NextLevelReq = NextLvlValue; }
	FORCEINLINE void						SetUltimateXP(const float UltimateValue)				{ UltimateXP = UltimateValue; }
	FORCEINLINE void						SetPlayerCharacter(APlayerCharacter* PlayerCharRef)		{ PlayerCharacter = PlayerCharRef; }
};
