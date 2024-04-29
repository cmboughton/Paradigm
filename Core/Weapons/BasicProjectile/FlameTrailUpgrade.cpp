// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameTrailUpgrade.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "Engine/DamageEvents.h"

// Sets default values
AFlameTrailUpgrade::AFlameTrailUpgrade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlameTrailUpgrade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlameTrailUpgrade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	//ActorsToIgnore.Init(PlayerCharacter, 1);
	TArray<FHitResult> AllActorsHit;
	UKismetSystemLibrary::SphereTraceMulti(	GetWorld(),
											StartLocation,
											EndLocation,
											AffectRadius,
											ETraceTypeQuery::TraceTypeQuery1,
											false,
											ActorsToIgnore,
											EDrawDebugTrace::ForOneFrame,
											AllActorsHit,
											true,
											FLinearColor::Red,
											FLinearColor::Green,
											2.f);

	for (FHitResult ActorHit : AllActorsHit)
	{
		if (ActorHit.GetActor())
		{
			if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
			{
				AActor* EnemyHit = ActorHit.GetActor();
				const FVector ActorLocation = ActorHit.GetActor()->GetActorLocation();
				const FPointDamageEvent DamageEvent(Damage, ActorHit, ActorLocation, nullptr);
				EnemyHit->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
			}
		}
	}

	if(DestroyDuration <= 0)
	{
		this->Destroy();
	}
	else
	{
		DestroyDuration -= DeltaTime;
	}
}

