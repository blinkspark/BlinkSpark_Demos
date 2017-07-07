// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "DummyObj.h"
#include "BOS_Projectile.h"
#include "BOS_PlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BOS_PlayerController.h"
#include "BOS_Skill.h"
#include "UnrealNetwork.h"
#include "Engine/ActorChannel.h"
//#include "BlinkCombatSystemComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BOS_AttributeSet.h"
#include "GameplayAbilitySet.h"
#include "BOS_ShipBlock.h"


// Sets default values
ABOS_ShipBlock::ABOS_ShipBlock():TagName(TEXT("AbilityTags.Attack"))
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bDebugMode = true;
	bUseNewAttribute = false;

	forwardVec = FVector(1.f, 0.f, 0.f);
	rightVec = FVector(0.f, 1.f, 0.f);

	//AtkFactor = 1.f;
	//DefFactor = 0.5f;

	//AngularImpulse = 3000.f;
	//ImpulseForce = 10000.f;
	//AngularImpulseStepUp = 9000.f;
	//ImpulseForceStepUp = 3000.f;

	DeltaNormalizer = 1.f;
	GunRotateDelta = 0.5f;

	// Replicate Props
	//HP = MaxHP = 300.f;
	//Atk = 160.f;
	//Def = 20.f;
	//CritcalRate = 0.1f;
	//CritcalDmg = 1.5f;

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
	Gun->SetIsReplicated(true);

	AI_SenceRange = CreateDefaultSubobject<USphereComponent>(TEXT("AI_SenceRange"));
	AI_SenceRange->SetSphereRadius(800.f);
	AI_SenceRange->SetupAttachment(RootComponent);

	ProjectileClass = ABOS_Projectile::StaticClass();

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

	AttributeSet = CreateDefaultSubobject<UBOS_AttributeSet>(TEXT("AttributeSet"));

	AbilitySet = CreateDefaultSubobject<UGameplayAbilitySet>(TEXT("AbilitySet"));

	//ConstructorHelpers::FObjectFinder<UDataTable> TestTable(TEXT("/Game/UI/Book1.Book1"));
	//this->TestDataTable = TestTable.Object;
	//BlinkCombatSystemComponent = CreateDefaultSubobject<UBlinkCombatSystemComponent>(TEXT("BlinkCombatSystemComponent"));

}

// Called when the game starts or when spawned
void ABOS_ShipBlock::BeginPlay()
{
	Super::BeginPlay();
	/*static const FString str(TEXT("GENERAL"));
	TArray<FTestData*> arr;
	TestDataTable->GetAllRows<FTestData>(str, arr);
	for (auto i : arr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s:%d"), i->name.GetCharArray().GetData(), i->age);
	}*/

	if (AbilitySystem)
	{
		if (HasAuthority() && AtkAbility && HealAbility)
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(AtkAbility.GetDefaultObject(), 1, 0));
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(HealAbility.GetDefaultObject(), 1, 1));
			//UE_LOG(LogTemp, Warning, TEXT("Give Ability"));
		}
	}
	OnDataRefresh();
}

float ABOS_ShipBlock::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	// TODO Skill Factor
	//auto finalDmg = Damage*AtkFactor - Def*DefFactor;

	//auto randf = FMath::RandRange(0.f, 1.f);
	//if (finalDmg > 0.f && randf <= CritcalRate)
	//{
	//	finalDmg *= CritcalDmg;
	//}

	////HP -= finalDmg;

	//HP = HP >= 0.f ? HP : 0.f;

	//if (HP == 0.f)
	//{
	//	auto rootActor = Cast<ABOS_ShipBlock>(GetRootActor());
	//	auto ps = Cast<ABOS_PlayerState>(rootActor->PlayerState);
	//	if (ps)
	//	{
	//		if (this == rootActor)
	//		{
	//			ps->bIsDead = true;
	//		}
	//		Destroy();
	//	}
	//}

	//UE_LOG(LogTemp, Warning, TEXT("%f DMG Taken"), finalDmg > 0.f ? finalDmg : 1.f);
	//return finalDmg > 0.f ? finalDmg : 1.f;
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

// Called every frame
void ABOS_ShipBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto controller = Cast<ABOS_PlayerController>(GetController());
	if (!PlayerState)
	{
		Gun->SetWorldRotation(GunRotator);
	}


	//auto world = GetWorld();
	//if (world && world->IsServer())
	//{
	//	auto attachRoot = Cast<ABOS_ShipBlock>(GetAttachParentActor());
	//	if (attachRoot && !(attachRoot->GetController()))
	//	{
	//		auto attachParent = GetAttachParentActor();
	//		if (attachParent && attachParent != this)
	//		{
	//			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//		}

	//		TArray<AActor*> attachChildren;
	//		this->GetAttachedActors(attachChildren);
	//		for (AActor* actor : attachChildren)
	//		{
	//			actor->DetachRootComponentFromParent(true);
	//		}
	//	}
	//}

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
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ABOS_ShipBlock::Shoot);
	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbiliyInputBinds("ConfirmInput", "CancelInput", "EAbilityInput"));
}

