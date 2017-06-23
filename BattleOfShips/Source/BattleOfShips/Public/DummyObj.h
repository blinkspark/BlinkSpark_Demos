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

/**** Structs **************/

//USTRUCT(Blueprintable)
//struct FTestData : public FTableRowBase
//{
//	GENERATED_BODY()
//
//public:
//
//	FTestData()
//		: name()
//		, age(0)
//	{
//	}
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
//		FString name;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
//		int32 age;
//};

USTRUCT(Blueprintable)
struct FBlockType : public FTableRowBase
{
	GENERATED_BODY()

public:

	FBlockType()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlockType")
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlockType")
		float Atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlockType")
		float Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlockType")
		float Crit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlockType")
		float AtkSpeed;

};