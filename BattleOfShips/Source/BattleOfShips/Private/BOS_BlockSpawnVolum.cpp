// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "Kismet/KismetMathLibrary.h"
#include "BOS_CombatGameMode.h"
#include "BOS_BlockSpawnVolum.h"


// Sets default values
ABOS_BlockSpawnVolum::ABOS_BlockSpawnVolum()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnVolum = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolum"));
	SpawnVolum->SetBoxExtent(FVector(1000.f, 1000.f, 10.f));
	RootComponent = SpawnVolum;

}

// Called when the game starts or when spawned
void ABOS_BlockSpawnVolum::BeginPlay()
{
	Super::BeginPlay();

	auto world = GetWorld();
	if (world && world->IsServer() && HasAuthority())
	{
		InitialSpawn();
		//FTimerHandle initTimerHandle;
		//world->GetTimerManager().SetTimer(initTimerHandle, this, &ABOS_BlockSpawnVolum::InitialSpawn, 0.1f);
		world->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ABOS_BlockSpawnVolum::OnSpawn, SpawnDelay, true);
	}
}

// Called every frame
void ABOS_BlockSpawnVolum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABOS_BlockSpawnVolum::OnSpawn()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s begin"), ANSI_TO_TCHAR(__FUNCTION__));

	auto origin = SpawnVolum->Bounds.Origin;
	auto boxExtent = SpawnVolum->Bounds.BoxExtent;


	auto world = GetWorld();
	if (world && world->IsServer())
	{
		
		FActorSpawnParameters sp;
		sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
		AActor *block = nullptr;
		auto mapCount = SpawnMap.Num();
		while (!block && mapCount)
		{
			for (auto m : SpawnMap)
			{
				float rand = FMath::RandRange(0.f, 1.f);
				if (rand < m.Value)
				{
					auto spawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(origin, boxExtent);
					block = world->SpawnActor(m.Key, &spawnPoint, &FRotator::ZeroRotator, sp);
					if (block)
					{
						break;
					}
				}
			}

		}
		
		//if (SpawnClasses.Num() > 0)
		//{
		//	auto randIndex = FMath::RandHelper(3);
		//	/*auto spawnClass = SpawnClasses[randIndex];
		//	auto actor = world->SpawnActor(spawnClass, &spawnPoint, &FRotator::ZeroRotator, sp);*/

		//	randIndex = (randIndex + 1) * 100 + 1;
		//	//UE_LOG(LogTemp, Warning, TEXT("randIndex:%d"), randIndex);
		//	auto gm = Cast<ABOS_CombatGameMode>(world->GetAuthGameMode());
		//	gm->ShipBlockFactory(randIndex, spawnPoint, FRotator::ZeroRotator);
		//}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s end"), ANSI_TO_TCHAR(__FUNCTION__));
}

void ABOS_BlockSpawnVolum::InitialSpawn()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s begin"), ANSI_TO_TCHAR(__FUNCTION__));

	auto i = InitialSpawnNum;

	while (i--)
	{
		OnSpawn();
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s end"), ANSI_TO_TCHAR(__FUNCTION__));

}

