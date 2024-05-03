// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Paradigm_IQ/Core/Ultimate/UltimateAbility.h"
#include "../EnemyCharacter/EnemyCharacter.h"

#include "GameFramework/Controller.h"

#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"
#include "Paradigm_IQ/Core/Data/Interfaces/CollectableInterface.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"


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


}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetUpShip();

	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
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
			ScoreModifierTracker = 0.f;
			//UE_LOGFMT(LogTemp, Warning, "Scoring Multiplier: {0}", ScoringModifier);
		}
		else
		{
			ScoreModifierTracker += DeltaTime;
			//UE_LOGFMT(LogTemp, Warning, "Scoring Tracker: {0}", ScoreModifierTracker);
		}
	}
}

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

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

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
        FRotator DesiredRotation = MovementDirection.Rotation() + FRotator(0.f, -90.f, 0.f);

		// Calculate the new position of the pickup using linear interpolation
		const FRotator NewRotation = FMath::Lerp(BaseModel->GetRelativeRotation(),DesiredRotation, .1f);

        BaseModel->SetRelativeRotation(NewRotation);
		
		// add movement 
		AddMovementInput(ForwardDirection,	MovementVector.Y);
		AddMovementInput(RightDirection,	MovementVector.X);
	}
}

void APlayerCharacter::Ultimate()
{
	if(CurrentUltimateTracker >= UltimateTracker)
	{
		if(UltimateAbilityRef)
		{
			CurrentUltimateTracker = 0.f;
			AUltimateAbility* UltimateSpawned = GetWorld()->SpawnActor<AUltimateAbility>(UltimateAbilityRef, this->GetActorTransform());
			UE_LOGFMT(LogTemp, Warning, "Ultimate Ability Ref Spawned: {0}", UltimateAbilityRef->GetName());
		}
		else
		{
			UE_LOGFMT(LogTemp, Warning, "Ultimate Ability Ref Not Valid");
		}
	}
}

void APlayerCharacter::SetUpShip()
{
	if (const UDataTable* ShipDataTableHardRef = ShipDataTable.LoadSynchronous())
	{
		if(const FShipsDataTable* ShipData = ShipDataTableHardRef->FindRow<FShipsDataTable>("BaseShip", "Ships Data Table Not set up", true))
		{
			GetCharacterMovement()->MaxWalkSpeed = ShipData->MovementSpeed;
			CurrentHealth = ShipData->Health;
			MaxHealth = ShipData->Health;

			if (UStaticMesh* ShipMeshData = ShipData->ShipMesh.LoadSynchronous())
			{
				BaseModel->SetStaticMesh(ShipMeshData);
				//UE_LOGFMT(LogTemp, Warning, "Static Mesh Set: {0}", ShipData->GetName());
			}

			if(const UDataTable* UltimateDataTableHardRef = UltimatesDataTable.LoadSynchronous())
			{
				if (const FUltimatesDataTable* UltimatesData = UltimateDataTableHardRef->FindRow<FUltimatesDataTable>(ShipData->UltimateAbility, "Ultimate Ability Data Table Not set up", true))
				{
					UltimateTracker = UltimatesData->UltimateExperience;
					//UE_LOGFMT(LogTemp, Warning, "Ultimate Tracker: {0}", UltimateTracker);
					UltimateAbilityRef = UltimatesData->UltimateAbility.LoadSynchronous();
				}
			}

			AddWeapon(ShipData->BaseWeapon);
		}
	}
}

void APlayerCharacter::CollectablePickUp()
{
	TArray<FHitResult> SweepResult = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), 200.f);
	for(FHitResult ActorsToCheck : SweepResult)
	{
		if (ActorsToCheck.GetActor())
		{
			if (ActorsToCheck.GetActor()->GetClass()->ImplementsInterface(UCollectableInterface::StaticClass()))
			{
				ICollectableInterface::Execute_PickUp(ActorsToCheck.GetActor(), this);
				//UE_LOGFMT(LogTemp, Warning, "HasInterface: {0}", ActorsToCheck.GetActor()->GetName());
			}
		}
	}
}

void APlayerCharacter::AddCollectable(struct FExperienceOrb Experience)
{
	ExperienceTracker += Experience.Experience;
	//UE_LOGFMT(LogTemp, Warning, "Experience: {0}", ExperienceTracker);
	if(CurrentUltimateTracker < UltimateTracker)
	{
		CurrentUltimateTracker += Experience.UltimateExperience;
		//UE_LOGFMT(LogTemp, Warning, "UltimateExperience: {0}", CurrentUltimateTracker);
	}
}

void APlayerCharacter::AddScore(const float AddedScore)
{
	Score += (AddedScore * ScoringModifier);
	UE_LOGFMT(LogTemp, Warning, "Score: {0}", Score);
}

void APlayerCharacter::AddWeapon(const FName WeaponName)
{
	if (const UDataTable* WeaponsDataTableHardRef = WeaponsDataTable.LoadSynchronous())
	{
		if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(WeaponName, "Weapon Data Table Not set up PlayerCharacter.cpp", true))
		{
			if (UClass* WeaponSpawn = WeaponsData->Weapon.LoadSynchronous())
			{
				WeaponsEquipped.Add(WeaponName);
				AWeapons* WeaponRef = GetWorld()->SpawnActor<AWeapons>(WeaponSpawn, this->GetActorTransform());
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
				WeaponRef->AttachToActor(this, AttachmentRules);
			}
		}
	}
}
