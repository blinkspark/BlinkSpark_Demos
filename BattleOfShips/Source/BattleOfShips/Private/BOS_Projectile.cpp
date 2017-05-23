// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_ShipBlock.h"
#include "DummyObj.h"
#include "BOS_Projectile.h"


// Sets default values
ABOS_Projectile::ABOS_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BodyCollision"));
	RootComponent = BodyCollision;
	BodyCollision->SetSphereRadius(CollisionRadius);
	//BodyCollision->OnComponentHit.AddDynamic(this, &ABOS_Projectile::OnBodyHit);
	BodyCollision->OnComponentBeginOverlap.AddDynamic(this, &ABOS_Projectile::OnBodyHit);
	BodyCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	ParticleEffect->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void ABOS_Projectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

	auto forward = RootComponent->GetForwardVector();
	auto instigator = Cast<ABOS_ShipBlock>(GetInstigator());
	auto parentSpeed = instigator ? instigator->GetVelocity() : FVector();

	ProjectileMovement->SetVelocityInLocalSpace(parentSpeed);
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->SetVelocityInLocalSpace(FVector(1000.f, 0.f, 0.f));
	ProjectileMovement->UpdateComponentVelocity();

}

void ABOS_Projectile::OnBodyHit_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto otherShipBlock = Cast<ABOS_ShipBlock>(OtherActor);
	if (otherShipBlock) {
		auto otherRootActor = otherShipBlock->GetRootActor();
		if (otherRootActor != GetInstigator()) {
			FDamageEvent de;
			de.DamageTypeClass = UBOS_DamageTypeKinect::StaticClass();
			otherRootActor->TakeDamage(10.f, de, GetInstigator()->GetController(), GetInstigator());
			Destroy();
			BodyCollision->SetSimulatePhysics(false);
			BodyCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

// Called every frame
void ABOS_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*FVector dir;
	float length;
	GetVelocity().ToDirectionAndLength(dir, length);
	UE_LOG(LogTemp, Warning, TEXT("Velocity: %f"), length);*/
}

