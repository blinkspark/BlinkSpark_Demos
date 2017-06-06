// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "BOS_Skill.generated.h"

UENUM(BlueprintType)
enum class ECastResult : uint8
{
	EOK, ECD
};


/**
 * 
 */
UCLASS(Blueprintable)
class BATTLEOFSHIPS_API UBOS_Skill : public UObject
{
	GENERATED_BODY()
	
	
public:
	UBOS_Skill();

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	// Properties
	UPROPERTY()
		FTimerHandle CoolDownTimerHandle;

	UPROPERTY()
		float CD;

	// Functions
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Skill")
		void BeginCast();

	UFUNCTION(BlueprintAuthorityOnly, Category = "Skill")
		ECastResult TryCast();

	UFUNCTION(BlueprintAuthorityOnly, Category = "Skill")
		void OnCast();

	UFUNCTION()
		void TimerFunc();

};
