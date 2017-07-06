// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "BOS_ShipBlock.h"
#include "BOS_AttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"


//UBOS_AttributeSet::UBOS_AttributeSet()
//{
//}


void UBOS_AttributeSet::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME( UMyAttributeSet, MyAttribute); Chances are this is how you would ordinarily do it, however in the case of attributes this'll lead to confusing and annoying replication errors, usually involving clientside ability prediction.
	DOREPLIFETIME_CONDITION_NOTIFY(UBOS_AttributeSet, HP, COND_None, REPNOTIFY_Always); //This is how it is done properly for attributes. 
	DOREPLIFETIME_CONDITION_NOTIFY(UBOS_AttributeSet, TeamID, COND_None, REPNOTIFY_Always); //This is how it is done properly for attributes. 
}


void UBOS_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	auto instig = Data.EffectSpec.GetContext().GetOriginalInstigator();
	auto target = Cast<ABOS_ShipBlock>(Data.Target.AvatarActor);
	auto shipBlock = instig ? Cast<ABOS_ShipBlock>(instig) : nullptr;

	if (target)
	{
		target->OnDataRefresh();
		float hp = target->AttributeSet->HP;
		if (bDebugMode)
		{
			UE_LOG(LogTemp, Error, TEXT("HP: %f"), target->AttributeSet->HP);
		}
		if (hp <= 0.f)
		{
			target->Destroy();
		}
	}
}

FGameplayAttribute UBOS_AttributeSet::HealthAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBOS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBOS_AttributeSet, HP));
	return FGameplayAttribute(Property);
}