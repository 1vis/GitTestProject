// Fill out your copyright notice in the Description page of Project Settings.

#include "GitPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CameraBase.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Utilities/GitStatics.h"
#include "Player/GitCharacter.h"


AGitPlayerController::AGitPlayerController()
{
	bAllowTickBeforeBeginPlay = false;
	bShowMouseCursor = true;
	bEnableTouchEvents = false;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AGitPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ActiveCamera == nullptr)
	{
		TArray<AActor*> CamerasInLevel;
		UGameplayStatics::GetAllActorsOfClass(this, TSubclassOf<ACameraBase>(), CamerasInLevel);

		if (CamerasInLevel.Num() > 0)
		{
			ActiveCamera = Cast<ACameraBase>(CamerasInLevel[0]);
		}
	}
	
	if (SelectedPawn == nullptr)
	{
		TArray<AActor*> GitCharactersInLevel;
		UGameplayStatics::GetAllActorsOfClass(this, TSubclassOf<AGitCharacter>(), GitCharactersInLevel);

		if (GitCharactersInLevel.Num() > 0)
		{
			SelectedPawn = Cast<AGitCharacter>(GitCharactersInLevel[0]);
		}
	}	
}

void AGitPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();	
}


