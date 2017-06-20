// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "BaseAbility.h"
#include "BlinkCombatSystemComponent.generated.h"



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLINKCOMBATSYSTEM_API UBlinkCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBlinkCombatSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FTimerManager *tm;

public:	
	// Core Functions
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Functions
	/** Getters */
	UFUNCTION(BlueprintPure, Category = "Properties|HP")
		float GetHP() const;

	UFUNCTION(BlueprintPure, Category = "Properties|HP")
		float GetMaxHP() const;

	UFUNCTION(BlueprintPure, Category = "Properties|MP")
		float GetMP() const;

	UFUNCTION(BlueprintPure, Category = "Properties|MP")
		float GetMaxMP() const;

	UFUNCTION(BlueprintPure, Category = "Properties")
		float GetAtk() const;

	UFUNCTION(BlueprintPure, Category = "Properties")
		float GetDef() const;

	UFUNCTION(BlueprintPure, Category = "Properties")
		float GetMoveSpeed() const;

	UFUNCTION(BlueprintPure, Category = "Properties")
		float GetAtkSpeed() const;
	/** Core Functions */
	UFUNCTION(BlueprintCallable, Category = "BCS|Ability")
		EAbilityResult TryUseAbility(int Index);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "BCS|Ability")
		void UseAbility(int index);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "BCS|Ability")
		void OnAbilityAffect(int index);

	UFUNCTION(BlueprintCallable, Category = "BCS|Ability")
		bool IsIndexValid(int index);

	UFUNCTION(BlueprintCallable, Category = "BCS|Ability")
		bool IsCD(int index);

	UFUNCTION(BlueprintCallable, Category = "BCS|Ability")
		UBaseAbility *GetDefaultAbilityObj(int index);

	// Properties
	/** Not Replicated */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimerHandle", meta = (AllowPrivateAccess = "true"))
		FTimerHandle CoolDownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		TArray<FBaseAblityStruct> Abilities;

	/** Replicated */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float BaseMaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float BaseMaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float BaseAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float BaseAtkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float BaseDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float BaseMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float BaseHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float BaseMP;

	

};
