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

void ABaseCharacter::UpdateMovementSpeed(const float Speed) const
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}