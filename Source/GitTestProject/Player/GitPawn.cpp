// Fill out your copyright notice in the Description page of Project Settings.

#include "GitPawn.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/GitCore.h"
#include "Utilities/GitStatics.h"
#include "Player/CameraBase.h"
#include "Player/GitPlayerController.h"
#include "Player/GitAIController.h"

// Sets default values
AGitPawn::AGitPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGitPawn::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AGitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGitPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AGitPawn::NotifyActorOnReleased(FKey ButtonReleased)
{
	Super::NotifyActorOnReleased(ButtonReleased);	

	AGitPlayerController* GitPC = UGitStatics::GetGitPlayerController(this);
	if (GitPC == nullptr)
	{
		PRINTC("PlayerController is not GitPlayerController. (GitCharacter, NotifyActorOnReleased)", FColor::Red);
		return;
	}

	float bLeftShiftDown = GitPC->GetInputAnalogKeyState(EKeys::LeftShift);
	float bLeftControlDown = GitPC->GetInputAnalogKeyState(EKeys::LeftControl);

	if (bLeftShiftDown == 1.0f)
	{
		int32 Index = GitPC->SelectedPawns.AddUnique(this);
		PRINTC(GitPC->SelectedPawns[Index]->GetName() + " selected.", FColor::Yellow);
	}
	else if (bLeftControlDown == 1.0f)
	{
		int32 Count = GitPC->SelectedPawns.Remove(this);
		if (Count > 0)
		{
			PRINTC(this->GetName() + " removed " + FString::FromInt(Count) + "x", FColor::Yellow);
		}
	}
	else
	{
		GitPC->SelectedPawns.Empty();
		int32 Index = GitPC->SelectedPawns.AddUnique(this);
		PRINTC(GitPC->SelectedPawns[Index]->GetName() + " selected.", FColor::Yellow);
	}
}

void AGitPawn::SetDestinationToMouseCursor()
{
	AGitPlayerController* PC = UGitStatics::GetGitPlayerController(this);
	if (PC->ActiveCamera == nullptr)
	{
		PRINTC("Camera not found. (GitPawn, GetDestination())", FColor::Red);
		return;
	}	
		
	FVector WorldLocation,WorldDirection;	
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	WorldDirection *= 100.0f * 100.0f;	
		
	FVector TraceStart = PC->ActiveCamera->Camera->GetComponentLocation();
	FVector TraceEnd = TraceStart + WorldDirection;
		
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		Destination = HitResult.Location;
	}	
}

FVector AGitPawn::GetDestination() const
{
	return Destination;
}

void AGitPawn::MoveGitPawnToDestination()
{
	AGitAIController* GitAIController = Cast<AGitAIController>(GetController());
	
	if (GitAIController)
	{		
		GitAIController->MoveToLocation(Destination);
	}	
	else
	{
		PRINTC("Wrong AI Controller (GitPawn, MoveGitCharacterToDestination())", FColor::Red);
	}
}

