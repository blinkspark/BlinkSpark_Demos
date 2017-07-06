// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemInterface.h"
#include "BOS_ShipBlock.generated.h"

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
	UseAbility1 UMETA(DisplayName = "Use Spell 1"), //This maps the first ability(input ID should be 0 in int) to the action mapping(which you define in the project settings) by the name of "UseAbility1". "Use Spell 1" is the blueprint name of the element.
	UseAbility2 UMETA(DisplayName = "Use Spell 2"), //Maps ability 2(input ID 1) to action mapping UseAbility2. "Use Spell 2" is mostly used for when the enum is a blueprint variable.
	UseAbility3 UMETA(DisplayName = "Use Spell 3"),
	UseAbility4 UMETA(DisplayName = "Use Spell 4"),
	WeaponAbility UMETA(DisplayName = "Use Weapon"), //This finally maps the fifth ability(here designated to be your weaponability, or auto-attack, or whatever) to action mapping "WeaponAbility".

	 //You may also do something like define an enum element name that is not actually mapped to an input, for example if you have a passive ability that isn't supposed to have an input. This isn't usually necessary though as you usually grant abilities via input ID,
	 //which can be negative while enums cannot. In fact, a constant called "INDEX_NONE" exists for the exact purpose of rendering an input as unavailable, and it's simply defined as -1.
	 //Because abilities are granted by input ID, which is an int, you may use enum elements to describe the ID anyway however, because enums are fancily dressed up ints.
};

USTRUCT(BlueprintType)
struct FHexTree
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		class ABOS_ShipBlock *L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		class ABOS_ShipBlock *R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		class ABOS_ShipBlock *LU;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		class ABOS_ShipBlock *RU;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		class ABOS_ShipBlock *LD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexTree")
		class ABOS_ShipBlock *RD;

	class ABOS_ShipBlock *GetOppositeLeaf(class ABOS_ShipBlock *Leaf);
};

UCLASS(config = Game)
class BATTLEOFSHIPS_API ABOS_ShipBlock : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABOS_ShipBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY()
		FVector forwardVec;

	UPROPERTY()
		FVector rightVec;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	UAbilitySystemComponent* GetAbilitySystemComponent() const override //We add this function, overriding it from IAbilitySystemInterface.
	{
		return AbilitySystem;
	};

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "ShipBlock|Controlls")
		void Forward(float Axis);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "ShipBlock|Controlls")
		void Right(float Axis);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "ShipBlock|Controlls")
		void RotateGun(float Axis);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "ShipBlock|Controlls")
		void Shoot();
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ShipBlock|Multicast")
		void Shoot_Multi();

	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Logic")
		void FollowV();

	UFUNCTION(BlueprintNativeEvent, Category = "ShipBlock|Logic")
		void ShipBodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable, Category = "ShipBlock|Multicast")
		void OnAttach(AActor *OtherActor, FName SocketName);
	UFUNCTION(BlueprintImplementableEvent, Category = "ShipBlock|Logic")
		void OnAttachBlueprintDelegate(AActor *OtherActor, FName SocketName);

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
		FName GetSocketNameByAngle(float Angle);

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
		ABOS_ShipBlock *GetRootActor();

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
		ABOS_ShipBlock *GetAttachRootActor();

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|AI")
		ABOS_ShipBlock *FindTarget_AI();

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
		bool IsEnemy(ABOS_ShipBlock * ship_block);
	
	UFUNCTION(BlueprintCallable, Category = "ShipBlock|AI")
		void TakeAim_AI(ABOS_ShipBlock *Enemy);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ShipBlock|Logic")
		void OnDataRefresh();

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
		bool CanAttack();

	UFUNCTION(BlueprintCallable, Category = "ShipBlock|Logic")
		int32 GetChildrenCount(AActor *ref);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ShipBlock|Ability")
		void ElectricShock(ABOS_ShipBlock *Enemy);

	UFUNCTION(BlueprintImplementableEvent, Category = "ShipBlock|Ability|Effect")
		void ElectricShockEffect(ABOS_ShipBlock *Enemy);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "ShipBlock|Ability")
		void SwitchAttackAbility(int32 index);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
		uint32 bDebugMode : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
		uint32 bUseNewAttribute : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	//	UDataTable *TestDataTable;


	/** BP ReadOnly */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent *CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent *Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent *ShipBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USphereComponent *AI_SenceRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		FTimerHandle ShootTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent *AbilitySystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> AtkAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> HealAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> ElectricShockAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		class UGameplayAbilitySet *AbilitySet;

	/** BP Editable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float ImpulseForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float AngularImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float DeltaNormalizer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float GunRotateDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ABOS_Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float AngularImpulseStepUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float ImpulseForceStepUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		class UBOS_AttributeSet *AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UGameplayEffect> DMGEffect;


	/****************  Replicated  **************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		FRotator GunRotator;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float Atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float CritcalRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float CritcalDmg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		class UBOS_Skill *TestSkill;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		int32 TeamID;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	//	class UBlinkCombatSystemComponent *BlinkCombatSystemComponent;
};
