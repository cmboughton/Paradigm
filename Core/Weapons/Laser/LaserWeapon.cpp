// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserWeapon.h"

#include "SpecialUpgrade/DettachableLaser.h"

void ALaserWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	if(bSpawnLasers || LaserMeshes.Num() < TriggerAmount)
	{
		bSpawnLasers = false;
		if(!LaserMeshes.IsEmpty())
		{
			for (const auto LaserMesh: LaserMeshes)
			{
				LaserMesh->DestroyComponent();
			}
		}
		LaserMeshes.Empty();
		for(int i = 0; i < TriggerAmount; i++)
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
	if(!ActorsHit.IsEmpty())
	{
		ApplyDamage(ActorsHit);
		ActorsHit.Empty(); 
	}
	if(!bSpecialUpgrade1)
	{
		const FVector StartLocation = this->GetActorLocation();
		const FRotator LaserRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
		for(int i = 0; i < TriggerAmount; i++)
		{
			const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + SweepTracker;
			const FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
			SetLaserTransform(i, FTransform(NewLaserRot, EndLocation, FVector(1, 1, 1)));

			TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
			ActorsHit.Append(ActiveActorsHit);

			if(LaserLengthTracker < AffectRadius && !bIsRetracting)
			{
				LaserLengthTracker += (DeltaTime * (((1 - FireRate) * 600) + 600)) / TriggerAmount;

			}
			else if(SweepTracker.Yaw <= LaserRot.Yaw)
			{
				if (bSpecialUpgrade2)
				{
					if (bRollDetachable)
					{
						bRollDetachable = false;
						if (const float Roll = FMath::RandRange(0, 100) <= DetachableChance)
						{
							SpawnDetachableLasers();
							SweepTracker.Yaw = LaserRot.Yaw;
						}
					}
				}
				SweepTracker.Yaw += (DeltaTime * (((1 - FireRate) * 50) + 50)) / TriggerAmount;
			}
			else
			{
				bIsRetracting = true;
				LaserLengthTracker -= (DeltaTime * (((1 - FireRate) * 600) + 600)) / TriggerAmount;
				if (LaserLengthTracker <= 0)
				{
					FireRateTracker = FireRate;
					SweepTracker.Yaw = 0.f;
					LaserLengthTracker = 0.f;
					bSpawnLasers = true;
					bIsRetracting = false;
					bRollDetachable = true;
					return;
				}
			}
		}
	}
	else
	{
		if (bSpecialUpgrade2)
		{
			DetachableLaserTracker -= DeltaTime;
			if(DetachableLaserTracker <= 0)
			{
				DetachableLaserTracker = FireRate * 3;
				bRollDetachable = true;
			}
			if (bRollDetachable)
			{
				bRollDetachable = false;
				const float Roll = FMath::RandRange(0, 100);
				UE_LOGFMT(LogTemp, Warning, "Roll: {0}", Roll);
				if (Roll <= DetachableChance)
				{
					SpawnDetachableLasers();
				}
			}
		}
		const FVector StartLocation = this->GetActorLocation();
		const FRotator LaserRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
		for (int i = 0; i < TriggerAmount; i++)
		{
			const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + SweepTracker;
			const FVector EndLocation = StartLocation + NewLaserRot.Vector() * AffectRadius;
			SetLaserTransform(i,FTransform(NewLaserRot, EndLocation, FVector(1, 1, 1)));
			const TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
			ActorsHit.Append(ActiveActorsHit);
		}
		SweepTracker.Yaw += DeltaTime * (((1 - FireRate) * 50) + 50);
		SweepTracker.Normalize();
	}
}

void ALaserWeapon::SetLaserTransform(const int& MeshIndex, const FTransform& LaserTransform)
{
	LaserMeshes[MeshIndex]->SetRelativeRotation(LaserTransform.GetRotation());

	const FVector Direction = LaserTransform.GetLocation() - this->GetActorLocation();
	const float Distance = Direction.Size();
	FVector NewScale = LaserMeshes[MeshIndex]->GetComponentScale();
	NewScale.X = Distance / 10;
	LaserMeshes[MeshIndex]->SetRelativeScale3D(NewScale);
}

void ALaserWeapon::SpawnDetachableLasers() const
{
	if (DetachableLaserRef)
	{
		ADettachableLaser* DetachLaser = GetWorld()->SpawnActorDeferred<ADettachableLaser>(DetachableLaserRef, this->GetActorTransform());
		DetachLaser->SetDamage(Damage);
		DetachLaser->SetFireRate(FireRate);
		DetachLaser->SetLaserLength(AffectRadius);
		DetachLaser->SetTriggerAmount(TriggerAmount);
		DetachLaser->FinishSpawning(this->GetActorTransform());
	}
}