void ABOS_ShipBlock::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//if (HasAuthority() && !TestSkill)
	//{
	//	TestSkill = NewObject<UBOS_Skill>(this);
	//}
}

bool ABOS_ShipBlock::ReplicateSubobjects(UActorChannel * Channel, FOutBunch * Bunch, FReplicationFlags * RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	//if (TestSkill != nullptr)
	//{
	//	WroteSomething |= Channel->ReplicateSubobject(TestSkill, *Bunch, *RepFlags);
	//}

	return WroteSomething;
}

void ABOS_ShipBlock::Forward_Implementation(float Axis)
{
	ShipBody->AddForce(AttributeSet->ImpulseForce * forwardVec * Axis);

}
bool ABOS_ShipBlock::Forward_Validate(float Axis)
{
	return true;
}


void ABOS_ShipBlock::Right_Implementation(float Axis)
{
	ShipBody->AddForce(AttributeSet->ImpulseForce * rightVec * Axis);
}

bool ABOS_ShipBlock::Right_Validate(float Axis)
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

	ShipBody->AddAngularImpulse(FVector(0.f, 0.f, yaw * (-AttributeSet->AngularImpulse)));
}

void ABOS_ShipBlock::ShipBodyHit_Implementation(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	auto world = GetWorld();
	if (world && world->IsServer() && OtherActor)
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
	if (HasAuthority())
	{
		//this->AngularImpulse += AngularImpulseStepUp;
		//this->ImpulseForce += ImpulseForceStepUp;

		auto childrenCount = GetChildrenCount(this);
		AttributeSet->AngularImpulse = AttributeSet->AngularImpulseStepUp * (childrenCount + 1) + AttributeSet->AngularImpulseStepUp / (10 - childrenCount);
		AttributeSet->ImpulseForce = AttributeSet->ImpulseForceStepUp * (childrenCount + 1) + AttributeSet->AngularImpulseStepUp / (10 - childrenCount);
		auto other = Cast<ABOS_ShipBlock>(OtherActor);
		if (other)
		{
			other->TeamID = TeamID;
		}
	}
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
	while (ret && ret->GetOwner())
	{
		ret = Cast<ABOS_ShipBlock>(ret->GetOwner());
	}
	return ret;
}

ABOS_ShipBlock * ABOS_ShipBlock::GetAttachRootActor()
{
	ABOS_ShipBlock *ret = this;
	while (ret && ret->GetAttachParentActor())
	{
		ret = Cast<ABOS_ShipBlock>(ret->GetAttachParentActor());
	}
	return ret;
}

ABOS_ShipBlock * ABOS_ShipBlock::FindTarget_AI()
{
	//UE_LOG(LogTemp, Warning, TEXT("FindTarget_AI Start"));
	auto loc = GetActorLocation();
	TArray<UPrimitiveComponent *> comps;
	AI_SenceRange->GetOverlappingComponents(comps);
	TMap<FString, ABOS_ShipBlock*> blocks;
	for (auto comp : comps)
	{
		auto ship_block = Cast<ABOS_ShipBlock>(comp->GetOwner());
		if (IsEnemy(ship_block)
			)
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
	//UE_LOG(LogTemp, Warning, TEXT("FindTarget_AI End"));
	return nearest;
}

void ABOS_ShipBlock::TakeAim_AI(ABOS_ShipBlock *Enemy)
{
	//UE_LOG(LogTemp, Warning, TEXT("TakeAim_AI Start"));

	if (Enemy && !(Enemy->IsPendingKill()))
	{
		auto enemyLoc = Enemy->GetActorLocation();
		auto loc = GetActorLocation();

		auto rot = UKismetMathLibrary::FindLookAtRotation(loc, enemyLoc);
		this->GunRotator = rot;
		Gun->SetWorldRotation(rot);
	}
	//else
	//{
		//UE_LOG(LogTemp, Warning, TEXT("TakeAim_AI target is NULL"));
	//}

	//UE_LOG(LogTemp, Warning, TEXT("TakeAim_AI End"));

}


bool ABOS_ShipBlock::CanAttack()
{
	bool res = false;
	auto ps = GetAttachRootActor()->PlayerState;
	if (ps)
	{
		res = true;
	}
	return res;
}

int32 ABOS_ShipBlock::GetChildrenCount(AActor *ref)
{
	int32 count = 0;
	int32 c = ref->Children.Num();
	if (c > 0)
	{
		for (auto actor : ref->Children)
		{
			c += GetChildrenCount(actor);
		}
		count += c;
	}

	return count;
}

void ABOS_ShipBlock::SwitchAttackAbility_Implementation(int32 index)
{
	FGameplayAbilitySpec *AbilitySpec = nullptr;
	AbilitySpec = AbilitySystem->FindAbilitySpecFromInputID(0);
	AbilitySystem->ClearAbility(AbilitySpec->Handle);
	switch (index)
	{
	case 0:
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(AtkAbility.GetDefaultObject(), 1, 0));
		break;
	case 1:
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(ElectricShockAbility.GetDefaultObject(), 1, 0));
		break;
	default:
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(AtkAbility.GetDefaultObject(), 1, 0));
		break;
	}
}

