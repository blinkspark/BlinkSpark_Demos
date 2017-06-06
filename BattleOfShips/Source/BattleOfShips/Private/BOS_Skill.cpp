// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_Skill.h"
#include "BOS_ShipBlock.h"



UBOS_Skill::UBOS_Skill() 
{
	CD = 1.f;
}

void UBOS_Skill::BeginCast_Implementation()
{
	auto res = TryCast();
	if (res == ECastResult::EOK)
	{
		OnCast();
	}
	else if (res == ECastResult::ECD)
	{
		UE_LOG(LogTemp, Warning, TEXT("CastFaild :CD"));
	}
}
bool UBOS_Skill::BeginCast_Validate()
{
	return true;
}

ECastResult UBOS_Skill::TryCast()
{
	ECastResult ret = ECastResult::EOK;

	auto world = GetOuter()->GetWorld();
	if (world)
	{
		auto tRemaining = world->GetTimerManager().GetTimerRemaining(CoolDownTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("CD remaining: %f"), tRemaining)
		if (tRemaining > 0.f)
		{
			ret = ECastResult::ECD;
		}
	}

	return ret;
}

void UBOS_Skill::OnCast()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Begin"), ANSI_TO_TCHAR(__FUNCTION__));
	auto shipBlock = Cast<ABOS_ShipBlock>(GetOuter());
	auto world = shipBlock->GetWorld();
	if (world)
	{
		world->GetTimerManager().SetTimer(CoolDownTimerHandle, this, &UBOS_Skill::TimerFunc, CD);
		if (shipBlock)
		{
			shipBlock->Shoot_Server();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s End"), ANSI_TO_TCHAR(__FUNCTION__));

}

void UBOS_Skill::TimerFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("Test Timer"));
}
