// Fill out your copyright notice in the Description page of Project Settings.


#include "PlasmaCannon.h"

#include "Kismet/KismetMathLibrary.h"

void APlasmaCannon::WeaponTriggered(const float DeltaTime)
{
	if(PlasmaDelay <= 0)
	{
		bIsPlasmaDelay = true;
		PlasmaDelay = 0.5f;
		FireRateTracker = FireRate;
	}
	else
	{
		StartLocation = GetActorLocation();
		PlasmaDelay -= DeltaTime;

		if(TriggerAmount == 1)
		{
			if(bIsPlasmaDelay)
			{
				SelectedEnemy = FindClosestEnemy();

				if (SelectedEnemy)
				{
					RotationalDirection = FRotationMatrix::MakeFromX(SelectedEnemy->GetActorLocation() - this->GetActorLocation()).Rotator();
				}

				RandomRot = FRotator(0, FMath::RandRange(0, 360), 0);

				bIsPlasmaDelay = false;
			}

			EndLocation = StartLocation + (UKismetMathLibrary::GetForwardVector(FRotator(0.f, RotationalDirection.Yaw, 0.f)) * (AffectRadius * 20));

			if (SelectedEnemy)
			{
				SphereTraceDamage(EndLocation);
			}
			else
			{
				const FVector EndLocationRan = StartLocation + (UKismetMathLibrary::GetForwardVector(RandomRot) * (AffectRadius * 20));
				SphereTraceDamage(EndLocationRan);
			}
		}
		else
		{
			if (bIsPlasmaDelay)
			{
				SelectedEnemies = FindClosestEnemies(1000);

				if (!SelectedEnemies.IsEmpty())
				{
					RotationalDirections.Empty();
					for(int i = 0; i < TriggerAmount; i++)
					{
						if(SelectedEnemies.IsValidIndex(i))
						{
							RotationalDirections.Add(FRotationMatrix::MakeFromX(SelectedEnemies[i]->GetActorLocation() - this->GetActorLocation()).Rotator());
						}
					}
				}
				RandomRots.Empty();
				for(int i = 0; i < TriggerAmount; i++)
				{
					RandomRots.Add(FRotator(0, FMath::RandRange(0, 360), 0)); 
				}
				bIsPlasmaDelay = false;
			}

			for(int i = 0; i < TriggerAmount; i++)
			{
				if(RotationalDirections.IsValidIndex(i))
				{
					EndLocation = StartLocation + (UKismetMathLibrary::GetForwardVector(FRotator(0.f, RotationalDirections[i].Yaw, 0.f)) * (AffectRadius * 20));
				}

				if (SelectedEnemies.IsValidIndex(i))
				{
					SphereTraceDamage(EndLocation);
				}
				else
				{
					FVector EndLocationRan;
					if(RandomRots.IsValidIndex(i))
					{
						EndLocationRan = StartLocation + (UKismetMathLibrary::GetForwardVector(RandomRots[i]) * (AffectRadius * 20));
					}
					else
					{
						EndLocationRan = StartLocation + (UKismetMathLibrary::GetForwardVector(FRotator(0, 25, 0)) * (AffectRadius * 20));
					}
					SphereTraceDamage(EndLocationRan);
				}
			}
		}
	}
}

void APlasmaCannon::SphereTraceDamage(FVector EndLoc)
{
	TArray<FHitResult> AllActorsHit = SphereTrace(StartLocation, EndLoc, AffectRadius);
	if (!AllActorsHit.IsEmpty())
	{
		for (FHitResult ActorHit : AllActorsHit)
		{
			if (ActorHit.GetActor())
			{
				if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
				{
					//UE_LOGFMT(LogTemp, Warning, "Enemy Targeted: {0}", ActorHit.GetActor()->GetName());
					ApplyDamage(ActorHit);
				}
			}
		}
	}
}
