// Fill out your copyright notice in the Description page of Project Settings.

#include "GitGroupAgent.h"
#include "Gameplay/GitFormation.h"
#include "Utilities/GitCore.h"
#include "Gameplay/Waypoint.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AGitGroupAgent::AGitGroupAgent()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGitGroupAgent::BeginPlay()
{	
	for (auto& Member : BP_Members)
	{
		AddMember(Member);
	}
	
	for (auto& Waypoint : BP_Waypoints)
	{
		Waypoints.AddTail(Waypoint);
	}

	Super::BeginPlay();			
}

// Called every frame
void AGitGroupAgent::Tick(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();	
	if (NewLocation.Equals(PreviousLocation, 1.0f) == false)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw = UKismetMathLibrary::MakeRotFromX(NewLocation - PreviousLocation).Yaw;		
		FRotator FinalRot = UKismetMathLibrary::RLerp(GetActorRotation(), NewRotation, TurnSpeed, true);
		SetActorRotation(FinalRot);
		PreviousLocation = NewLocation;
	}	

	UpdateFormation();

	Super::Tick(DeltaTime);		
}

// Called to bind functionality to input
void AGitGroupAgent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGitGroupAgent::UpdateFormation()
{
	const int32 UnitCount = Members.Num();
	const FVector UnitLocation = GetActorLocation();
	const FVector ForwardDirection = GetActorForwardVector();

	switch (FormationType)
	{
	case EFormationTypes::FT_LineFormation:
	{
		Positions = UGitFormation::GetLineFormation(UnitCount, UnitSizeX, UnitSizeY, UnitLocation, ForwardDirection);
	}
	}
}

void AGitGroupAgent::AddMember(APawn* Pawn)
{
	Members.AddUnique(Pawn);
}

void AGitGroupAgent::RemoveMember(APawn * Pawn)
{
	Members.Remove(Pawn);	
}

void AGitGroupAgent::RemoveAllMembers()
{
	Members.Empty();
}

bool AGitGroupAgent::GetMemberPosition(APawn* Member, FVector& OutPosition)
{
	int32 Index = Members.Find(Member);	

	if(Index == INDEX_NONE)
	{
		return false;
	}

	bool bIsValidIndex = Positions.IsValidIndex(Index);
	if (bIsValidIndex)
	{
		OutPosition = Positions[Index];		
	}
	else
	{
		PRINTC("Index is not valid. (GitGroupAgent, GetMemberPositon)", FColor::Yellow);
	}

	return true;
}

void AGitGroupAgent::AddWaypointAsHead(AWaypoint * NewWaypoint)
{
	Waypoints.AddHead(NewWaypoint);
}

void AGitGroupAgent::AddWaypointAsTail(AWaypoint * NewWaypoint)
{
	Waypoints.AddTail(NewWaypoint);	
}

void AGitGroupAgent::RemoveWaypoint(AWaypoint * WaypointToRemove)
{
	Waypoints.RemoveNode(WaypointToRemove);
}

void AGitGroupAgent::RemoveAllWaypoints()
{
	Waypoints.Empty();
}

TArray<FVector> AGitGroupAgent::GetAllMembersPositions()
{
	return Positions;
}

