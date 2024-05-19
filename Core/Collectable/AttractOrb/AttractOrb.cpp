// Fill out your copyright notice in the Description page of Project Settings.


#include "AttractOrb.h"

#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"


void AAttractOrb::Tick(float DeltaTime)
{
	if (isCollected)
	{
		if (PlayerCharacter)
		{
			constexpr float LerpSpeed = 10.0f;
			// Calculate the target position towards the player character
			const FVector TargetPosition = PlayerCharacter->GetActorLocation();

			// Calculate the new position of the pickup using linear interpolation
			const FVector NewPosition = FMath::Lerp(GetActorLocation(), TargetPosition, DeltaTime * LerpSpeed);

			// Set the new position of the pickup
			SetActorLocation(NewPosition);

			if (FMath::Sqrt(FVector::DistSquared(PlayerCharacter->GetActorLocation(), this->GetActorLocation())) <= 50)
			{
				BaseModel->SetVisibility(false);
				const TSubclassOf<ACollectable> CollectableActors = ACollectable::StaticClass();
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), CollectableActors, ActorsFound);

				const TSubclassOf<AAttractOrb> ClassToRemove = AAttractOrb::StaticClass();
				ActorsFound.RemoveAll([ClassToRemove](const AActor* ActorToRemove) {return ActorToRemove && ActorToRemove->IsA(ClassToRemove); });
				if (ActorsFound.IsEmpty())
				{
					this->Destroy();
				}
				for (AActor* ActorFound : ActorsFound)
				{
					if(ActorFound)
					{
						constexpr float LerpSpeed2 = 20.0f;
						// Calculate the target position towards the player character
						const FVector TargetPosition2 = PlayerCharacter->GetActorLocation();

						// Calculate the new position of the pickup using linear interpolation
						const FVector NewPosition2 = FMath::Lerp(ActorFound->GetActorLocation(), TargetPosition2, DeltaTime * LerpSpeed2);

						// Set the new position of the pickup
						ActorFound->SetActorLocation(NewPosition2);
					}
				}
			}
		}
		else
		{
			PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		}
	}

	if (DestroyDuration <= 0)
	{
		this->Destroy();
	}
	else
	{
		DestroyDuration -= DeltaTime;
	}
}
