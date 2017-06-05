// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "BOS_Skill.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEOFSHIPS_API UBOS_Skill : public UObject
{
	GENERATED_BODY()
	
	
public:
	UBOS_Skill();

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	UPROPERTY()
		FTimerHandle CoolDownTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Skill")
		void Cast();

	UFUNCTION()
		void TimerFunc();


};
