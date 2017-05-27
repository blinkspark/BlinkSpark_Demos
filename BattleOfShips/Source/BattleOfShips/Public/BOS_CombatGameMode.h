// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BOS_CombatGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEOFSHIPS_API ABOS_CombatGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABOS_CombatGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	
};
