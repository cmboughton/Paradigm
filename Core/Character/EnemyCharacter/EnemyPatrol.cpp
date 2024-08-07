// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPatrol.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

void AEnemyPatrol::BeginPlay()
{
	Super::BeginPlay();

	CurrentMoveToLoc = PatrolPoints.EndLocation;
}

void AEnemyPatrol::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetCharacterState() != ECharacterState::Death)
		{
			if (FVector::DistSquared(this->GetActorLocation(), CurrentMoveToLoc) <= 200 * 200)
			{
				if (bLocationTracker)
				{
					CurrentMoveToLoc = PatrolPoints.StartLocation;
				}
				else
				{
					CurrentMoveToLoc = PatrolPoints.EndLocation;
				}
				bLocationTracker = !bLocationTracker;
			}

			if(GetCharacterMovement() != nullptr)
			{
				GetCharacterMovement()->MaxWalkSpeed = FVector::DistSquared(this->GetActorLocation(), CurrentMoveToLoc) * MovementSpeedModifier;
			}

			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), CurrentMoveToLoc);

			if (DistanceCheck(AttackRange))
			{
				ApplyDamage(PlayerCharacter);
				EnemyDeath(false, EDeathType::Normal);
			}
		}
	}
	else
	{
		PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}