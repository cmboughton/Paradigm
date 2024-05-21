// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"

#include "Paradigm_IQ/Core/Ultimate/UltimateAbility.h"
#include "../EnemyCharacter/EnemyCharacter.h"

#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"
#include "Paradigm_IQ/Core/Data/Interfaces/CollectableInterface.h"

#include "Paradigm_IQ/Core/Passives/Passives.h"
#include "Paradigm_IQ/Core/Passives/ArcanicEcho/ArcanicEcho.h"

#include "Paradigm_IQ/Core/Weapons/Weapons.h"
#include "Paradigm_IQ/Core/Weapons/WeaponUpgradeManager.h"

#include "Paradigm_IQ/UI/Menus/MainHUDWidget.h"
#include "Paradigm_IQ/UI/Menus/MenuComponets/HealthBarComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->TargetArmLength = 2000;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagMaxDistance = 2000.f;
	CameraBoom->CameraLagSpeed = 1.f;
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));
	HealthWidget->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (MainWidgetUI)
	{
		if (IsValid(WidgetInstance = CreateWidget<UMainHUDWidget>(GetWorld(), MainWidgetUI)))
		{
			WidgetInstance->AddToViewport();
			WidgetInstance->SetScoreMultiplier(ScoringModifier);
			WidgetInstance->SetPlayerCharacter(this);

			for(int i = 0; i < MaxWeaponsEquipped; i++)
			{
				WidgetInstance->SetUpDefaultIcons(EIconType::WeaponIcon);
			}
			for (int i = 0; i < MaxPassivesEquipped; i++)
			{
				WidgetInstance->SetUpDefaultIcons(EIconType::PassiveIcon);
			}
		}
	}

	SetUpShip();

	AActor* FoundManager = UGameplayStatics::GetActorOfClass(GetWorld(), AWeaponUpgradeManager::StaticClass());

	UpgradeManagerRef = Cast<AWeaponUpgradeManager>(FoundManager);

	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


	NextLevelReq = CalculateExperienceForLevel(CurrentLevel, 100, LevelingGrowthRate);
	WidgetInstance->SetCurrentLevel(CurrentLevel);
	WidgetInstance->SetNextLevelReq(NextLevelReq);
	AddCollectable(FExperienceOrb(0, 0));

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->ActivateTouchInterface(TouchInterface);
	}

	if (HealthWidget)
	{
		HealthBar = Cast<UHealthBarComponent>(HealthWidget->GetUserWidgetObject());
	}
}

void APlayerCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	CollectablePickUp();

	//Score Multiplier
	if(ScoringModifier < MaxScoringModifier)
	{
		if(ScoringModifier * ScoreDurationModifier <= ScoreModifierTracker)
		{
			ScoringModifier++;
			WidgetInstance->SetScoreMultiplier(ScoringModifier);
			ScoreModifierTracker = 0.f;
		}
		else
		{
			ScoreModifierTracker += DeltaTime;
		}
	}

	if(CurrentHealth < MaxHealth && HealthRegenTracker <= 0)
	{
		UpdateHealth(HealthRegenRate * (MaxHealth * 0.05));
		HealthRegenTracker = 5.f;
	}
	else if(HealthRegenTracker > 0)
	{
		HealthRegenTracker -= DeltaTime;
	}

	LerpHealthBar(DeltaTime);
}

/**
 * @brief Sets up the player input component.
 * 
 * This function is responsible for binding the player's input to the character's actions. 
 * It binds the player's movement and ultimate actions to the corresponding functions in the class.
 * 
 * @param PlayerInputComponent The input component of the player.
 */
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Ultimate
		EnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Started, this, &APlayerCharacter::Ultimate);

	}
}

/**
 * @brief This function is used to apply damage to the player character.
 * 
 * @param DamageAmount The amount of damage to be applied.
 * @param DamageEvent The type of damage event that is causing the damage.
 * @param EventInstigator The controller that was responsible for causing this damage.
 * @param DamageCauser The actor that was responsible for causing this damage.
 * 
 * @return The current health of the player character after the damage has been applied.
 */
