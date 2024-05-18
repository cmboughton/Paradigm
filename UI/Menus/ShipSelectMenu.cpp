// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipSelectMenu.h"

#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"
#include "Paradigm_IQ/UI/Menus/MenuComponets/ShipSelect.h"
#include "Components/UniformGridPanel.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "MenuComponets/ShipRenderer.h"
#include "Paradigm_IQ/Core/Game/MainGameMode.h"
#include "Components/Button.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/Game/MainGameInstance.h"

struct FShipsDataTable;


void UShipSelectMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddDynamic(this, &UShipSelectMenu::OnButtonClicked);

	if (const UDataTable* ShipDataTableHardRef = ShipDataTable.LoadSynchronous())
	{
		int RowTracker = -1;
		int ColumnTracker = 0;
		TArray<FName> Ships = ShipDataTableHardRef->GetRowNames();
		for (int i = 0; i < Ships.Num(); i++)
		{

			if (const FShipsDataTable* ShipData = ShipDataTableHardRef->FindRow<FShipsDataTable>(Ships[i], "Ships Data Table Not set up", true))
			{
				if (ShipSelectRef)
				{
					if (UShipSelect* WidgetInstance = CreateWidget<UShipSelect>(GetWorld(), ShipSelectRef))
					{
						if (i % 4 == 0)
						{
							RowTracker++;
							ColumnTracker = 0;
						}
						else
						{
							ColumnTracker++;
						}
						WidgetInstance->SetShipSelectRef(this);
						WidgetInstance->SetShipText(ShipData->ShipName);
						WidgetInstance->SetShipRowName(Ships[i]);
						UE_LOGFMT(LogTemp, Warning, "Column: {0} Row {1}", ColumnTracker, RowTracker);
						ShipSelectGrid->AddChildToUniformGrid(WidgetInstance, RowTracker, ColumnTracker);
					}
				}
			}

		}
		DisplayShip(Ships[0]);
	}
}

void UShipSelectMenu::DisplayShip(const FName& RowName) const
{
	if(ShipRenderMat)
	{
		ShipRender->SetBrushFromMaterial(ShipRenderMat);
		AActor* ShipRenderer = UGameplayStatics::GetActorOfClass(GetWorld(), AShipRenderer::StaticClass());
		if(const AShipRenderer* ShipRenderRef = Cast<AShipRenderer>(ShipRenderer))
		{
			ShipRenderRef->SetShipMesh(RowName);
		}
	}

	if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		MainGameInstance->SetSelectedShip(RowName);
	}
}

void UShipSelectMenu::OnButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "BaseLevel");
}