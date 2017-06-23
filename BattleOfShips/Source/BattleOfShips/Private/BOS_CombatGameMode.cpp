// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_PlayerController.h"
#include "BOS_ShipBlock.h"
#include "DummyObj.h"
#include "BOS_CombatGameMode.h"



ABOS_CombatGameMode::ABOS_CombatGameMode() {
	PlayerControllerClass = ABOS_PlayerController::StaticClass();
	DefaultPawnClass = ABOS_ShipBlock::StaticClass();

	ConstructorHelpers::FObjectFinder<UDataTable> BlockTable(TEXT("/Game/Data/ShipBlockTable.ShipBlockTable"));
	this->ShipBlockTable = BlockTable.Object;
}

void ABOS_CombatGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("%s, post login"), NewPlayer->GetName().GetCharArray().GetData());
}

bool ABOS_CombatGameMode::ShouldSpawnAtStartSpot(AController * Player)
{
	return false;
}

void ABOS_CombatGameMode::RestartPlayer(AController * NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	auto pc = Cast<ABOS_PlayerController>(NewPlayer);
	auto uids = pc->GetUniqueID();
	//UE_LOG(LogTemp, Warning, TEXT("uid: %d"), uids);
	auto sb = Cast<ABOS_ShipBlock>(NewPlayer->GetPawn());
	if (sb)
	{
		sb->TeamID = uids;
		//UE_LOG(LogTemp, Warning, TEXT("changing teamid: %d"), TeamID++);
	}
}

AActor * ABOS_CombatGameMode::ShipBlockFactory(int32 TypeID, FVector SpawnLoc, FRotator SpawnRot)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s begin"), ANSI_TO_TCHAR(__FUNCTION__));

	auto world = GetWorld();
	auto i = TypeID / 100;
	auto maxi = SpawnClasses.Num();
	AActor *ret = nullptr;
	if (world && maxi > 0 
		&& i > 0 && i <= maxi)
	{
		FActorSpawnParameters sp;
		sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
		//UE_LOG(LogTemp, Warning, TEXT("i:%d"), i);
		TSubclassOf<ABOS_ShipBlock> ClassToSpawn = SpawnClasses[i - 1];
		ret = world->SpawnActor(ClassToSpawn, &SpawnLoc, &SpawnRot, sp);
		if (ret)
		{
			auto shipBlock = Cast<ABOS_ShipBlock>(ret);
			if (shipBlock)
			{
				static const FString context(TEXT("GENERAL"));
				auto row = ShipBlockTable->FindRow<FBlockType>(*FString::Printf(TEXT("%d"), TypeID), context);
				if (row) 
				{
					//UE_LOG(LogTemp, Warning, TEXT("OK"));
					shipBlock->MaxHP = shipBlock->HP = row->HP;
					shipBlock->Atk = row->Atk;
					shipBlock->Def = row->Def;
					shipBlock->CritcalRate = row->Crit;
					shipBlock->OnDataRefresh();
					//TODO AtkSpeed
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to read data from ShipBlockTable."));
				}
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s end"), ANSI_TO_TCHAR(__FUNCTION__));
	return ret;
}

