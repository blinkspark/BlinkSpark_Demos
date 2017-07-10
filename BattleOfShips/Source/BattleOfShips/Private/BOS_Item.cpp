// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_Item.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BOS_ShipBlock.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"


// Sets default values
ABOS_Item::ABOS_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	DebugMode = false;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetSphereRadius(200.f);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABOS_Item::OnOverlapBegin);
	RootComponent = Trigger;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	ParticalSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticalSystem"));
	ParticalSystem->SetupAttachment(RootComponent);

	PMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMovement"));
}

// Called when the game starts or when spawned
void ABOS_Item::BeginPlay()
{
	Super::BeginPlay();

	if (DebugMode && GWorld)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT(__FUNCTION__)));
	}
	
}

// Called every frame
void ABOS_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABOS_Item::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DebugMode)
	{
		UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
		//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT(__FUNCTION__)));
	}
	auto otherComp = Cast<UStaticMeshComponent>(OtherComp);
	auto otherActor = Cast<ABOS_ShipBlock>(OtherActor);
	if (otherComp && otherActor)
	{
		auto otherRoot = otherActor->GetRootActor();
		if (otherRoot && otherRoot->PlayerState)
		{
			auto effect = EffectToApply->GetDefaultObject<UGameplayEffect>();
			if (effect)
			{
				if (HasAuthority())
				{
					auto ab = otherRoot->AbilitySystem;
					auto context = ab->MakeEffectContext();
					ab->ApplyGameplayEffectToSelf(effect, 0, context);
					otherRoot->AbilityIndex = AbilityIndex;
				}
				//otherActor->AbilitySystem->ApplyGameplayEffectToS
				Destroy();
			}
		}
	}
}

