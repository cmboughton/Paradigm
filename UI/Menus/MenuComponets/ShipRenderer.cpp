// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipRenderer.h"
#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"

// Sets default values
AShipRenderer::AShipRenderer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Model"));
	BaseModel->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShipRenderer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShipRenderer::SetShipMesh(const FName RowName) const
{
	if (const UDataTable* ShipDataTableHardRef = ShipDataTable.LoadSynchronous())
	{
		if (const FShipsDataTable* ShipData = ShipDataTableHardRef->FindRow<FShipsDataTable>(RowName, "Ships Data Table Not set up", true))
		{
			if (UStaticMesh* ShipMeshData = ShipData->ShipMesh.LoadSynchronous())
			{
				BaseModel->SetStaticMesh(ShipMeshData);
				//UE_LOGFMT(LogTemp, Warning, "Static Mesh Set: {0}", ShipData->GetName());
			}
		}
	}
}

// Called every frame
void AShipRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

