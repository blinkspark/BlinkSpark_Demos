// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_PlayerController.h"
#include "BOS_ShipBlock.h"
#include "BOS_CombatGameMode.h"



ABOS_CombatGameMode::ABOS_CombatGameMode() {
	PlayerControllerClass = ABOS_PlayerController::StaticClass();
	DefaultPawnClass = ABOS_ShipBlock::StaticClass();
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
	auto sb = Cast<ABOS_ShipBlock>(NewPlayer->GetPawn());
	if (sb)
	{
		sb->TeamID = TeamID;
		UE_LOG(LogTemp, Warning, TEXT("changing teamid: %d"), TeamID++);
	}
}


