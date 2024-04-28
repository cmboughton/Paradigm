// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Paradigm_IQ/Core/Weapons/BasicProjectile/Projectile/BaseProjectile.h"
#include "../EnemyCharacter/EnemyCharacter.h"

#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

#include "../Data/DataTables/DataTables.h"
#include "../Data/Interfaces/CollectableInterface.h"
#include "../Data/Structs/Structs.h"
#include "Paradigm_IQ/Core/Weapons/Weapons.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom
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

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	BaseModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Model"));
	BaseModel->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
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

// Called every frame
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

// Called to bind functionality to input
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
			UltimateSpawned->UltimateAbilityStart();
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

			if (const UDataTable* WeaponsDataTableHardRef = WeaponsDataTable.LoadSynchronous())
			{
				if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(ShipData->BaseWeapon, "Ultimate Ability Data Table Not set up", true))
				{
					if(UClass* WeaponSpawn = WeaponsData->Weapon.LoadSynchronous())
					{
						AWeapons* WeaponRef = GetWorld()->SpawnActor<AWeapons>(WeaponSpawn, this->GetActorTransform());
						FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
						WeaponRef->AttachToActor(this,AttachmentRules);
					}
				}
				if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(ShipData->Weapon1, "Ultimate Ability Data Table Not set up", true))
				{
					if (UClass* WeaponSpawn = WeaponsData->Weapon.LoadSynchronous())
					{
						AWeapons* WeaponRef = GetWorld()->SpawnActor<AWeapons>(WeaponSpawn, this->GetActorTransform());
						FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
						WeaponRef->AttachToActor(this, AttachmentRules);
					}
				}
				if (const FWeaponsDataTable* WeaponsData = WeaponsDataTableHardRef->FindRow<FWeaponsDataTable>(ShipData->Weapon2, "Ultimate Ability Data Table Not set up", true))
				{
					if (UClass* WeaponSpawn = WeaponsData->Weapon.LoadSynchronous())
					{
						AWeapons* WeaponRef = GetWorld()->SpawnActor<AWeapons>(WeaponSpawn, this->GetActorTransform());
						FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
						WeaponRef->AttachToActor(this, AttachmentRules);
					}
				}
			}
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
	UE_LOGFMT(LogTemp, Warning, "Experience: {0}", ExperienceTracker);
	if(CurrentUltimateTracker < UltimateTracker)
	{
		CurrentUltimateTracker += Experience.UltimateExperience;
		UE_LOGFMT(LogTemp, Warning, "UltimateExperience: {0}", CurrentUltimateTracker);
	}
}

TArray<FHitResult> APlayerCharacter::SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Init(this, 1);
	TArray<FHitResult> AllActorsHit;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(),
		ActorStartLocation,
		ActorEndLocation,
		TraceRadius,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		AllActorsHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f);
	return TArray<FHitResult>(AllActorsHit);
}

void APlayerCharacter::AddScore(const float AddedScore)
{
	Score += (AddedScore * ScoringModifier);
	UE_LOGFMT(LogTemp, Warning, "Score: {0}", Score);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	ScoringModifier = 0;
	if(CurrentHealth <= 0)
	{
		Death();
	}
	return CurrentHealth;
}

void APlayerCharacter::Death()
{
	UE_LOGFMT(LogTemp, Warning, "You have Died");
}
