// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_Projectile.h"
#include "BOS_PlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BOS_PlayerController.h"
#include "BOS_Skill.h"
#include "UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "BOS_ShipBlock.h"


// Sets default values
ABOS_ShipBlock::ABOS_ShipBlock()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	forwardVec = FVector(1.f, 0.f, 0.f);
	rightVec = FVector(0.f, 1.f, 0.f);

	AtkFactor = 1.f;
	DefFactor = 0.5f;

	// Replicate Props
	HP = MaxHP = 300.f;
	Atk = 160.f;
	Def = 20.f;
	CritcalRate = 0.1f;
	CritcalDmg = 1.5f;

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

	AI_SenceRange = CreateDefaultSubobject<USphereComponent>(TEXT("AI_SenceRange"));
	AI_SenceRange->SetSphereRadius(800.f);
	AI_SenceRange->SetupAttachment(RootComponent);

	ProjectileClass = ABOS_Projectile::StaticClass();

}

// Called when the game starts or when spawned
void ABOS_ShipBlock::BeginPlay()
{
	Super::BeginPlay();
}

float ABOS_ShipBlock::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	// TODO Skill Factor
	auto finalDmg = Damage*AtkFactor - Def*DefFactor;

	auto randf = FMath::RandRange(0.f, 1.f);
	if (finalDmg > 0.f && randf <= CritcalRate)
	{
		finalDmg *= CritcalDmg;
	}

	HP -= finalDmg;

	HP = HP >= 0.f ? HP : 0.f;

	if (HP == 0.f)
	{
		auto rootActor = Cast<ABOS_ShipBlock>(GetRootActor());
		auto ps = Cast<ABOS_PlayerState>(rootActor->PlayerState);
		if (ps)
		{
			if (this == rootActor)
			{
				ps->bIsDead = true;
			}
			Destroy();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%f DMG Taken"), finalDmg > 0.f ? finalDmg : 1.f);
	return finalDmg > 0.f ? finalDmg : 1.f;
}

// Called every frame
void ABOS_ShipBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto world = GetWorld();
	if (world && world->IsServer())
	{
		auto attachRoot = Cast<ABOS_ShipBlock>(GetAttachParentActor());
		if (attachRoot && !(attachRoot->GetController()))
		{
			/*auto attachParent = GetAttachParentActor();
			if (attachParent && attachParent != this)
			{
				DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			}

			TArray<AActor*> attachChildren;
			this->GetAttachedActors(attachChildren);
			for (AActor* actor : attachChildren)
			{
				actor->DetachRootComponentFromParent(true);
			}*/
		}
	}

	//auto world = GetWorld();
	//if (world && world->IsServer())
	//{
	//	auto rootName = GetRootActor()->GetName();
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), rootName.GetCharArray().GetData());
	//}

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

void ABOS_ShipBlock::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		TestSkill = NewObject<UBOS_Skill>(this);
	}
}