float APlayerCharacter::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ScoringModifier = 1;
	WidgetInstance->SetScoreMultiplier(ScoringModifier);
	ScoreModifierTracker = 0.f;
	if(ArcanicEcho)
	{
		ArcanicEcho->SetWasHit(true);
		ArcanicEcho->SetDamage(DamageAmount);
	}
	return CurrentHealth;
}

/**
 * @brief Interpolates the health bar value based on the current health of the player character.
 *
 * This function is used to smoothly transition the health bar value to match the current health of the player character.
 * The interpolation speed is defined by a constant value.
 *
 * @param InDeltaTime The time elapsed since the last frame, used to ensure frame rate independent lerp speed.
 */
void APlayerCharacter::LerpHealthBar(const float& InDeltaTime)
{
	constexpr float LerpSpeed = 5.0f;
	LerpCurrentHealth = FMath::Lerp(LerpCurrentHealth, CurrentHealth, InDeltaTime * LerpSpeed);
	HealthBar->UpdateHealthBar(LerpCurrentHealth, MaxHealth);
}

/**
 * @brief Handles the movement of the player character.
 * 
 * This function is bound to the MoveAction input action. It is triggered when the player provides input for movement.
 * The function calculates the movement direction based on the input and the current rotation of the controller.
 * It then applies this movement direction to the character, causing the character to move.
 * 
 * @param Value The input action value, which is a 2D vector representing the direction and magnitude of the player's input for movement.
 */
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		 // Calculate the movement direction as a 2D vector
        FVector MovementDirection = MovementVector.Y * ForwardDirection + MovementVector.X * RightDirection;
        MovementDirection.Z = 0.f; // Zero out Z component to ensure movement is on the XY plane

        // Calculate the rotation angle based on the movement direction
        const FRotator DesiredRotation = MovementDirection.Rotation() + FRotator(0.f, -90.f, 0.f);

		// Calculate the new position of the pickup using linear interpolation
		const FRotator NewRotation = FMath::Lerp(BaseModel->GetRelativeRotation(),DesiredRotation, 1.f);

        BaseModel->SetRelativeRotation(NewRotation);
		
		// add movement 
		AddMovementInput(ForwardDirection,	MovementVector.Y);
		AddMovementInput(RightDirection,	MovementVector.X);
	}
}

/**
 * @brief This function is triggered when the Ultimate action is initiated.
 * 
 * If the current Ultimate tracker is greater than or equal to the Ultimate tracker, 
 * it checks if the Ultimate Ability Reference is valid. If it is, it resets the 
 * Current Ultimate Tracker to 0 and spawns an Ultimate Ability at the actor's current 
 * transform. If the Ultimate Ability Reference is not valid, it logs a warning message.
 */
void APlayerCharacter::Ultimate()
{
	if(CurrentUltimateTracker >= UltimateTracker)
	{
		if(UltimateAbilityRef)
		{
			CurrentUltimateTracker = 0.f;
			WidgetInstance->SetUltimateXP(UKismetMathLibrary::NormalizeToRange(CurrentUltimateTracker, 0, UltimateTracker));
			GetWorld()->SpawnActor<AUltimateAbility>(UltimateAbilityRef, this->GetActorTransform());
		}
		else
		{
			UE_LOGFMT(LogTemp, Warning, "Ultimate Ability Ref Not Valid");
		}
	}
}

/**
 * @brief Sets up the ship for the player character.
 *
 * This method loads the ship data from a data table and applies the loaded data to the player character.
 * It sets the character's movement speed, health, and ship mesh based on the loaded data.
 * It also loads and sets the ultimate ability for the ship from another data table.
 * Finally, it adds the base weapon to the ship.
 *
 * @note This method assumes that the data tables for ships and ultimate abilities are properly set up.
 */
