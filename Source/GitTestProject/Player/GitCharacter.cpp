// Fill out your copyright notice in the Description page of Project Settings.

#include "GitCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/GitCore.h"
#include "Utilities/GitStatics.h"
#include "Player/CameraBase.h"
#include "Player/GitPlayerController.h"
#include "Player/GitAIController.h"
#include "Player/GitCharacter.h"
#include "Kismet/KismetInputLibrary.h"

// Sets default values
AGitCharacter::AGitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;	
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

	if ((!EnemyTarget && IsAttacking))
	{
		IsAttacking = false;
	}
}

// Called to bind functionality to input
void AGitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGitCharacter::NotifyActorOnReleased(FKey ButtonReleased)
{
	Super::NotifyActorOnReleased(ButtonReleased);

	if (Controllable)
	{
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

 		//Check the UI panels
		GitPC->HighlightCharacterPanels(GitPC->SelectedPawns);	
	}

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

	if (IsAttacking)
	{
		IsAttacking = false;
		EnemyTarget = nullptr;

		if (AbleToFire)
			AbleToFire = false;
	}

	if (IsInteracting)
	{
		IsInteracting = false;
		TargetItem = false;
	}
}

bool AGitCharacter::HasAssignedPanel()
{
	return AssignedPanel;
}

void AGitCharacter::AssignPanel()
{
	AssignedPanel = true;
}

void AGitCharacter::SetEnemyTarget(APawn* EnemyPawn)
{
	IsAttacking = true;
	EnemyTarget = EnemyPawn;
}

void AGitCharacter::HandleDamage(float Damage)
{
	CharacterHealth -= Damage;

	if (CharacterHealth <= 0.0f)
	{
		IsDead = true;
	}
}

void AGitCharacter::SetInteractionItem(AActor* Item)
{
	IsInteracting = true;
	TargetItem = Item;
}