bool ABOS_ShipBlock::ReplicateSubobjects(UActorChannel * Channel, FOutBunch * Bunch, FReplicationFlags * RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (TestSkill != nullptr)
	{
		WroteSomething |= Channel->ReplicateSubobject(TestSkill, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void ABOS_ShipBlock::Forward_Implementation(float Axis)
{
	Forward_Server(Axis);
}

void ABOS_ShipBlock::Forward_Server_Implementation(float Axis)
{
	ShipBody->AddForce(ImpulseForce * forwardVec * Axis);
}

bool ABOS_ShipBlock::Forward_Server_Validate(float Axis)
{
	return true;
}

void ABOS_ShipBlock::Right_Implementation(float Axis)
{
	Right_Server(Axis);
}

void ABOS_ShipBlock::Right_Server_Implementation(float Axis)
{
	ShipBody->AddForce(ImpulseForce * rightVec * Axis);
}

bool ABOS_ShipBlock::Right_Server_Validate(float Axis)
{
	return true;
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
	auto world = GetWorld();
	if (world && world->IsServer())
	{
		auto rootActor = Cast<ABOS_ShipBlock>(GetRootActor());
		auto other = Cast<ABOS_ShipBlock>(OtherActor);
		auto otherRoot = Cast<ABOS_ShipBlock>(other->GetRootActor());
		if (other && !(other->PlayerState)
			&& rootActor && rootActor->PlayerState
			&& otherRoot && !(otherRoot->PlayerState)
			)
		{
			auto hitLoc = Hit.Location;
			auto loc = GetActorLocation();
			auto rot = UKismetMathLibrary::FindLookAtRotation(loc, hitLoc);
			auto yaw = rot.Yaw;
			auto locYaw = yaw - GetActorRotation().Yaw;
			auto socketName = GetSocketNameByAngle(locYaw);
			OtherActor->SetOwner(this);
			OnAttach(other, socketName);
		}
	}
}

void ABOS_ShipBlock::OnAttach_Implementation(AActor *OtherActor, FName SocketName)
{
	/*UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, SocketName: %s"),
		OtherActor->GetName().GetCharArray().GetData(),
		SocketName.ToString().GetCharArray().GetData());*/
	OnAttachBlueprintDelegate(OtherActor, SocketName);
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

ABOS_ShipBlock * ABOS_ShipBlock::GetRootActor()
{
	ABOS_ShipBlock *ret = this;
	while (ret->GetOwner())
	{
		ret = Cast<ABOS_ShipBlock>(ret->GetOwner());
	}
	return ret;
}

ABOS_ShipBlock * ABOS_ShipBlock::GetAttachRootActor()
{
	ABOS_ShipBlock *ret = this;
	while (ret->GetAttachParentActor())
	{
		ret = Cast<ABOS_ShipBlock>(ret->GetAttachParentActor());
	}
	return ret;
}

ABOS_ShipBlock * ABOS_ShipBlock::FindTarget_AI()
{
	UE_LOG(LogTemp, Warning, TEXT("FindTarget_AI Start"));
	auto loc = GetActorLocation();
	TArray<UPrimitiveComponent *> comps;
	AI_SenceRange->GetOverlappingComponents(comps);
	TMap<FString, ABOS_ShipBlock*> blocks;
	for (auto comp : comps)
	{
		auto ship_block = Cast<ABOS_ShipBlock>(comp->GetOwner());
		if (ship_block && ship_block->TeamID != TeamID)
		{
			auto name = ship_block->GetName();
			blocks.Add(name, ship_block);
		}
	}

	float minLength = -1.f;
	ABOS_ShipBlock *nearest = nullptr;
	for (auto i : blocks)
	{
		float length;
		FVector dir;
		auto enemyLoc = i.Value->GetActorLocation();
		(loc - enemyLoc).ToDirectionAndLength(dir, length);
		if (minLength < 0.f || length < minLength)
		{
			if (i.Value != this)
			{
				nearest = i.Value;
				minLength = length;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("FindTarget_AI End"));
	return nearest;
}

void ABOS_ShipBlock::TakeAim_AI(ABOS_ShipBlock *Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeAim_AI Start"));

	if (Enemy)
	{
		auto enemyLoc = Enemy->GetActorLocation();
		auto loc = GetActorLocation();

		auto rot = UKismetMathLibrary::FindLookAtRotation(loc, enemyLoc);
		UE_LOG(LogTemp, Warning, TEXT("TakeAim_AI Rot is : %s"), rot.ToString().GetCharArray().GetData());
		Gun->SetWorldRotation(rot);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TakeAim_AI target is NULL"));
	}

	UE_LOG(LogTemp, Warning, TEXT("TakeAim_AI End"));

}

void ABOS_ShipBlock::RotateGun_Implementation(float Axis)
{
	RotateGun_Server(Axis);
}

void ABOS_ShipBlock::RotateGun_Server_Implementation(float Axis)
{
#if ROTATE_SWITCH
	GunRotator.Add(0.f, GunRotateDelta * Axis, 0.f);
	Gun->SetWorldRotation(GunRotator);
#else
	ShipBody->AddAngularImpulse(FVector(0.f, 0.f, Axis * (AngularImpulse)));
#endif
}

bool ABOS_ShipBlock::RotateGun_Server_Validate(float Axis)
{
	return true;
}

void ABOS_ShipBlock::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot Start"));
	//Shoot_Server();

	TestSkill->BeginCast();
	UE_LOG(LogTemp, Warning, TEXT("Shoot End"));
}

void ABOS_ShipBlock::Shoot_Server_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot_Server Start"));

	auto world = GetWorld();
	if (world)
	{
		auto trans = Gun->GetSocketTransform(TEXT("Gun"));
		auto loc = trans.GetLocation();
		auto rot = FRotator(trans.GetRotation());
		FActorSpawnParameters sp;
		sp.Instigator = this;
		auto actor = world->SpawnActor(ProjectileClass, &loc, &rot, sp);
	}
	UE_LOG(LogTemp, Warning, TEXT("Shoot_Server End"));

}

bool ABOS_ShipBlock::Shoot_Server_Validate()
{
	return true;
}

void ABOS_ShipBlock::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABOS_ShipBlock, GunRotator);
	DOREPLIFETIME(ABOS_ShipBlock, HP);
	DOREPLIFETIME(ABOS_ShipBlock, MaxHP);
	DOREPLIFETIME(ABOS_ShipBlock, Atk);
	DOREPLIFETIME(ABOS_ShipBlock, Def);
	DOREPLIFETIME(ABOS_ShipBlock, CritcalRate);
	DOREPLIFETIME(ABOS_ShipBlock, CritcalDmg);
	DOREPLIFETIME(ABOS_ShipBlock, TestSkill);

}