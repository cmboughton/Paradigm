// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShipRenderer.generated.h"

UCLASS()
class PARADIGM_IQ_API AShipRenderer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShipRenderer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseModel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetShipMesh(const FName RowName);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Varaibles|DataTables", meta = (ToolTip = "The Data Table that holds the data of the ships."))
	TSoftObjectPtr<UDataTable> ShipDataTable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
