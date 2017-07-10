// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataStructureLib.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEOFSHIPS_API UDataStructureLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Test")
		static void Test();
	
	
};
