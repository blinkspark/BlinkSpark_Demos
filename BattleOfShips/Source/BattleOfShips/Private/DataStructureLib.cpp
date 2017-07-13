// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "DataStructureLib.h"
#include "BOS_ShipBlock.h"



ABOS_ShipBlock * UDataStructureLib::GetOppositeLeaf(FHexTree &Tree, ABOS_ShipBlock *Leaf)
{
	return nullptr;
}



TArray<class ABOS_ShipBlock*> UDataStructureLib::GetAllChildren(const FHexTree &Tree)
{
	TArray<ABOS_ShipBlock*> res;
	for (const auto &ite : Tree.Leaves)
	{
		if (ite.Value)
		{
			res.Add(ite.Value);
			//if (UDataStructureLib::bDebugMode)
			//{
			//}

			UE_LOG(LogTemp, Warning, TEXT("%s: %s"), TEXT(__FUNCTION__), ite.Key.ToString().GetCharArray().GetData());

			auto arr = UDataStructureLib::GetAllChildren(ite.Value->HexTree);
			res.Append(arr);
		}
	}
	return res;
}

FName UDataStructureLib::FindLeafName(const FHexTree &Tree, const ABOS_ShipBlock *Leaf)
{
	FName name = TEXT("NULL");
	for (const auto &ite : Tree.Leaves)
	{
		if (Leaf == ite.Value) {
			name = ite.Key;
		}
	}
	return name;
}

//TPair<FName, class ABOS_ShipBlock*> UDataStructureLib::FindLeaf(FHexTree &Tree, class ABOS_ShipBlock *Leaf)
//{
//	TPair<FName, ABOS_ShipBlock*> res;
//	res.Value = nullptr;
//	res.Key = TEXT("Root");
//	return res;
//}

FHexTree::FHexTree()
{
}
