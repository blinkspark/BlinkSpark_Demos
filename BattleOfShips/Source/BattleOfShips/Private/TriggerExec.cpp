// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "TriggerExec.h"

#include "GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameplayTags.h"
#include "GameplayTagsModule.h"




void UTriggerExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	auto targetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	auto sourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	FGameplayEventData ge;
	auto tag = FGameplayTag::RequestGameplayTag(FName(TEXT("AbilityTags.Heal")));
	//FName name("AbilityTags.Heal");
	ge.EventTag = tag;
	ge.Instigator = sourceASC ? sourceASC->AvatarActor : nullptr;
	ge.Target = targetASC ? targetASC->AvatarActor : nullptr;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(targetASC->AvatarActor, tag, ge);
}
