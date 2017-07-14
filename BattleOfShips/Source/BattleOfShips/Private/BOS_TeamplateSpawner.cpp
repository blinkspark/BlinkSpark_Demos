// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_TeamplateSpawner.h"
#include "BOS_ShipBlock.h"


// Sets default values
ABOS_TeamplateSpawner::ABOS_TeamplateSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	bDebugMode = false;

}

// Called when the game starts or when spawned
void ABOS_TeamplateSpawner::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters sp;
	sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	auto loc = GetActorLocation();
	auto world = GetWorld();
	if (world && HasAuthority())
	{
		auto actor = world->SpawnActor(Template->Content, &loc, &FRotator::ZeroRotator, sp);
		if (bDebugMode)
		{
			if (actor)
			{
				UE_LOG(LogTemp, Warning, TEXT("actor spawned"));
			}
		}
		if (actor)
		{
			auto rootActor = CastChecked<ABOS_ShipBlock>(actor);
			rootActor->ShipBody->SetSimulatePhysics(false);
			loc.X += 200.f;
			float delay = 0.2f;
			for (auto ite : Template->leaves)
			{
				auto a = Cast<ABOS_ShipBlock>(world->SpawnActor(ite.Value->Content, &loc, &FRotator::ZeroRotator, sp));
				if (a)
				{
					FTimerHandle handle;
					world->GetTimerManager().SetTimer(handle, [a, ite, rootActor]() {
						a->ShipBody->SetSimulatePhysics(false);
						rootActor->OnAttach(a, ite.Key);
					}, delay, false);
					delay += 0.2f;
				}
			}
			rootActor->ShipBody->SetSimulatePhysics(true);
		}
	}
}

// Called every frame
void ABOS_TeamplateSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

