// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paradigm_IQ/Core/Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"


class UHealthBarComponent;
class AArcanicEcho;
class AWeaponUpgradeManager;
class AUltimateAbility;
class UMainHUDWidget;

UCLASS()
class PARADIGM_IQ_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

#pragma region Components
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* Follow camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthWidget;

#pragma endregion

#pragma region Input
protected:

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Ultimate Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* UltimateAction;

	/** Touch Interface**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Input", meta = (AllowPrivateAccess = "true"))
	class UTouchInterface* TouchInterface;
	

#pragma endregion

#pragma region Public Functions
public:

	// Sets default values for this character's properties
	APlayerCharacter();

	UFUNCTION()
	void AddCollectable(const FExperienceOrb Experience);

	UFUNCTION()
	void AddScore(const float AddedScore);

	UFUNCTION()
	void AddWeapon(const FName& WeaponName);

	UFUNCTION()
	void AddPassive(const FName& PassiveName);

	/** Called for Ultimate input */
	UFUNCTION()
	void Ultimate();

	virtual float TakeDamage(const float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	UFUNCTION()
	void SetInvulnerability(const float& DurationSec);

#pragma endregion

#pragma region Protected Functions
protected:

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Death() override;

	/** Called for movement input */
	UFUNCTION()
	void Move(const struct FInputActionValue& Value);

	UFUNCTION()
	void SetUpShip();

	UFUNCTION()
	void CollectablePickUp();

	UFUNCTION()
	static int CalculateExperienceForLevel(const int Level, const int BaseExperience, const float GrowthRate);

	UFUNCTION()
	void LerpHealthBar(const float& InDeltaTime);

#pragma endregion

#pragma  region Stats
protected:

	UPROPERTY()
	TArray<FName> WeaponsEquipped;

	//UPROPERTY()
	//int MaxWeaponsEquipped = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The levels required to unlock weapon slots."))
	TArray<int> WeaponUnlockLevels;

	UPROPERTY()
	TArray<FName> PassivesEquipped;

	//UPROPERTY()
	//int MaxPassivesEquipped = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The levels required to unlock passive slots."))
	TArray<int> PassiveUnlockLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ships."))
	TSoftObjectPtr<UDataTable> ShipDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ultimates."))
	TSoftObjectPtr<UDataTable> UltimatesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Weapons."))
	TSoftObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Passives."))
	TSoftObjectPtr<UDataTable> PassivesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The highest Score Modifer the player can achieve."))
	int MaxScoringModifier = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The score duration modifer used to determine when the modifer increases."))
	float ScoreDurationModifier = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The growth rate used to determine the how expodential the exp required per level becomes."))
	float LevelingGrowthRate = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The current level."))
	int CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The Radius of the sphere check that checks for collectables."))
	float PickUpRadius = 200.f;

	UPROPERTY()
	FName SelectedShipName;

	UPROPERTY()
	float HealthRegenRate = 0.f;

	UPROPERTY()
	bool bHasExtraLife = false;

	UPROPERTY()
	float InvulnerabilityDuration = 0.f;

#pragma endregion

#pragma region References
protected:

	UPROPERTY()
	AWeaponUpgradeManager* UpgradeManagerRef;

	UPROPERTY()
	AArcanicEcho* ArcanicEcho;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variables|References", meta = (ToolTip = "Reference to the Main Widget BP."))
	TSubclassOf<UUserWidget> MainWidgetUI = nullptr;

	UPROPERTY()
	UMainHUDWidget* WidgetInstance;

	UPROPERTY()
	UHealthBarComponent* HealthBar;

#pragma endregion

#pragma  region Trackers
protected:

	UPROPERTY()
	float CurrentUltimateTracker = 0.f;

	UPROPERTY()
	float UltimateTracker = 0.f;

	UPROPERTY()
	int ExperienceTracker = 0;

	UPROPERTY()
	float FireRateTracker = 0.f;

	UPROPERTY()
	float BulletDelayTracker = 0.f;

	UPROPERTY()
	int BulletTracker = 0;

	UPROPERTY()
	float ScoreModifierTracker = 0.f;

	UPROPERTY()
	int NextLevelReq = 0;

	UPROPERTY()
	float HealthRegenTracker = 0.f;

	UPROPERTY()
	float LerpCurrentHealth = 0.f;

#pragma endregion

#pragma  region Ultimate
protected:

	UPROPERTY()
	TSubclassOf<AUltimateAbility> UltimateAbilityRef;

#pragma endregion

#pragma region Getters and Setters
public:

	FORCEINLINE int								GetScoringModifier()									const { return ScoringModifier; }
	FORCEINLINE float							GetCurrentUltimate()									const { return CurrentUltimateTracker; }
	FORCEINLINE int								GetCurrentLevel()										const { return CurrentLevel; }
	FORCEINLINE TArray<FName>					GetWeaponsEquipped()									const { return WeaponsEquipped; }
	FORCEINLINE TArray<FName>					GetPassivesEquipped()									const { return PassivesEquipped; }
	FORCEINLINE float							GetPickUpRadius()										const { return PickUpRadius; }
	FORCEINLINE UMainHUDWidget*					GetMainHUDWidget()										const { return WidgetInstance; }
	FORCEINLINE TArray<int>						GetWeaponUnlockLevels()									const { return WeaponUnlockLevels; }
	FORCEINLINE TArray<int>						GetPassiveUnlockLevels()								const { return PassiveUnlockLevels; }
	FORCEINLINE TSoftObjectPtr<UDataTable>		GetShipDataTable()										const { return ShipDataTable; }
	FORCEINLINE FName							GetSelectedShipName()									const { return SelectedShipName; }

	FORCEINLINE void							SetPickUpRadius(const float& PickUpValue)					  { PickUpRadius = PickUpValue; }
	FORCEINLINE void							SetArcanicEchoRef(AArcanicEcho* AeValue)					  { ArcanicEcho = AeValue; }
	FORCEINLINE void							SetSelectedShipName(const FName& ShipNameValue)				  { SelectedShipName = ShipNameValue; }
	FORCEINLINE void							SetHasExtraLife(const bool& ExtraLifeValue)					  { bHasExtraLife = ExtraLifeValue; }

#pragma endregion
};
