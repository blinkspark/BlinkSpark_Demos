// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_ShipBlock.h"
#include "BOS_Projectile.h"


// Sets default values
ABOS_Projectile::ABOS_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BodyCollision"));
	RootComponent = BodyCollision;
	BodyCollision->SetSphereRadius(CollisionRadius);
	BodyCollision->SetSimulatePhysics(true);
	BodyCollision->OnComponentHit.AddDynamic(this, &ABOS_Projectile::OnBodyHit);
	BodyCollision->GetBodyInstance()->bNotifyRigidBodyCollision = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	ParticleEffect->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABOS_Projectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

	auto forward = RootComponent->GetForwardVector();
	auto instigator = Cast<ABOS_ShipBlock>(GetInstigator());
	auto parentSpeed = instigator ? instigator->GetVelocity() : FVector();
	BodyCollision->SetPhysicsLinearVelocity(forward * LaunchSpeed + parentSpeed);

}

void ABOS_Projectile::OnBodyHit_Implementation(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
	Destroy();
	BodyCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ABOS_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

