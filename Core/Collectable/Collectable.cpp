// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"

#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


// Sets default values
ACollectable::ACollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Model"));
	SetRootComponent(BaseModel);
	BaseModel->CastShadow = false;

}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	BaseModel->OnComponentHit.AddDynamic(this, &ACollectable::HitMesh);

	BaseModel->SetSimulatePhysics(true);
	FVector Impulse = FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), FMath::RandRange(50, 100));
	BaseModel->AddImpulse(Impulse);

}

void ACollectable::PickUp_Implementation(APlayerCharacter* PlayerCharacterRef)
{
	isCollected = true;
	BaseModel->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
}

void ACollectable::Collected(APlayerCharacter* PlayerCharacterRef)
{
}

void ACollectable::HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BaseModel->SetSimulatePhysics(false);
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(isCollected)
	{
		if(PlayerCharacter)
		{
			constexpr float LerpSpeed = 10.0f;
			// Calculate the target position towards the player character
			const FVector TargetPosition = PlayerCharacter->GetActorLocation();

			// Calculate the new position of the pickup using linear interpolation
			const FVector NewPosition = FMath::Lerp(GetActorLocation(), TargetPosition, DeltaTime * LerpSpeed);

			// Set the new position of the pickup
			SetActorLocation(NewPosition);

			if (FMath::Sqrt(FVector::DistSquared(PlayerCharacter->GetActorLocation(), this->GetActorLocation())) <= 100)
			{
				Collected(PlayerCharacter);
				this->Destroy();
			}
		}
		else
		{
			PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		}
	}
}
