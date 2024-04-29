// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateAbility.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AUltimateAbility::AUltimateAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUltimateAbility::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (const UDataTable* UltimatesDataTableHardRef = UltimatesDataTable.LoadSynchronous())
	{
		if (const FUltimatesDataTable* UltimatesData = UltimatesDataTableHardRef->FindRow<FUltimatesDataTable>(UltimateName, "Ultimates Data Table Not set up", true))
		{
			Damage = UltimatesData->Damage;
			CharacterState = UltimatesData->PlayerState;
			EnemyState = UltimatesData->EnemyState;
			Duration = UltimatesData->Duration;
			//UE_LOGFMT(LogTemp, Warning, "FireRate: {0}", FireRate);
		}
	}

	UltimateAbilityStart();
}

void AUltimateAbility::UltimateAbilityStart()
{
	if(CharacterState != ECharacterState::Normal)
	{
		PlayerCharacter->SetCharacterState(CharacterState);
	}
	UltimateAbilityStartBP();
}

void AUltimateAbility::UltimateAbilityFinish()
{
	if(CharacterState != ECharacterState::Normal)
	{
		PlayerCharacter->SetCharacterState(ECharacterState::Normal);
	}
	UltimateAbilityFinishBP();
	this->Destroy();
}

// Called every frame
void AUltimateAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Duration > 0)
	{
		Duration -= DeltaTime;
	}
	else if (Duration < 0)
	{
		UltimateAbilityFinish();
	}
}

TArray<FHitResult> AUltimateAbility::SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius)
{
	TArray<AActor*> ActorsToIgnore;
	//ActorsToIgnore.Init(PlayerCharacter, 1);
	TArray<FHitResult> AllActorsHit;
	UKismetSystemLibrary::SphereTraceMulti(	GetWorld(),
											ActorStartLocation,
											ActorEndLocation,
											TraceRadius,
											ETraceTypeQuery::TraceTypeQuery1,
											false,
											ActorsToIgnore,
											EDrawDebugTrace::ForOneFrame,
											AllActorsHit,
											true,
											FLinearColor::Red,
											FLinearColor::Green,
											5.f);
	return TArray<FHitResult>(AllActorsHit);
}

TArray<FHitResult> AUltimateAbility::LineTrace(const FVector ActorStartLocation, const FVector ActorEndLocation)
{
	TArray<AActor*> ActorsToIgnore;
	/*ActorsToIgnore.Init(PlayerCharacter, 1);*/
	TArray<FHitResult> AllActorsHit;
	UKismetSystemLibrary::LineTraceMulti(	GetWorld(),
											ActorStartLocation,
											ActorEndLocation,
											ETraceTypeQuery::TraceTypeQuery1,
											false,
											ActorsToIgnore,
											EDrawDebugTrace::ForOneFrame,
											AllActorsHit,
											true,
											FLinearColor::Red,
											FLinearColor::Green,
											5.f);
	return AllActorsHit;
}

void AUltimateAbility::ApplyDamage(const FHitResult ActorHit)
{
	AActor* EnemyHit = ActorHit.GetActor();
	const FVector ActorLocation = ActorHit.GetActor()->GetActorLocation();
	const FPointDamageEvent DamageEvent(Damage, ActorHit, ActorLocation, nullptr);
	EnemyHit->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
}
