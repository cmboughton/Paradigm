// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipSelectMenu.h"

#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"
#include "Paradigm_IQ/UI/Menus/MenuComponets/ShipSelect.h"
#include "Paradigm_IQ/Core/Game/MainGameInstance.h"

#include "Components/UniformGridPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "MenuComponets/ShipStatsComponent.h"
#include "MenuComponets/WeaponStatsComponent.h"
#include "MenuComponets/ShipRenderer.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"

struct FShipsDataTable;


void UShipSelectMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddDynamic(this, &UShipSelectMenu::OnButtonClicked);

	ShipSelectGrid->ClearChildren();
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

	SetShipStats(RowName);
}

void UShipSelectMenu::OnButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "BaseLevel");
}

void UShipSelectMenu::SetShipStats(const FName& DTRowName) const
{
	ShipStatsVB->ClearChildren();
	if (const UDataTable* ShipDataTableHardRef = ShipDataTable.LoadSynchronous())
	{
		if (const FShipsDataTable* ShipData = ShipDataTableHardRef->FindRow<FShipsDataTable>(DTRowName, "Ships Data Table Not set up", true))
		{
			if (!ShipData->ShipStats.IsEmpty())
			{
				for (const auto Stat : ShipData->ShipStats)
				{
					if (ShipStatsRef)
					{
						if (UShipStatsComponent* WidgetInstance = CreateWidget<UShipStatsComponent>(GetWorld(), ShipStatsRef))
						{
							WidgetInstance->SetStatName(FText::FromName(Stat.StatName));
							WidgetInstance->SetStatValue(Stat.StatValue);
							WidgetInstance->SetStatValuePB(UKismetMathLibrary::NormalizeToRange(Stat.StatValue, 0, Stat.MaxStatValue));
							ShipStatsVB->AddChild(WidgetInstance);
						}
					}
				}
			}
			WeaponUltimateHB->ClearChildren();
			SetShipWeapon(*ShipData);
			SetShipUltimate(*ShipData);
		}
	}
}

void UShipSelectMenu::SetShipWeapon(const FShipsDataTable& ShipData) const
{
	if (const UDataTable* WeaponDataTableHardRef = WeaponDataTable.LoadSynchronous())
	{
		if (const FWeaponsDataTable* WeaponData = WeaponDataTableHardRef->FindRow<FWeaponsDataTable>(ShipData.BaseWeapon, "Weapons Data Table Not set up", true))
		{
			if (WeaponStatsRef)
			{
				if (UWeaponStatsComponent* WidgetInstance = CreateWidget<UWeaponStatsComponent>(GetWorld(), WeaponStatsRef))
				{
					WidgetInstance->GetIcon()->SetBrushFromTexture(WeaponData->WeaponIcon);
					WidgetInstance->GetTextName()->SetText(FText::FromName(WeaponData->Name));
					WidgetInstance->SetPadding(FMargin(10, 0));
					WeaponUltimateHB->AddChild(WidgetInstance);
				}
			}
		}
	}
}

void UShipSelectMenu::SetShipUltimate(const FShipsDataTable& ShipData) const
{
	if (const UDataTable* UltimateDataTableHardRef = UltimateDataTable.LoadSynchronous())
	{
		if (const FUltimatesDataTable* UltimateData = UltimateDataTableHardRef->FindRow<FUltimatesDataTable>(ShipData.UltimateAbility, "Weapons Data Table Not set up", true))
		{
			if (UltimateStatsRef)
			{
				if (UWeaponStatsComponent* WidgetInstance = CreateWidget<UWeaponStatsComponent>(GetWorld(), UltimateStatsRef))
				{
					WidgetInstance->GetIcon()->SetBrushFromTexture(UltimateData->UltimateIcon);
					WidgetInstance->GetTextName()->SetText(FText::FromName(UltimateData->UltimateName));
					WidgetInstance->SetPadding(FMargin(10, 0));
					WeaponUltimateHB->AddChild(WidgetInstance);
				}
			}
		}
	}
}
