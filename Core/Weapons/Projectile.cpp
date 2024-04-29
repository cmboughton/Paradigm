// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Projectile Movement Component
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->ProjectileGravityScale = GravityScale;
	MovementComponent->InitialSpeed = ProjectileSpeed;

	//Static Mesh
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("BulletMesh"));
	SetRootComponent(BulletMesh);
	BulletMesh->CastShadow = false;
	BulletMesh->SetMassOverrideInKg("None", MeshMass);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->MaxSpeed = ProjectileSpeed;

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if(bIsArcImpulse)
	{
		BulletMesh->SetSimulatePhysics(true);
		FVector ArcImpulse = FVector(FMath::RandRange(-Impulse, Impulse), FMath::RandRange(-Impulse, Impulse), FMath::RandRange(Impulse, Impulse * 2));
		BulletMesh->AddImpulse(ArcImpulse);
	}

	if(bAutoSizeMesh)
	{
		BulletMesh->SetRelativeScale3D(FVector(AffectRadius / 180, AffectRadius / 180, AffectRadius / 180));
	}
}


float AProjectile::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(bShouldTakeDamage)
	{
		Damage -= DamageAmount;
		if (Damage <= 0)
		{
			DestroyProjectile();
		}
	}
	return 0.0f;
}

void AProjectile::DestroyProjectile()
{
	this->Destroy();
}

TArray<FHitResult> AProjectile::SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerCharacter);
	ActorsToIgnore.Add(this);
	//ActorsToIgnore.Init(PlayerCharacter, 1);
	TArray<FHitResult> AllActorsHit;
	UKismetSystemLibrary::SphereTraceMulti(	GetWorld(),
											ActorStartLocation,
											ActorEndLocation,
											TraceRadius,
											ETraceTypeQuery::TraceTypeQuery1,
											false,
											ActorsToIgnore,
											EDrawDebugTrace::ForDuration,
											AllActorsHit,
											true,
											FLinearColor::Red,
											FLinearColor::Green,
											.2f);
	return TArray<FHitResult>(AllActorsHit);
}

void AProjectile::ApplyDamage(const TArray<FHitResult> AllActorsHit)
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

void AProjectile::TraceCheck(const float DeltaTime)
{
	ActorsHit.Empty();
	ActorsHit = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), AffectRadius);
}


// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceCheck(DeltaTime);


	if (DestroyDuration <= 0)
	{
		DestroyProjectile();
	}
	else
	{
		DestroyDuration -= DeltaTime;
	}
}

