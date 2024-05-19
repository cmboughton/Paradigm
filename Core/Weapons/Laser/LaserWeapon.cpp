// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserWeapon.h"

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
		if(SweepTracker.Yaw <= LaserRot.Yaw)
		{
			for(int i = 0; i < TriggerAmount; i++)
			{
				const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + SweepTracker;
				
				if(LaserLengthTracker < AffectRadius)
				{
					LaserLengthTracker += DeltaTime * (((1 - FireRate) * 300) + 300);
					const FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
					SetLaserTransform(i, FTransform(NewLaserRot, EndLocation, FVector(1, 1, 1)));
					const TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
					ActorsHit.Append(ActiveActorsHit);
				}
				else
				{
					const FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
					SetLaserTransform(i, FTransform(NewLaserRot, EndLocation, FVector(1, 1, 1)));
					const TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
					ActorsHit.Append(ActiveActorsHit);
					bStartSweep = true;
				}
			}
		}
		if(bStartSweep)
		{
			if(SweepTracker.Yaw <= LaserRot.Yaw)
			{
				SweepTracker.Yaw += DeltaTime * (((1 - FireRate) * 50) + 50);
			}
			else
			{
				if(LaserLengthTracker <= 0)
				{
					FireRateTracker = FireRate;
					SweepTracker.Yaw = 0.f;
					LaserLengthTracker = 0.f;
					bStartSweep = false;
					bSpawnLasers = true;
				}
				else
				{
					for (int i = 0; i < TriggerAmount; i++)
					{
						LaserLengthTracker -= DeltaTime * (((1 - FireRate) * 300) + 300);
						const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + SweepTracker;
						const FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
						SetLaserTransform(i, FTransform(NewLaserRot, EndLocation, FVector(1, 1, 1)));
						TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
						ActorsHit.Append(ActiveActorsHit);
					}
				}
			}
		}
	}
	else
	{
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
