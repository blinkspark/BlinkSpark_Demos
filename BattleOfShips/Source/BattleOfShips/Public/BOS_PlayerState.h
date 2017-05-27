// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "BOS_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEOFSHIPS_API ABOS_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABOS_PlayerState();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Components", meta = (AllowPrivateAccess = "true"))
		uint32 bIsDead : 1;


};
