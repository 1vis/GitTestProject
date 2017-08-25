// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraBase.h"
#include "CameraSwitchingSystem.generated.h"
UCLASS()
class GITTESTPROJECT_API ACameraSwitchingSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraSwitchingSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SwitchCameraTarget();

	UFUNCTION()
	void InitKeyMapping();

private:

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ArrayOfSwitchableActors;
	
	UPROPERTY(EditAnywhere)
	unsigned int CurrentSwitchPosition = 0;

	UPROPERTY(EditAnywhere)
	ACameraBase* ControllableCamera;
};
