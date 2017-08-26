// Fill out your copyright notice in the Description page of Project Settings.

#include "GitCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/GitCore.h"
#include "Utilities/GitStatics.h"
#include "Player/CameraBase.h"
#include "Player/GitPlayerController.h"
#include "Player/GitAIController.h"


// Sets default values
AGitCharacter::AGitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ClickableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClickableMesh"));
	ClickableMesh->bVisible = true;
	ClickableMesh->bHiddenInGame = true;	
}

// Called when the game starts or when spawned
void AGitCharacter::BeginPlay()
{
	Destination = GetActorLocation();

	Super::BeginPlay();	
}

// Called every frame
void AGitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGitCharacter::NotifyActorOnReleased(FKey ButtonReleased)
{
	Super::NotifyActorOnReleased(ButtonReleased);

	AGitPlayerController* PC = UGitStatics::GetGitPlayerController(this);
	PC->SelectedPawn = this;	
}

void AGitCharacter::SetDestinationToMouseCursor()
{
	AGitPlayerController* PC = UGitStatics::GetGitPlayerController(this);
	if (PC->ActiveCamera == nullptr)
	{
		PRINTC("Camera not found. (GitCharacter, GetDestination())", FColor::Red);
		return;
	}	
		
	FVector WorldLocation,WorldDirection;	
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	WorldDirection *= 10'000.0f;	
		
	FVector TraceStart = PC->ActiveCamera->Camera->GetComponentLocation();
	FVector TraceEnd = TraceStart + WorldDirection;
		
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	
	Destination = HitResult.Location;	
}

FVector AGitCharacter::GetDestination() const
{
	return Destination;
}

void AGitCharacter::MoveGitCharacterToDestination()
{
	AGitAIController* GitAIController = Cast<AGitAIController>(GetController());
	
	if (GitAIController)
	{		
		GitAIController->MoveToLocation(Destination);
	}	
	else
	{
		PRINTC("Wrong AI Controller (GitCharacter, MoveGitCharacterToDestination())", FColor::Red);
	}
}
