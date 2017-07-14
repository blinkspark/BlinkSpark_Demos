// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BOS_TeamplateSpawner.generated.h"

UCLASS()
class BATTLEOFSHIPS_API ABOS_TeamplateSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABOS_TeamplateSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TemplateSpawn")
		uint8 bDebugMode : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TemplateSpawn")
		class UHexTemplateTree *Template;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent *Root;
};
