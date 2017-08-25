// Fill out your copyright notice in the Description page of Project Settings.

#include "GitPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CameraBase.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Utilities/GitStatics.h"
#include "Player/GitCharacter.h"


void AGitPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> CamerasInLevel;
	UGameplayStatics::GetAllActorsOfClass(this, TSubclassOf<ACameraBase>(), CamerasInLevel);

	if (ACameraBase* Camera = Cast<ACameraBase>(CamerasInLevel[0]))
	{
		ActiveCamera = Camera;
	}	

}

void AGitPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SelectObject", IE_Pressed, this, &AGitPlayerController::On_LMB_Pressed).bConsumeInput = false;
	InputComponent->BindAction("MoveToLocation", IE_Pressed, this, &AGitPlayerController::On_RMB_Pressed).bConsumeInput = false;
}

void AGitPlayerController::On_LMB_Pressed()
{
	//AActor* SelectedActor = GetClickedOnActor();	
}

void AGitPlayerController::On_RMB_Pressed()
{	
}

//AActor* AGitPlayerController::GetClickedOnActor()
//{
//	check(ActiveCamera && "Active camera not found. (GitPlayerController)");
//	
//	FVector WorldLocation,WorldDirection;
//	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
//	WorldDirection *= 10'000.0f;	
//	
//	FVector TraceStart = ActiveCamera->GetActorLocation();
//	FVector TraceEnd = TraceStart + WorldDirection;
//	
//	FHitResult HitResult;
//	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
//
//	return HitResult.GetActor();
//}