void APlayerCharacter::SetUpShip()
{
	if (const UDataTable* ShipDataTableHardRef = ShipDataTable.LoadSynchronous())
	{
		if(const FShipsDataTable* ShipData = ShipDataTableHardRef->FindRow<FShipsDataTable>(SelectedShipName, "Ships Data Table Not set up", true))
		{
			if (!ShipData->ShipStats.IsEmpty())
			{
				for (const auto Stats : ShipData->ShipStats)
				{
					switch (Stats.StatType)
					{
					case EStatsType::Health:

						CurrentHealth = Stats.StatValue;
						MaxHealth = Stats.StatValue;
						UpdateHealth(0);
						break;

					case EStatsType::HealthRegen:

						HealthRegenRate = Stats.StatValue;
						break;

					case EStatsType::DamageModifier:

						break;

					case EStatsType::CritChanceModifier:

						break;

					case EStatsType::CritDamageModifier:

						break;

					case EStatsType::MovementSpeed:

						GetCharacterMovement()->MaxWalkSpeed = Stats.StatValue;
						break;
					}
				}
			}

			if (UStaticMesh* ShipMeshData = ShipData->ShipMesh.LoadSynchronous())
			{
				BaseModel->SetStaticMesh(ShipMeshData);
			}

			if(const UDataTable* UltimateDataTableHardRef = UltimatesDataTable.LoadSynchronous())
			{
				if (const FUltimatesDataTable* UltimatesData = UltimateDataTableHardRef->FindRow<FUltimatesDataTable>(ShipData->UltimateAbility, "Ultimate Ability Data Table Not set up", true))
				{
					UltimateTracker = UltimatesData->UltimateExperience;
					UltimateAbilityRef = UltimatesData->UltimateAbility.LoadSynchronous();
				}
			}

			AddWeapon(ShipData->BaseWeapon);
		}
	}
}

/**
 * @brief Performs a sphere trace to check for collectables in the vicinity of the player character.
 * 
 * This function performs a sphere trace from the player character's location and checks for any actors that implement the UCollectableInterface. 
 * If such an actor is found, the PickUp function of the UCollectableInterface is executed on that actor.
 */
void APlayerCharacter::CollectablePickUp()
{
	const TArray<FHitResult> SweepResult = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), PickUpRadius);
	for(FHitResult ActorsToCheck : SweepResult)
	{
		if (ActorsToCheck.GetActor())
		{
			if (ActorsToCheck.GetActor()->GetClass()->ImplementsInterface(UCollectableInterface::StaticClass()))
			{
				ICollectableInterface::Execute_PickUp(ActorsToCheck.GetActor(), this);
			}
		}
	}
}

/**
 * @brief Adds the experience from a collected orb to the player's experience tracker.
 * 
 * This function takes an FExperienceOrb object as an argument, which contains the amount of experience gained.
 * The function adds this experience to the player's ExperienceTracker. If the updated ExperienceTracker 
 * exceeds the experience required for the next level, the player's level is incremented, and the experience 
 * required for the next level is recalculated. If an UpgradeManagerRef is available, it adds 3 to the UpgradeQueManager.
 * The function also updates the player's CurrentUltimateTracker with the UltimateExperience from the FExperienceOrb.
 * 
 * @param Experience The FExperienceOrb object containing the experience gained from the collected orb.
 */
void APlayerCharacter::AddCollectable(const FExperienceOrb Experience)
{
	ExperienceTracker = FMath::RoundToInt(ExperienceTracker + Experience.Experience);
	WidgetInstance->SetCurrentXP(ExperienceTracker);
	if(ExperienceTracker >= CalculateExperienceForLevel(CurrentLevel, 100, LevelingGrowthRate))
	{
		CurrentLevel++;
		WidgetInstance->SetCurrentLevel(CurrentLevel);

		NextLevelReq = CalculateExperienceForLevel(CurrentLevel, 100, LevelingGrowthRate);
		WidgetInstance->SetNextLevelReq(NextLevelReq);

		if(UpgradeManagerRef)
		{
			UpgradeManagerRef->SetUpgradeQueManager(3);
		}
	}

	if(CurrentUltimateTracker < UltimateTracker)
	{
		CurrentUltimateTracker = FMath::Clamp(CurrentUltimateTracker + Experience.UltimateExperience, 0, UltimateTracker);
		WidgetInstance->SetUltimateXP(UKismetMathLibrary::NormalizeToRange(CurrentUltimateTracker, 0, UltimateTracker));
		UE_LOGFMT(LogTemp, Warning, "Ultimate XP: {0}", CurrentUltimateTracker);
	}
}

