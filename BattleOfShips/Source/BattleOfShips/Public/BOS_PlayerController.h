// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BOS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEOFSHIPS_API ABOS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABOS_PlayerController();

	//virtual void FailedToSpawnPawn() override;
	
	virtual void Tick(float DeltaTime) override;
};
