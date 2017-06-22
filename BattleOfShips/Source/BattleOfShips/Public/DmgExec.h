// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "DmgExec.generated.h"


/**
 * 
 */
UCLASS()
class BATTLEOFSHIPS_API UDmgExec : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;


	
};
