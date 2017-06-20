// Fill out your copyright notice in the Description page of Project Settings.

#include "BlinkCombatSystem.h"
#include "UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "TimerManager.h"
#include "BlinkCombatSystemComponent.h"


// Sets default values for this component's properties
UBlinkCombatSystemComponent::UBlinkCombatSystemComponent() :CoolDownTimer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetNetAddressable();
	SetIsReplicated(true);
	// ...
}


// Called when the game starts
void UBlinkCombatSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld *world = GetWorld();
	if (world)
	{
		tm = &world->GetTimerManager();
	}

}


// Called every frame
void UBlinkCombatSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UBlinkCombatSystemComponent::GetHP() const
{
	return BaseHP;
}

float UBlinkCombatSystemComponent::GetMaxHP() const
{
	return BaseMaxHP;
}

float UBlinkCombatSystemComponent::GetMP() const
{
	return BaseMP;
}

float UBlinkCombatSystemComponent::GetMaxMP() const
{
	return BaseMaxMP;
}

float UBlinkCombatSystemComponent::GetAtk() const
{
	return BaseAtk;
}

float UBlinkCombatSystemComponent::GetDef() const
{
	return BaseDef;
}

float UBlinkCombatSystemComponent::GetMoveSpeed() const
{
	return BaseMoveSpeed;
}

float UBlinkCombatSystemComponent::GetAtkSpeed() const
{
	return BaseAtkSpeed;
}

EAbilityResult UBlinkCombatSystemComponent::TryUseAbility(int index)
{
	EAbilityResult res = EAbilityResult::EOK;

	if (!IsIndexValid(index))
	{
		res = EAbilityResult::EError;
	}
	else if (IsCD(index))
	{
		res = EAbilityResult::ECD;
	}
	
	return res;
}

void UBlinkCombatSystemComponent::UseAbility_Implementation(int index)
{
	UE_LOG(LogTemp, Warning, TEXT("UseAbility_Implementation"));
	auto abo = GetDefaultAbilityObj(index);
	if (abo)
	{
		if(abo->OnUse_Server(this))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnUse_Server over"));
			OnAbilityAffect(index);
		}
	}
}
bool UBlinkCombatSystemComponent::UseAbility_Validate(int index)
{
	return true;
}

void UBlinkCombatSystemComponent::OnAbilityAffect_Implementation(int index)
{
	auto ab = Abilities[index];  
	auto abo = GetDefaultAbilityObj(index);
	if (abo)
	{
		tm->SetTimer(ab.CDTimerH, ab.CD, false); 
		abo->OnAffect_Multi(this);
	}
}

//void UBlinkCombatSystemComponent ::UseAbility_Implementation(int index)
//{
//	auto abs = Abilities[index];
//	auto abo = abs.AbilityObj.GetDefaultObject();
//	auto abobj = Cast<UBaseAbility>(abo);
//	//abobj->OnUse();
//}

bool UBlinkCombatSystemComponent::IsIndexValid(int index)
{
	bool res = true;
	auto an = Abilities.Num();
	//UE_LOG(LogTemp, Warning, TEXT("abilities num %d"), an);
	if (an == 0 || index >= an)
	{
		res = false;
	}
	return res;
}

bool UBlinkCombatSystemComponent::IsCD(int index)
{
	bool res = true;
	
	if (IsIndexValid(index)) 
	{
		auto ab = Abilities[index];
		float rem = tm->GetTimerRemaining(ab.CDTimerH);
		if (rem <= 0)
		{
			res = false;
		}
	}
	return res;
}

UBaseAbility *UBlinkCombatSystemComponent::GetDefaultAbilityObj(int index)
{
	UBaseAbility *abo = nullptr;
	if (IsIndexValid(index))
	{
		auto ab = Abilities[index];
		abo = ab.AbilityObj.GetDefaultObject();
	}
	return abo;
}

void UBlinkCombatSystemComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBlinkCombatSystemComponent, BaseMaxHP);
	DOREPLIFETIME(UBlinkCombatSystemComponent, BaseHP);
	DOREPLIFETIME(UBlinkCombatSystemComponent, BaseMaxMP);
	DOREPLIFETIME(UBlinkCombatSystemComponent, BaseMP);
	DOREPLIFETIME(UBlinkCombatSystemComponent, BaseAtk);
	DOREPLIFETIME(UBlinkCombatSystemComponent, BaseDef);
	DOREPLIFETIME(UBlinkCombatSystemComponent, BaseAtkSpeed);
	DOREPLIFETIME(UBlinkCombatSystemComponent, BaseMoveSpeed);

}

