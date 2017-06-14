// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "BOS_ShipBlock.generated.h"

UCLASS()
class BATTLEOFSHIPS_API ABOS_ShipBlock : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABOS_ShipBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY()
		FVector forwardVec;

	UPROPERTY()
		FVector rightVec;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "ShipBlock|Controlls")
		void Forward(float Axis);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "ShipBlock|Controlls")
		void Right(float Axis);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "ShipBlock|Controlls")
		void RotateGun(float Axis);

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Controlls")
		void Shoot();
	UFUNCTION(NetMulticast, Reliable, Category = "ShipBlock|Multicast")
		void Shoot_Multi();

	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Logic")
		void FollowV();

	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Logic")
		void ShipBodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable, Category = "ShipBlock|Multicast")
		void OnAttach(AActor *OtherActor, FName SocketName);
	UFUNCTION(BlueprintImplementableEvent, Category = "ShipBlock|Logic")
		void OnAttachBlueprintDelegate(AActor *OtherActor, FName SocketName);

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
		FName GetSocketNameByAngle(float Angle);

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
		ABOS_ShipBlock *GetRootActor();

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
		ABOS_ShipBlock *GetAttachRootActor();

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|AI")
		ABOS_ShipBlock *FindTarget_AI();

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|AI")
		void TakeAim_AI(ABOS_ShipBlock *Enemy);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ShipBlock|Logic")
		void OnDataRefresh();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		uint32 bLog : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	//	UDataTable *TestDataTable;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent *CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent *Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent *ShipBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USphereComponent *AI_SenceRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float ImpulseForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float AngularImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float DeltaNormalizer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float GunRotateDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ABOS_Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float AtkFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float DefFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float AngularImpulseStepUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float ImpulseForceStepUp;


	/****************  Replicated  **************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		FRotator GunRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float Atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float CritcalRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float CritcalDmg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		class UBOS_Skill *TestSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		int32 TeamID;

};
