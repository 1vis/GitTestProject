// Fill out your copyright notice in the Description page of Project Settings.

#include "GitGroupAgent.h"
#include "Gameplay/GitFormation.h"
#include "Utilities/GitCore.h"


// Sets default values
AGitGroupAgent::AGitGroupAgent()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGitGroupAgent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGitGroupAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFormation();
}

// Called to bind functionality to input
void AGitGroupAgent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGitGroupAgent::AddMember(APawn* Pawn)
{
	Members.AddUnique(Pawn);
}

void AGitGroupAgent::RemoveMember(APawn * Pawn)
{
	Members.Remove(Pawn);
	int ind;
	Members.Find(Pawn, ind);
}

void AGitGroupAgent::UpdateFormation()
{
	const int32 UnitCount = Members.Num();
	const FVector UnitLocation = GetActorLocation();
	const FVector ForwardDirection = GetActorForwardVector();

	Positions = UGitFormation::GetLineFormation(UnitCount, UnitSizeX, UnitSizeY, UnitLocation, ForwardDirection);
}

bool AGitGroupAgent::GetMemberPosition(APawn* Member, UPARAM(DisplayName = "Position") FVector& OutPosition)
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
		PRINTC("Index is not valid. (GitGroupAgent, GetMemberPositon)", FColor::Red);
	}

	return true;
}

TArray<FVector> AGitGroupAgent::GetAllMembersPositions()
{
	return Positions;
}

