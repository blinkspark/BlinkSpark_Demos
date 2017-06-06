// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_PlayerState.h"
#include "BOS_PlayerController.h"



ABOS_PlayerController::ABOS_PlayerController()
{
}

//void ABOS_PlayerController::FailedToSpawnPawn()
//{
//	
//}

void ABOS_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*auto p = GetPawn();
	if (p)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn : %s"), p->GetName().GetCharArray().GetData());
	}
	UE_LOG(LogTemp, Warning, TEXT("Pawn : NULL"));*/

	auto world = GetWorld();
	if (world && world->IsServer())
	{
		auto gm = world->GetAuthGameMode();
		auto ps = Cast<ABOS_PlayerState>(PlayerState);
		if (gm && ps && !GetPawn() && !(ps->bIsDead))
		{
			gm->RestartPlayer(this);
		}
	}

}