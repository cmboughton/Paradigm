// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFollower.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"

void AEnemyFollower::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter)
	{
		const FVector TargetLocation = PlayerCharacter->GetActorLocation();
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), TargetLocation);
		if (FVector::DistSquared(this->GetActorLocation(), PlayerCharacter->GetActorLocation()) <= AttackRange * AttackRange)
		{
			Death();
		}
	}
	else
	{
		PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}
