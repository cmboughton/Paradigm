// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Paradigm_IQ/Core/Ultimate/UltimateAbility.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PARADIGM_IQ_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* Follow camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseModel;

#pragma endregion

#pragma region Input

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Ultimate Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* UltimateAction;

#pragma endregion

#pragma region Public Functions

public:

	// Sets default values for this character's properties
	APlayerCharacter();

	UFUNCTION()
	void AddCollectable(struct FExperienceOrb Experience);

	UFUNCTION()
	void AddScore(const float AddedScore);

#pragma endregion

#pragma region Protected Functions

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(const float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const struct FInputActionValue& Value);

	/** Called for Ultimate input */
	void Ultimate();

	UFUNCTION()
	void SetUpShip();

	UFUNCTION()
	void CollectablePickUp();

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	UFUNCTION()
	void Death();


#pragma endregion

#pragma  region Stats

protected:

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ships."))
	TSoftObjectPtr<UDataTable> ShipDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Ultimates."))
	TSoftObjectPtr<UDataTable> UltimatesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Weapons."))
	TSoftObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY(BlueprintReadOnly)
	int Score = 0;

	UPROPERTY(BlueprintReadOnly)
	int ScoringModifier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Varaibles|Stats", meta = (ToolTip = "The highest Score Modifer the player can achieve."))
	int MaxScoringModifier = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Varaibles|Stats", meta = (ToolTip = "The score duration modifer used to determine when the modifer increases."))
	float ScoreDurationModifier = 5.f;

#pragma endregion

#pragma  region Trackers

protected:

	UPROPERTY()
	float CurrentUltimateTracker = 0.f;

	UPROPERTY()
	float UltimateTracker = 0.f;

	UPROPERTY()
	float ExperienceTracker = 0.f;

	UPROPERTY()
	float FireRateTracker = 0.f;

	UPROPERTY()
	float BulletDelayTracker = 0.f;

	UPROPERTY()
	int BulletTracker = 0;

	UPROPERTY()
	float ScoreModifierTracker = 0.f;

#pragma endregion

#pragma  region Ultimate

protected:

	UPROPERTY()
	TSubclassOf<AUltimateAbility> UltimateAbilityRef;

#pragma endregion

#pragma region Public Variables

public:

	FORCEINLINE int		GetScore()					const { return Score; }
	FORCEINLINE int		GetScoringModifier()		const { return ScoringModifier; }
	FORCEINLINE float	GetHealth()					const { return CurrentHealth; }
	FORCEINLINE float	GetMaxHealth()				const { return MaxHealth; }
	FORCEINLINE float	GetCurrentUltimate()		const { return CurrentUltimateTracker; }
	FORCEINLINE float	GetMaxUltimate()			const { return UltimateTracker; }

#pragma endregion
};
