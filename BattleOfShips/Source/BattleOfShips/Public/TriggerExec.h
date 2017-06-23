// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "TriggerExec.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEOFSHIPS_API UTriggerExec : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	
	
};
