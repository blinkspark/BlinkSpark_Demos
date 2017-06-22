// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BOS_AttributeSet.generated.h"

/**
 *
 */
UCLASS()
class BATTLEOFSHIPS_API UBOS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	UPROPERTY(Category = "Attribute|HP", EditAnywhere, ReplicatedUsing = OnRep_HP, BlueprintReadWrite)
		float HP;
	UFUNCTION()
		void OnRep_HP()
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBOS_AttributeSet, HP);
	}

	//Outgoing damage-multiplier.
	UPROPERTY(Category = "Attribute|HP", EditAnywhere, BlueprintReadWrite, meta = (HideFromModifiers))
		float AttackMultiplier;

	//Incoming damage-multiplier.
	UPROPERTY(Category = "Attribute|HP", EditAnywhere, BlueprintReadWrite)
		float DefenseMultiplier;

	UPROPERTY(Category = "Attribute|HP", EditAnywhere, BlueprintReadWrite)
		float BaseAttackPower;

	UFUNCTION(BlueprintCallable, Category = "Attribute|HP")
		FGameplayAttribute HealthAttribute();
};
