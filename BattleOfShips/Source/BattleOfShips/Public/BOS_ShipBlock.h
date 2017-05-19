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

	UPROPERTY()
	FVector forwardVec;

	UPROPERTY()
	FVector rightVec;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Controlls")
	void Forward(float Axis);
	void Forward_Implementation(float Axis);

	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Controlls")
	void Right(float Axis);
	void Right_Implementation(float Axis);

	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Controlls")
	void RotateGun(float Axis);
	void RotateGun_Implementation(float Axis);

	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Controlls")
	void Shoot();
	void Shoot_Implementation();


	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Logic")
	void FollowV();
	void FollowV_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Logic")
	void ShipBodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void ShipBodyHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Logic")
	void OnAttach(AActor *OtherActor, FName SocketName);
	void OnAttach_Implementation(AActor *OtherActor, FName SocketName);



	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
	FName GetSocketNameByAngle(float Angle);

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
	AActor *GetRootActor();


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *ShipBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float ImpulseForce = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float AngularImpulse = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float DeltaNormalizer = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float GunRotateDelta = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FRotator GunRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABOS_Projectile> ProjectileClass;

};