bool ABOS_ShipBlock::SwitchAttackAbility_Validate(int32 index)
{
	return true;
}

void ABOS_ShipBlock::ElectricShock_Implementation(ABOS_ShipBlock *Enemy)
{
	if (HasAuthority())
	{
		auto effectToApply = Cast<UGameplayEffect>(DMGEffect.GetDefaultObject());
		AbilitySystem->ApplyGameplayEffectToTarget(effectToApply, Enemy->AbilitySystem);
		UE_LOG(LogTemp, Warning, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	}
	ElectricShockEffect(Enemy);
}

void ABOS_ShipBlock::OnDataRefresh_Implementation()
{
	if (bDebugMode)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ImpulseForce: %f"), AttributeSet->ImpulseForce));
	}

	if (AttributeSet->BaseAttackPower <= 0.f)
	{
		Gun->SetVisibility(false);
	}

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

bool ABOS_ShipBlock::RotateGun_Validate(float Axis)
{
	return true;
}

void ABOS_ShipBlock::Shoot_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shoot Start")); 
	//Shoot_Server();
	/*if (Atk > 0.f)
	{
		TestSkill->BeginCast();
	}*/
	auto world = GetWorld();
	if (world)
	{
		if (AttributeSet->BaseAttackPower > 0.f)
		{
			Shoot_Multi();
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Shoot End"));
}
bool ABOS_ShipBlock::Shoot_Validate()
{
	return true;
}

void ABOS_ShipBlock::Shoot_Multi_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shoot_Server Start"));

	auto world = GetWorld();
	if (world)
	{
		auto remaining = world->GetTimerManager().GetTimerRemaining(ShootTimer);
		if (remaining <= 0.f)
		{
			world->GetTimerManager().SetTimer(ShootTimer, 1.f, false);
			auto trans = Gun->GetSocketTransform(TEXT("Gun"));
			auto loc = trans.GetLocation();
			auto rot = FRotator(trans.GetRotation());
			FActorSpawnParameters sp;
			sp.Instigator = this;
			auto actor = world->SpawnActor(ProjectileClass, &loc, &rot, sp);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Shoot_Server End"));
}


void ABOS_ShipBlock::Attack_Implementation()
{
	if (bDebugMode)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT(__FUNCTION__)));
	}
	
}
bool ABOS_ShipBlock::Attack_Validate()
{
	return true;
}

void ABOS_ShipBlock::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABOS_ShipBlock, GunRotator);
	//DOREPLIFETIME(ABOS_ShipBlock, HP);
	//DOREPLIFETIME(ABOS_ShipBlock, MaxHP);
	//DOREPLIFETIME(ABOS_ShipBlock, Atk);
	//DOREPLIFETIME(ABOS_ShipBlock, Def);
	//DOREPLIFETIME(ABOS_ShipBlock, CritcalRate);
	//DOREPLIFETIME(ABOS_ShipBlock, CritcalDmg);
	//DOREPLIFETIME(ABOS_ShipBlock, TestSkill);
	DOREPLIFETIME(ABOS_ShipBlock, TeamID);

}

ABOS_ShipBlock * FHexTree::GetOppositeLeaf(ABOS_ShipBlock *Leaf)
{
	ABOS_ShipBlock *res = nullptr;
	if (Leaf == L)
	{
		res = R;
	}
	else if (Leaf == R)
	{
		res = L;
	}
	else if (Leaf == LU)
	{
		res = RD;
	}
	else if (Leaf == RU)
	{
		res = LD;
	}
	else if (Leaf == LD)
	{
		res = RU;
	}
	else if (Leaf == RD)
	{
		res = LU;
	}
	return res;
}

bool ABOS_ShipBlock::IsEnemy(ABOS_ShipBlock *ship_block)
{
	return ship_block && ship_block->TeamID != TeamID
		&& ship_block->GetRootActor()->PlayerState
		&& ship_block->GetRootActor() != GetRootActor();
}