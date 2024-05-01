// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserWeapon.h"

void ALaserWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	ApplyDamage(ActorsHit);

	ActorsHit.Empty();
	FRotator LaserRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
	for(int i = 0; i < TriggerAmount; i++)
	{
		FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + SweepTracker;
		FVector StartLocation = this->GetActorLocation();
		if(LaserLengthTracker < AffectRadius)
		{
			LaserLengthTracker += DeltaTime * (((1 - FireRate) * 300) + 300);
			FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
			TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
			ActorsHit.Append(ActiveActorsHit);
		}
		else
		{
			FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
			TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
			ActorsHit.Append(ActiveActorsHit);
		}
		//UE_LOGFMT(LogTemp, Warning, "LaserRot: {0}", FireRate);
	}
	if(bShouldSweep)
	{
		SweepRotation.Yaw = LaserRot.Yaw;
		if(LaserLengthTracker >= AffectRadius)
		{
			if(SweepTracker.Yaw <= SweepRotation.Yaw)
			{
				SweepTracker.Yaw += DeltaTime * (((1 - FireRate) * 50) + 50);
				//UE_LOGFMT(LogTemp, Warning, "LaserRot: {0}", SweepTracker.Yaw);
			}
			else if (LaserEndDelayTracker >= .2f)
			{
				FireRateTracker = FireRate;
				SweepTracker.Yaw = 0.f;
				LaserLengthTracker = 0.f;
				LaserEndDelayTracker = 0.f;
				//UE_LOGFMT(LogTemp, Warning, "Fire Rate Tracker Set: {0}", FireRateTracker);
			}
			else
			{
				LaserEndDelayTracker += DeltaTime;
				//UE_LOGFMT(LogTemp, Warning, "Delay: {0}", LaserEndDelayTracker);
			}
		}
	}
	else
	{
		FireRateTracker = FireRate;
	}
}