// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Collectable/Experience/Experience.h"
#include "Paradigm_IQ/Core/Collectable/WeaponUpgrade/WeaponUpgradeCollectable.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AEnemyCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector TargetLocation = PlayerCharacter->GetActorLocation();
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), TargetLocation);
	if(FVector::DistSquared(this->GetActorLocation(), PlayerCharacter->GetActorLocation()) <= AttackRange * AttackRange)
	{
		Death();
	}
}

/**
 * @brief This method is used to apply damage to the enemy character.
 * 
 * @param DamageAmount The amount of damage to be applied.
 * @param DamageEvent The event that describes the damage.
 * @param EventInstigator The controller that was responsible for causing this damage.
 * @param DamageCauser The actor that caused the damage.
 * 
 * @return The current health of the enemy character after the damage has been applied.
 * 
 * If the enemy character is currently immune to damage or in an invulnerable state, 
 * the method will return the current health without applying any damage. 
 * Otherwise, the damage amount is subtracted from the current health. 
 * If the damage causes the health to drop to or below zero, the Death method is called. 
 * After applying the damage, the damage immunity is set to a small positive value.
 */
float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (DamageImmunity > 0 || CurrentState == ECharacterState::Invulnerable)
    {
        return CurrentHealth;
    }
    CurrentHealth -= DamageAmount;
	UE_LOGFMT(LogTemp, Warning, "CurrentHealth: {0} DamageAmount: {1}", CurrentHealth, DamageAmount);
    if (DamageCauser)
    {
        ApplyBackDamage(DamageAmount, DamageCauser);
    }
    if (CurrentHealth <= 0)
    {
        Death();
    }
    DamageImmunity = 0.1f;
    return CurrentHealth;
}

/**
 * @brief Applies back damage to the enemy character.
 *
 * This method calculates the damage done back to the enemy character and applies it. 
 * The damage done back is the amount of damage that was applied the enemy character.
 * The damage is then applied to the damage causer.
 *
 * @param DamageAmount The amount of damage to apply.
 * @param DamageCauser The actor causing the damage.
 */
void AEnemyCharacter::ApplyBackDamage(float DamageAmount, AActor* DamageCauser)
{
    float DamageDoneBack = (CurrentHealth > 0) ? DamageAmount : DamageAmount + CurrentHealth;
    const FHitResult Hit;
    const FVector ActorLocation = this->GetActorLocation();
    const FPointDamageEvent DamageBackEvent(DamageDoneBack, Hit, ActorLocation, nullptr);
    DamageCauser->TakeDamage(DamageDoneBack, DamageBackEvent, GetInstigatorController(), this);
}

/**
 * @brief Handles the death of the enemy character.
 *
 * This method is called when the enemy character dies. It performs several actions:
 * - Calls the base class's Death method.
 * - Adds the enemy's score to the player character's score.
 * - Drops experience orbs based on a random number.
 * - If the drop chance is met, drops a collectable item based on a weighted random roll.
 * - Destroys the enemy character.
 */
void AEnemyCharacter::Death()
{
	Super::Death();

	if(PlayerCharacter)
	{
		PlayerCharacter->AddScore(Score);
	}
	const int RandExpOrbs = FMath::RandRange(1, 3);
	ExperienceStruct.Experience = ExperienceStruct.Experience / RandExpOrbs;
	ExperienceStruct.UltimateExperience = ExperienceStruct.UltimateExperience / RandExpOrbs;
	//UE_LOGFMT(LogTemp, Warning, "RandExpOrbs: {0}", RandExpOrbs);

	for (int i = 0; i < RandExpOrbs; i++)
	{
		if(ExperienceOrb)
		{
			FTransform XPSpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), this->GetActorLocation() + FVector(0.f, 0.f, 100.f), FVector(1.f, 1.f, 1.f));
			AExperience* ExperienceOrbSpawn = GetWorld()->SpawnActorDeferred<AExperience>(ExperienceOrb, XPSpawnTransform);
			ExperienceOrbSpawn->SetUp(ExperienceStruct);
			ExperienceOrbSpawn->FinishSpawning(XPSpawnTransform);
			//UE_LOGFMT(LogTemp, Warning, "Actor Spawned: {0}", ExperienceOrbSpawn->GetName());
		}
	}
	if(CollectableLootTable.DropChance >= FMath::RandRange(0.f, 1.f))
	{
		float RollRange = 0;
		float CurrentRollTracker = 0.f;
		for (const FCollectableRoll Collectable : CollectableLootTable.Collectables)
		{
			RollRange += Collectable.RollWeight;
		}

		const float Roll = FMath::RandRange(0.f, RollRange);

		for (const FCollectableRoll Collectable : CollectableLootTable.Collectables)
		{
			CurrentRollTracker += Collectable.RollWeight;
			if (Roll <= CurrentRollTracker && Roll > CurrentRollTracker - Collectable.RollWeight)
			{
				if (TSubclassOf<ACollectable> CollectableRef = Collectable.Collectable.LoadSynchronous())
				{
					FTransform CollectableSpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), this->GetActorLocation() + FVector(0.f, 0.f, 100.f), FVector(1.f, 1.f, 1.f));
					SpawnActor(CollectableRef, CollectableSpawnTransform);
				}
			}
		}
	}
	this->Destroy();
}

void AEnemyCharacter::UpdateStats(const float& GrowthModifier)
{
	CurrentHealth *= (1 + GrowthModifier);
	MaxHealth *= (1 + GrowthModifier);
	Score *= (1 + GrowthModifier);
	UpdateMovementSpeed(GetCharacterMovement()->MaxWalkSpeed * (1 + GrowthModifier));
	Damage *= (1 + GrowthModifier);
	ExperienceStruct.Experience *= (1 + GrowthModifier);
	ExperienceStruct.UltimateExperience *= (1 + GrowthModifier);
	CollectableLootTable.DropChance *= (1 + GrowthModifier);
}

void AEnemyCharacter::ChangeCharacterState_Implementation(const ECharacterState CharacterState)
{
	CurrentState = CharacterState;
	UE_LOGFMT(LogTemp, Warning, "Character State");
	Death();
}
