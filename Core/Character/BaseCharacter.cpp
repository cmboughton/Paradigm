// Fill out your copyright notice in the Description page of Project Settings.


#include "Paradigm_IQ/Core/Character/BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Model"));
	BaseModel->SetupAttachment(RootComponent);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DamageImmunity > 0)
	{
		DamageImmunity -= DeltaTime;
	}
}

/**
 * @brief Performs a sphere trace from the start location to the end location with a specified radius.
 * 
 * This function performs a sphere trace in the world, ignoring the actor that calls this function. It returns an array of all actors hit by the trace.
 * 
 * @param ActorStartLocation The starting location of the trace.
 * @param ActorEndLocation The ending location of the trace.
 * @param TraceRadius The radius of the sphere trace.
 * 
 * @return Returns an array of FHitResult, each containing information about each actor hit by the trace.
 */
TArray<FHitResult> ABaseCharacter::SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius)
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

/**
 * @brief Applies damage to the character and updates its health and state.
 *
 * This function applies the specified amount of damage to the character. If the character is not invulnerable and its damage immunity is not active, the damage amount is subtracted from the character's current health. If the character's health drops to zero or below, the Death function is called. After applying damage, the character's damage immunity is set to a small positive value.
 *
 * @param DamageAmount The amount of damage to apply to the character.
 * @param DamageEvent The damage event that caused the damage.
 * @param EventInstigator The controller that was responsible for causing the damage.
 * @param DamageCauser The actor that caused the damage.
 * @return The character's current health after applying the damage.
 */
float ABaseCharacter::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageImmunity <= 0)
	{
		if (CurrentState != ECharacterState::Invulnerable)
		{
			CurrentHealth -= DamageAmount;
			ScoringModifier = 0;
			if (CurrentHealth <= 0)
			{
				Death();
			}
		}
		DamageImmunity = 0.1f;
	}
	return CurrentHealth;
}

void ABaseCharacter::Death()
{
	CurrentState = ECharacterState::Death;
	UE_LOGFMT(LogTemp, Warning, "{0} has Died.", this->GetName());
}

/**
 * @brief Spawns an actor in the game world.
 * 
 * This method is used to spawn an actor of a specified class at a specified transform location in the game world. 
 * It is typically used for spawning collectables, enemies, or other game objects dynamically during gameplay.
 * 
 * @param ClassToSpawn The class of the actor to be spawned. This should be a subclass of AActor.
 * @param SpawnTransform The transform (location, rotation, and scale) at which to spawn the actor.
 */
void ABaseCharacter::SpawnActor(const TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform) const
{
	GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTransform);
}

void ABaseCharacter::UpdateHealth(const float& HealthIn)
{
	if(CurrentHealth < MaxHealth)
	{
		if(CurrentHealth + HealthIn >= MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
		else
		{
			CurrentHealth += HealthIn;
		}
	}
	UE_LOGFMT(LogTemp, Warning, "HealthIn: {0}", HealthIn);
}

/**
 * @brief Updates the character's movement speed.
 * 
 * This function sets the character's maximum walking speed to the given speed.
 * 
 * @param Speed The new speed to set for the character's movement.
 */
void ABaseCharacter::UpdateMovementSpeed(const float Speed) const
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}