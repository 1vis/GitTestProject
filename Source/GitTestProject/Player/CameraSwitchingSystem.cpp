// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraSwitchingSystem.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values
ACameraSwitchingSystem::ACameraSwitchingSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraSwitchingSystem::BeginPlay()
{
	Super::BeginPlay();

	InitKeyMapping();
}

void ACameraSwitchingSystem::InitKeyMapping()
{
	APlayerController* TmpPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (TmpPlayerController)
	{
		EnableInput(TmpPlayerController);
		if (InputComponent)
		{
			InputComponent->BindAction("SwitchCameraTarget", IE_Pressed, this, &ACameraSwitchingSystem::SwitchCameraTarget);
		}
	}
}

// Called every frame
void ACameraSwitchingSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraSwitchingSystem::SwitchCameraTarget()
{
	if (ControllableCamera)
	{
		CurrentSwitchPosition++;

		if (int(CurrentSwitchPosition) >= ArrayOfSwitchableActors.Num())
			CurrentSwitchPosition = 0;

		if (ControllableCamera)
		{
			ControllableCamera->AttachCameraToObject(ArrayOfSwitchableActors[CurrentSwitchPosition]);
		}
	}
}

