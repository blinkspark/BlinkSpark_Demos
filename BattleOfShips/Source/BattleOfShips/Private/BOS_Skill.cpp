// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_Skill.h"


UBOS_Skill::UBOS_Skill() 
{
}

void UBOS_Skill::Cast()
{
	UE_LOG(LogTemp, Warning, TEXT("Cast"));
	auto world = this->GetOuter()->GetWorld();
	if (world)
	{
		world->GetTimerManager().SetTimer(CoolDownTimerHandle, this, &UBOS_Skill::TimerFunc, 1.f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null"));
	}
}

void UBOS_Skill::TimerFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("Test Timer"));
}
