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

	if (SelectBoxMode)
		UpdateSelectionBoxData(DeltaTime);
}

void AGitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();	

	InputComponent->BindAction("MoveToDestination", IE_Released, this, &AGitPlayerController::MoveSelectedPawnToDestination);

	InputComponent->BindAction("SelectionMode", IE_Pressed, this, &AGitPlayerController::StartSelectionMode);
	InputComponent->BindAction("SelectionMode", IE_Released, this, &AGitPlayerController::StopSelectionMode);
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

void AGitPlayerController::HighlightCharacterPanels(TArray<APawn*> SelectedActors)
{
	UpdateCharacterPanelsFunction.Broadcast(SelectedActors);
}

void AGitPlayerController::StartSelectionMode()
{
	SelectBoxMode = true;
	GetMousePosition(SelectionBoxX, SelectionBoxY);

}

void AGitPlayerController::StopSelectionMode()
{
	SelectBoxMode = false;
	SelectionBoxX = 0.0f;
	SelectionBoxY = 0.0f;
	SelectionBoxW = 0.0f;
	SelectionBoxH = 0.0f;
}

void AGitPlayerController::UpdateSelectionBoxData(const float& DeltaTime)
{
	float tmpX = 0.0f;
	float tmpY = 0.0f;

	GetMousePosition(tmpX, tmpY);

	SelectionBoxW = tmpX - SelectionBoxX;
	SelectionBoxH = tmpY - SelectionBoxY;

}

void AGitPlayerController::SetSelectedActors(TArray<APawn*> SelectedActors)
{
	SelectedPawns = SelectedActors;
}

bool AGitPlayerController::IsInSelectionBox(APawn * SelectedActor)
{

	FVector position = GetHUD()->Project(SelectedActor->GetActorLocation());
	float BoxW = SelectionBoxW;
	float BoxH = SelectionBoxH;

	float BoxX = SelectionBoxX;
	float BoxY = SelectionBoxY;

	if (BoxW < 0)
	{
		BoxX += BoxW;
		BoxW *= -1.0f;
	}

	if (BoxH < 0)
	{
		BoxY += BoxH;
		BoxH *= -1.0f;
	}

	return (position.X < BoxX + BoxW && position.X + 1.0f > BoxX && position.Y < BoxY + BoxH && position.Y + +1.0f > BoxY);
}