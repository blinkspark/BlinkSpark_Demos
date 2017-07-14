// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataStructureLib.generated.h"


UCLASS()
class UHexTemplateTree : public UDataAsset{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		TSubclassOf<class ABOS_ShipBlock> Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		TMap<FName, UHexTemplateTree*> leaves;
};

USTRUCT(BlueprintType)
struct FHexTree
{
	GENERATED_BODY()
public:
	FHexTree();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		uint8 bIsRoot : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		TMap<FName, class ABOS_ShipBlock*> Leaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		class ABOS_ShipBlock *Trunk;
};

/**
 * 
 */
UCLASS()
class BATTLEOFSHIPS_API UDataStructureLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	const static bool bDebugMode = true;

	UFUNCTION(BlueprintCallable, Category = "HexTree")
		static class ABOS_ShipBlock *GetOppositeLeaf(FHexTree &Tree, class ABOS_ShipBlock *Leaf);
	
	UFUNCTION(BlueprintCallable, Category = "HexTree")
		static TArray<class ABOS_ShipBlock*>GetAllChildren(const FHexTree &Tree);

	UFUNCTION(BlueprintCallable, Category = "HexTree")
		static FName FindLeafName(const FHexTree &Tree, const ABOS_ShipBlock *Leaf);

	UFUNCTION(BlueprintCallable, Category = "HexTree")
		static TArray<class ABOS_ShipBlock*> GetChildrenByTag(FHexTree &Tree, FName tag);

	//UFUNCTION(BlueprintCallable, Category = "HexTree")
	//	static TPair<FName, class ABOS_ShipBlock*> FindLeaf(FHexTree &Tree, class ABOS_ShipBlock *Leaf);
};

