// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "DummyObj.generated.h"

/**
 * 
 */
UCLASS()
class UDummyObj : public UObject
{
	GENERATED_BODY()
	
};

UCLASS(MinimalAPI, const, Blueprintable, BlueprintType)
class UBOS_DamageTypeBase : public UDamageType {
	GENERATED_BODY()
	
};

UCLASS(MinimalAPI, const, Blueprintable, BlueprintType)
class UBOS_DamageTypeKinect : public UDamageType {
	GENERATED_BODY()

};

UCLASS(MinimalAPI, const, Blueprintable, BlueprintType)
class UBOS_DamageTypeEnergy : public UDamageType {
	GENERATED_BODY()

};

UCLASS(MinimalAPI, const, Blueprintable, BlueprintType)
class UBOS_DamageTypeExplode : public UDamageType {
	GENERATED_BODY()

};