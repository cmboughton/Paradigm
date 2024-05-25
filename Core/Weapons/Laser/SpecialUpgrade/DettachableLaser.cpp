// Fill out your copyright notice in the Description page of Project Settings.


#include "DettachableLaser.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "Engine/DamageEvents.h"

// Sets default values
ADettachableLaser::ADettachableLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADettachableLaser::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < TriggerAmount; i++)
	{
		const FName DroneName = FName(*FString("LaserMesh").Append(FString::SanitizeFloat(i)));
		if (UStaticMeshComponent* LaserMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), DroneName))
		{
			LaserMesh->RegisterComponent();
			LaserMesh->CreationMethod = EComponentCreationMethod::Instance;
			LaserMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			LaserMesh->SetStaticMesh(LaserMeshRef);
			LaserMesh->AttachToComponent(this->RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false));
			LaserMeshes.Add(LaserMesh);
		}
	}
}

// Called every frame
void ADettachableLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ActorsHit.IsEmpty())
	{
		ApplyDamage(ActorsHit);
		ActorsHit.Empty();
	}

	const FVector StartLocation = this->GetActorLocation();
	const FRotator LaserRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
	for (int i = 0; i < TriggerAmount; i++)
	{
		const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + SweepTracker;
		const FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
		SetLaserTransform(i, FTransform(NewLaserRot, EndLocation, FVector(1, 1, 1)));

		TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
		ActorsHit.Append(ActiveActorsHit);

		if (LaserDuration > 0)
		{
			SweepTracker.Yaw += (DeltaTime * (((1 - FireRate) * 50) + 50)) / TriggerAmount;
		}
		else
		{
			LaserLengthTracker -= (DeltaTime * (((1 - FireRate) * 600) + 600)) / TriggerAmount;
		}
	}
	LaserDuration -= DeltaTime;
	if(LaserLengthTracker <= 0)
	{
		this->Destroy();
	}
}

void ADettachableLaser::SetLaserTransform(const int& MeshIndex, const FTransform& LaserTransform)
{
	LaserMeshes[MeshIndex]->SetRelativeRotation(LaserTransform.GetRotation());

	const FVector Direction = LaserTransform.GetLocation() - this->GetActorLocation();
	const float Distance = Direction.Size();
	FVector NewScale = LaserMeshes[MeshIndex]->GetComponentScale();
	NewScale.X = Distance / 10;
	LaserMeshes[MeshIndex]->SetRelativeScale3D(NewScale);
}

TArray<FHitResult> ADettachableLaser::LineTrace(const FVector& ActorStartLocation, const FVector& ActorEndLocation)
{
	const TArray<AActor*> ActorsToIgnore;
	//ActorsToIgnore.Init(PlayerCharacter, 1);
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

void ADettachableLaser::ApplyDamage(const TArray<FHitResult>& AllActorsHit)
{
	if (!AllActorsHit.IsEmpty())
	{
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
	}
}