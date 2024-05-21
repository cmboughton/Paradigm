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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Ultimate Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* UltimateAction;

	/** Touch Interface**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|Input", meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY()
	int MaxWeaponsEquipped = 6;

	UPROPERTY()
	TArray<FName> PassivesEquipped;

	UPROPERTY()
	int MaxPassivesEquipped = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ships."))
	TSoftObjectPtr<UDataTable> ShipDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ultimates."))
	TSoftObjectPtr<UDataTable> UltimatesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Weapons."))
	TSoftObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Passives."))
	TSoftObjectPtr<UDataTable> PassivesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Varaibles|Stats", meta = (ToolTip = "The highest Score Modifer the player can achieve."))
	int MaxScoringModifier = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Varaibles|Stats", meta = (ToolTip = "The score duration modifer used to determine when the modifer increases."))
	float ScoreDurationModifier = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Varaibles|Stats", meta = (ToolTip = "The growth rate used to determine the how expodential the exp required per level becomes."))
	float LevelingGrowthRate = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "Varaibles|Stats", meta = (ToolTip = "The current level."))
	int CurrentLevel = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Varaibles|Stats", meta = (ToolTip = "The Radius of the sphere check that checks for collectables."))
	float PickUpRadius = 200.f;

	UPROPERTY()
	FName SelectedShipName;

	UPROPERTY()
	float HealthRegenRate = 0.f;

#pragma endregion

#pragma region References
protected:

	UPROPERTY()
	AWeaponUpgradeManager* UpgradeManagerRef;

	UPROPERTY()
	AArcanicEcho* ArcanicEcho;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Varaibles|References", meta = (ToolTip = "Reference to the Main Widget BP."))
	TSubclassOf<UUserWidget> MainWidgetUI = nullptr;

	UPROPERTY()
	class UMainHUDWidget* WidgetInstance;

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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int						GetScoringModifier()							const { return ScoringModifier; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float					GetCurrentUltimate()							const { return CurrentUltimateTracker; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float					GetMaxUltimate()								const { return UltimateTracker; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int						GetCurrentXP()									const { return ExperienceTracker; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int						GetCurrentLevel()								const { return CurrentLevel; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int						GetNextLevelReq()								const { return NextLevelReq; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FName>			GetWeaponsEquipped()							const { return WeaponsEquipped; }
	FORCEINLINE int						GetMaxWeaponsEquipped()							const { return MaxWeaponsEquipped; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FName>			GetPassivesEquipped()							const { return PassivesEquipped; }
	FORCEINLINE int						GetMaxPassivesEquipped()						const { return MaxPassivesEquipped; }
	FORCEINLINE float					GetPickUpRadius()								const { return PickUpRadius; }

	FORCEINLINE void					SetPickUpRadius(const float PickUpValue)			  { PickUpRadius = PickUpValue; }
	FORCEINLINE void					SetArcanicEchoRef(AArcanicEcho* AeValue)			  { ArcanicEcho = AeValue; }
	FORCEINLINE void					SetSelectedShipName(const FName ShipNameValue)		  { SelectedShipName = ShipNameValue; }

#pragma endregion
};
