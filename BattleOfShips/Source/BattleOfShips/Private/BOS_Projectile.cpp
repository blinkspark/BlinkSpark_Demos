// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_ShipBlock.h"
#include "DummyObj.h"
#include "UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "BOS_Projectile.h"


// Sets default values
ABOS_Projectile::ABOS_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = true;

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

	LaunchSpeed = 800.f;
}

// Called when the game starts or when spawned
void ABOS_Projectile::BeginPlay()
{
	Super::BeginPlay();

	auto world = GetWorld();
	if (world)
	{
		auto forward = RootComponent->GetForwardVector();
		auto instigator = Cast<ABOS_ShipBlock>(GetInstigator());
		auto speed = instigator ? instigator->GetVelocity() : FVector();

		ProjectileMovement->MaxSpeed = 0.f;
		speed.X += LaunchSpeed;
		ProjectileMovement->SetVelocityInLocalSpace(speed);
		UE_LOG(LogTemp, Warning, TEXT("speed: %f"), speed.X);
		ProjectileMovement->UpdateComponentVelocity();

		//DMG = instigator ? instigator->Atk : 160.f;
	}
}

void ABOS_Projectile::OnBodyHit_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto world = GetWorld();
	auto otherShipBlock = Cast<ABOS_ShipBlock>(OtherActor);
	if (otherShipBlock)
	{
		auto otherRootActor = otherShipBlock->GetRootActor();
		auto instigate = Cast<ABOS_ShipBlock>(GetInstigator());
		auto root = instigate->GetRootActor();
		auto defObj = Cast<UGameplayEffect>(EffectToApply->GetDefaultObject());
		if (otherRootActor != root && defObj)
		{
			
			if (world->IsServer())
			{
				//UGameplayStatics::ApplyDamage(OtherActor, DMG, GetInstigator()->GetController(), GetInstigator(), UBOS_DamageTypeKinect::StaticClass());
				auto otherAS = otherShipBlock->AbilitySystem;
				auto ownAS = instigate->AbilitySystem->ApplyGameplayEffectToTarget(defObj, otherAS);
			}
			BodyCollision->SetSimulatePhysics(false);
			BodyCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Destroy();
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

//void ABOS_Projectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(ABOS_Projectile, DMG);
//}