// Fill out your copyright notice in the Description page of Project Settings.

#include "BlinkCombatSystem.h"
#include "BlinkCombatSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "BaseAbility.h"



//void UBaseAbility::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(UBaseAbility, test);
//
//}

UBaseAbility::UBaseAbility() 
{

}

bool UBaseAbility::OnUse_Server_Implementation(UBlinkCombatSystemComponent *BCS)
{
	UE_LOG(LogTemp, Warning, TEXT("OnUse_Server"));

	return true;
}

void UBaseAbility::OnAffect_Multi_Implementation(UBlinkCombatSystemComponent *BCS)
{
	UE_LOG(LogTemp, Warning, TEXT("OnAffect_Multi"));
}
