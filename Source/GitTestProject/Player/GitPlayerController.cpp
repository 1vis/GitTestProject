// Fill out your copyright notice in the Description page of Project Settings.

#include "GitPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CameraBase.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Utilities/GitStatics.h"
#include "Player/GitCharacter.h"
#include "Utilities/GitCore.h"
#include "Player/GitPawn.h"


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

	// Set ActiveCamera to the first ABaseCamera found in level.
	if (ActiveCamera == nullptr)
	{
		TArray<AActor*> CamerasInLevel;
		UGameplayStatics::GetAllActorsOfClass(this, ACameraBase::StaticClass(), CamerasInLevel);

		if (CamerasInLevel.Num() > 0)
		{
			ActiveCamera = Cast<ACameraBase>(CamerasInLevel[0]);		
		}	
	}

	SetViewTarget(ActiveCamera);
}

void AGitPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();	

	InputComponent->BindAction("MoveToDestination", IE_Released, this, &AGitPlayerController::MoveSelectedPawnToDestination);
}

void AGitPlayerController::MoveSelectedPawnToDestination()
{
	for (auto& SelectedPawn : SelectedPawns)
	{
		if (SelectedPawn)
		{
			if (AGitCharacter* GitCharacter = Cast<AGitCharacter>(SelectedPawn))
			{
				GitCharacter->SetDestinationToMouseCursor();
				GitCharacter->MoveGitCharacterToDestination();
			}
			else if (AGitPawn* GitPawn = Cast<AGitPawn>(SelectedPawn))
			{
				GitPawn->SetDestinationToMouseCursor();
				GitPawn->MoveGitPawnToDestination();
			}
			else
			{
				PRINTC("Selected Pawn isn't GitPawn/GitCharacter. (GitPlayerController, MoveSelectedPawnToDestination)", FColor::Red);
			}
		}
		else
		{
			PRINTC("No pawn is selected. (GitPlayerController, MoveSelectedPawnToDestination)", FColor::Red);
		}
	}
}


