// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Logging/StructuredLog.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Collectable/Experience/Experience.h"
#include "Paradigm_IQ/Core/Collectable/WeaponUpgrade/WeaponUpgradeCollectable.h"


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

	CurrentHealth = Health;

}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(DamageImmunity <= 0)
	{
		CurrentHealth = DealDamage(CurrentHealth, DamageAmount);
		//UE_LOGFMT(LogTemp, Warning, "Remaing Health: {0}", CurrentHealth);
		if (DamageCauser)
		{
			float DamageDoneBack = 0.f;
			if(CurrentHealth > 0)
			{
				DamageDoneBack = DamageAmount;
			}
			else
			{
				DamageDoneBack = DamageAmount + CurrentHealth;
			}
			const FHitResult Hit;
			const FVector ActorLocation = this->GetActorLocation();
			const FPointDamageEvent DamageBackEvent(DamageDoneBack, Hit, ActorLocation, nullptr);
			DamageCauser->TakeDamage(DamageDoneBack, DamageBackEvent, GetInstigatorController(), this);
			//UE_LOGFMT(LogTemp, Warning, "Damage Done Back: {0}", DamageDoneBack);
		}
		if (CurrentHealth <= 0)
		{
			Death();
		}
		DamageImmunity = 0.1f;
	}

	return CurrentHealth;
}

float AEnemyCharacter::DealDamage(const float InHealth, const float DamageValue)
{
	return InHealth - DamageValue;
}

void AEnemyCharacter::Death()
{
	if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
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
	if(UpgradeDropChance  >= FMath::RandRange(0.f, 1.f))
	{
		if(WeaponUpgradeCollectable)
		{
			FTransform XPSpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), this->GetActorLocation() + FVector(0.f, 0.f, 100.f), FVector(1.f, 1.f, 1.f));
			GetWorld()->SpawnActor<AWeaponUpgradeCollectable>(WeaponUpgradeCollectable, XPSpawnTransform);
		}
	}
	this->Destroy();
}

void AEnemyCharacter::ChangeCharacterState_Implementation(const ECharacterState CharacterState)
{
	CurrentCharacterState = CharacterState;
	UE_LOGFMT(LogTemp, Warning, "Character State");
	Death();
}

// Called every frame
void AEnemyCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(DamageImmunity > 0)
	{
		DamageImmunity -= DeltaTime;
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

