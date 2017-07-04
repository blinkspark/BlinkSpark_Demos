// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BOS_ShipBlock.h"
#include "BOS_BlockSpawnVolum.generated.h"

UCLASS()
class BATTLEOFSHIPS_API ABOS_BlockSpawnVolum : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABOS_BlockSpawnVolum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UBoxComponent *SpawnVolum;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		TMap<TSubclassOf<ABOS_ShipBlock>, float> SpawnMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		float SpawnDelay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		int32 InitialSpawnNum = 20;


	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void OnSpawn();
	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void InitialSpawn();

};
