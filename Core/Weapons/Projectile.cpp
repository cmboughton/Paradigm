// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"



AProjectile::AProjectile()
{
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

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->MaxSpeed = ProjectileSpeed;

	if(bIsArcImpulse)
	{
		BulletMesh->SetSimulatePhysics(true);
		const FVector ArcImpulse = FVector(FMath::RandRange(-Impulse, Impulse), FMath::RandRange(-Impulse, Impulse), FMath::RandRange(Impulse, Impulse * 2));
		BulletMesh->AddImpulse(ArcImpulse);
	}

	if(bAutoSizeMesh)
	{
		BulletMesh->SetRelativeScale3D(FVector(AffectRadius / 180, AffectRadius / 180, AffectRadius / 180));
	}
}

/**
 * @brief This function is used to apply damage to the projectile.
 * 
 * @param DamageAmount The amount of damage to be applied to the projectile.
 * @param DamageEvent The event that describes the cause of the damage.
 * @param EventInstigator The controller that was responsible for causing the damage.
 * @param DamageCauser The actor that directly caused the damage.
 * 
 * @return Returns 0.0f after applying the damage.
 * 
 * If the projectile is set to take damage, it will subtract the DamageAmount from the projectile's current damage.
 * If the damage is less than or equal to zero after the subtraction, it will destroy the projectile.
 */
float AProjectile::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

/**
 * @brief Destroys the projectile instance.
 *
 * This method is responsible for destroying the current instance of the projectile.
 * It is typically called when the projectile has reached its lifespan or has hit a target.
 */
void AProjectile::DestroyProjectile()
{
	this->Destroy();
}

/**
 * @brief Performs a sphere trace from the start location to the end location with a specified radius.
 *
 * This function performs a sphere trace in the world, ignoring specific actors, and returns an array of hit results.
 * The trace is performed from the start location to the end location with a specified radius. The trace ignores the player character and the projectile itself.
 * The trace type is set to TraceTypeQuery1, and the trace is visible for one frame. The trace hits are displayed in red and green.
 * The function returns an array of all actors hit by the sphere trace.
 *
 * @param ActorStartLocation The start location of the sphere trace.
 * @param ActorEndLocation The end location of the sphere trace.
 * @param TraceRadius The radius of the sphere trace.
 * @return TArray<FHitResult> An array of hit results from the sphere trace.
 */
TArray<FHitResult> AProjectile::SphereTrace(const FVector& ActorStartLocation, const FVector& ActorEndLocation, const float& TraceRadius)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Init(PlayerCharacter, 1);
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
											EDrawDebugTrace::ForOneFrame,
											AllActorsHit,
											true,
											FLinearColor::Red,
											FLinearColor::Green,
											.2f);
	return TArray<FHitResult>(AllActorsHit);
}

/**
 * @brief Applies damage to all actors hit.
 *
 * This function iterates over all actors hit and applies damage to those that implement the UEnemyInterface.
 * The damage is applied using the TakeDamage function of the actor hit.
 *
 * @param AllActorsHit A reference to an array of FHitResult representing all actors hit.
 */
void AProjectile::ApplyDamage(const TArray<FHitResult>& AllActorsHit)
{
	if(!AllActorsHit.IsEmpty())
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

/**
 * @brief Performs a trace check for the projectile.
 * 
 * This method is responsible for performing a trace check for the projectile. It clears the list of actors hit by the projectile,
 * performs a sphere trace from the current location of the projectile, and stores the hit results in the ActorsHit list.
 * 
 * @param DeltaTime The time elapsed since the last frame.
 */
void AProjectile::TraceCheck(const float& DeltaTime)
{
	ActorsHit.Empty();
	ActorsHit = SphereTrace(this->GetActorLocation(), this->GetActorLocation(), AffectRadius);
}

void AProjectile::Tick(const float DeltaTime)
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