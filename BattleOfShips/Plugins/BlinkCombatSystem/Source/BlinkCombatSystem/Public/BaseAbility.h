// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "UObject/ScriptMacros.h"
#include "BaseAbility.generated.h"

/**
*
*/
UCLASS(Blueprintable)
class BLINKCOMBATSYSTEM_API UBaseAbility : public UObject
{
	GENERATED_BODY()

public:
	UBaseAbility();
	//virtual bool IsSupportedForNetworking() const override
	//{
	//	return true;
	//}
	//virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Properties
	/** Replicate */

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
		bool OnUse_Server(class UBlinkCombatSystemComponent *BCS);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
		void OnAffect_Multi(class UBlinkCombatSystemComponent *BCS);

};

UENUM(BlueprintType)
enum class EAbilityResult : uint8
{
	EOK, ECD, EFail, EError
};

USTRUCT(BlueprintType)
struct FBaseAblityStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int32 ID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float CD = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		FTimerHandle CDTimerH;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TSubclassOf<class UBaseAbility> AbilityObj;

};


