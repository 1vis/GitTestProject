// Fill out your copyright notice in the Description page of Project Settings.

#include "GitPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CameraBase.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Utilities/GitStatics.h"
#include "Player/GitCharacter.h"
#include "Utilities/GitCore.h"


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
	
	/// Set SelectedPawn to the first AGitCharacter found in level.
	//if (SelectedPawn == nullptr)
	//{
	//	TArray<AActor*> GitCharactersInLevel;
	//	UGameplayStatics::GetAllActorsOfClass(this, AGitCharacter::StaticClass(), GitCharactersInLevel);

	//	if (GitCharactersInLevel.Num() > 0)
	//	{
	//		SelectedPawn = Cast<AGitCharacter>(GitCharactersInLevel[0]);
	//	}
	//}	
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
	if (SelectedPawn)
	{
		AGitCharacter* GitCharacter = Cast<AGitCharacter>(SelectedPawn);
		if (GitCharacter)
		{
			GitCharacter->SetDestinationToMouseCursor();
			GitCharacter->MoveGitCharacterToDestination();
		}
		else
		{
			PRINTC("Selected Pawn isn't GitCharacter. (GitPlayerController, MoveSelectedPawnToDestination)", FColor::Red);
		}
	}
	else
	{
		PRINTC("No pawn is selected. (GitPlayerController, MoveSelectedPawnToDestination)", FColor::Red);
	}
}