/**
 * @brief Calculates the experience required for a given level.
 * 
 * This function calculates the experience required to reach a certain level based on a base experience and a growth rate.
 * The formula used is: BaseExperience * (GrowthRate ^ Level)
 * 
 * @param Level The level for which the experience is being calculated.
 * @param BaseExperience The base experience that is used in the calculation.
 * @param GrowthRate The growth rate that is used in the calculation.
 * @return The experience required to reach the given level.
 */
int APlayerCharacter::CalculateExperienceForLevel(const int Level, const int BaseExperience, const float GrowthRate)
{
	if(Level <= 0)
	{
		return 0;
	}
	return FMath::RoundToInt(BaseExperience * FMath::Pow(GrowthRate, Level));
}

/**
 * @brief Adds a specified score to the player's current score.
 * 
 * This function increases the player's score by the product of the added score and the scoring modifier.
 * The new score is then logged.
 * 
 * @param AddedScore The score to be added to the player's current score.
 */
void APlayerCharacter::AddScore(const float AddedScore)
{
	Score += (AddedScore * ScoringModifier);
	WidgetInstance->SetScore(Score);
	UE_LOGFMT(LogTemp, Warning, "Score: {0}", Score);
}

/**
 * @brief Adds a weapon to the player character.
 * 
 * This function takes a weapon name as input and adds the corresponding weapon to the player character's equipped weapons.
 * The weapon data is retrieved from a data table using the weapon name. If the weapon data is found and the weapon class can be loaded,
 * a new weapon is spawned and attached to the player character.
 * 
 * @param WeaponName The name of the weapon to be added.
 */
void APlayerCharacter::AddWeapon(const FName& WeaponName)
{
	if (const UDataTable* WeaponsDataTableHardRef = WeaponsDataTable.LoadSynchronous())
	{
		if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(WeaponName, "Weapon Data Table Not set up PlayerCharacter.cpp", true))
		{
			if (UClass* WeaponSpawn = WeaponsData->Weapon.LoadSynchronous())
			{
				WeaponsEquipped.Add(WeaponName);
				AWeapons* WeaponRef = GetWorld()->SpawnActorDeferred<AWeapons>(WeaponSpawn, this->GetActorTransform());
				WeaponRef->SetPlayerCharacter(this);
				WeaponRef->FinishSpawning(this->GetActorTransform());
				const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
				WeaponRef->AttachToActor(this, AttachmentRules);

				if(WidgetInstance)
				{
					WidgetInstance->AddIcon(WeaponsData->WeaponIcon, EIconType::WeaponIcon);
				}
			}
		}
	}
}

/**
 * @brief Adds a passive ability to the player character.
 *
 * This function loads the passive ability data from a data table using the provided name. 
 * If the passive ability data is found and the passive ability class can be loaded, 
 * the passive ability is spawned in the world and attached to the player character.
 *
 * @param PassiveName The name of the passive ability to add.
 */
void APlayerCharacter::AddPassive(const FName& PassiveName)
{
	if (const UDataTable* PassivesDataTableHardRef = PassivesDataTable.LoadSynchronous())
	{
		if (const FPassivesDataTable* PassivesData = PassivesDataTableHardRef->FindRow<FPassivesDataTable>(PassiveName, "Passive Data Table Not set up PlayerCharacter.cpp", true))
		{
			if (UClass* PassiveSpawn = PassivesData->Passive.LoadSynchronous())
			{
				PassivesEquipped.Add(PassiveName);
				APassives* PassiveRef = GetWorld()->SpawnActorDeferred<APassives>(PassiveSpawn, this->GetActorTransform());
				PassiveRef->SetPlayerCharacter(this);
				PassiveRef->FinishSpawning(this->GetActorTransform());
				const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
				PassiveRef->AttachToActor(this, AttachmentRules);

				if (WidgetInstance)
				{
					WidgetInstance->AddIcon(PassivesData->PassiveIcon, EIconType::PassiveIcon);
				}
			}
		}
	}
}