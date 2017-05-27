// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "UnrealNetwork.h"
#include "BOS_PlayerState.h"




ABOS_PlayerState::ABOS_PlayerState()
{
	bReplicates = true;

	bIsDead = false;
}

void ABOS_PlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABOS_PlayerState, bIsDead);
}