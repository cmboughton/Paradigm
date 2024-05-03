// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paradigm_IQ/Core/Data/Interfaces/CollectableInterface.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"
#include "Collectable.generated.h"

UCLASS()
class PARADIGM_IQ_API ACollectable : public AActor, public ICollectableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectable();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseModel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PickUp_Implementation(APlayerCharacter* PlayerCharacterRef) override;

	UFUNCTION()
	virtual void Collected(APlayerCharacter* PlayerCharacterRef);

	UFUNCTION()
	virtual void HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	float ImpulseDelayTracker = 0.8f;

	UPROPERTY()
	bool isCollected = false;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
