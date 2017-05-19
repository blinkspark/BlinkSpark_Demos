// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BOS_PlayerController.h"
#include "BOS_ShipBlock.h"


// Sets default values
ABOS_ShipBlock::ABOS_ShipBlock()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	forwardVec = FVector(1.f, 0.f, 0.f);
	rightVec = FVector(0.f, 1.f, 0.f);


	ShipBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipBody"));
	RootComponent = ShipBody;
	ShipBody->OnComponentHit.AddDynamic(this, &ABOS_ShipBlock::ShipBodyHit);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	Gun->SetupAttachment(RootComponent);

	ProjectileClass = ABOS_Projectile::StaticClass();
}

// Called when the game starts or when spawned
void ABOS_ShipBlock::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABOS_ShipBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (GetInputAxisValue(TEXT("Forward"))
	//	|| GetInputAxisValue(TEXT("Right")))
	//{
#if ROTATE_SWITCH
	FollowV();
#endif
	//}
}

// Called to bind functionality to input
void ABOS_ShipBlock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ABOS_ShipBlock::Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &ABOS_ShipBlock::Right);
	PlayerInputComponent->BindAxis(TEXT("GunRotate"), this, &ABOS_ShipBlock::RotateGun);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &ABOS_ShipBlock::Shoot);

}

void ABOS_ShipBlock::Forward_Implementation(float Axis)
{
	ShipBody->AddForce(ImpulseForce * forwardVec * Axis);
}

void ABOS_ShipBlock::Right_Implementation(float Axis)
{
	ShipBody->AddForce(ImpulseForce * rightVec * Axis);
}

void ABOS_ShipBlock::FollowV_Implementation()
{

	auto vr = Gun->GetComponentRotation();
	auto currentR = GetActorRotation();

	auto deltaR = UKismetMathLibrary::NormalizedDeltaRotator(currentR, vr);
	auto yaw = FMath::GetMappedRangeValueUnclamped(FVector2D(-90.f, 90.f), FVector2D(-DeltaNormalizer, DeltaNormalizer), deltaR.Yaw);
	//auto yaw = deltaR.Yaw;


	ShipBody->AddAngularImpulse(FVector(0.f, 0.f, yaw * (-AngularImpulse)));
}

void ABOS_ShipBlock::ShipBodyHit_Implementation(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Cast<ABOS_ShipBlock>(OtherActor))
	{
		if (!OtherActor->GetOwner() && (Cast<ABOS_ShipBlock>(GetRootActor())->GetController()))
		{
			auto hitLoc = Hit.Location;
			auto loc = GetActorLocation();
			auto rot = UKismetMathLibrary::FindLookAtRotation(loc, hitLoc);
			auto yaw = rot.Yaw;
			auto locYaw = yaw - GetActorRotation().Yaw;
			auto socketName = GetSocketNameByAngle(locYaw);
			OnAttach(OtherActor, socketName);
		}
	}

}

void ABOS_ShipBlock::OnAttach_Implementation(AActor *OtherActor, FName SocketName)
{
	/*UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, SocketName: %s"),
		OtherActor->GetName().GetCharArray().GetData(),
		SocketName.ToString().GetCharArray().GetData());*/
	auto rootActor = GetRootActor();
	OtherActor->SetOwner(rootActor);
}

FName ABOS_ShipBlock::GetSocketNameByAngle(float Angle)
{
	FName ret;
	if ((Angle >= 0.f && Angle < 60.f) || (Angle > -360.f && Angle <= -300.f))
	{
		ret = TEXT("RU");
	}
	else if ((Angle >= 60.f && Angle < 120.f) || (Angle > -300.f && Angle <= -240.f))
	{
		ret = TEXT("R");
	}
	else if ((Angle >= 120.f && Angle < 180.f) || (Angle > -240.f && Angle <= -180.f))
	{
		ret = TEXT("RD");
	}
	else if ((Angle >= 180.f && Angle < 240.f) || (Angle > -180.f && Angle <= -120.f))
	{
		ret = TEXT("LD");
	}
	else if ((Angle >= 240.f && Angle < 300.f) || (Angle > -120.f && Angle <= -60.f))
	{
		ret = TEXT("L");
	}
	else if ((Angle >= 300.f && Angle < 360.f) || (Angle > -60.f && Angle <= 0.f))
	{
		ret = TEXT("LU");
	}
	return ret;
}

AActor * ABOS_ShipBlock::GetRootActor()
{
	AActor *ret = this;
	while (ret->GetOwner())
	{
		ret = ret->GetOwner();
	}
	return ret;
}

void ABOS_ShipBlock::RotateGun_Implementation(float Axis)
{
#if ROTATE_SWITCH
	GunRotator.Add(0.f, GunRotateDelta * Axis, 0.f);
	Gun->SetWorldRotation(GunRotator);
#else
	ShipBody->AddAngularImpulse(FVector(0.f, 0.f, Axis * (AngularImpulse)));
#endif
}


void ABOS_ShipBlock::Shoot_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot"));

	auto world = GetWorld();
	if (world)
	{
		auto trans = Gun->GetSocketTransform(TEXT("Gun"));
		auto loc = trans.GetLocation();
		auto rot = FRotator(trans.GetRotation());
		FActorSpawnParameters sp;
		sp.Instigator = this;
		auto actor = world->SpawnActor(ProjectileClass, &loc, &rot, sp);
		UE_LOG(LogTemp, Warning, TEXT("%s"), trans.ToString().GetCharArray().GetData());
	}
}
