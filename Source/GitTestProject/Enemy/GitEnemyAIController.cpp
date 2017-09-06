// Fill out your copyright notice in the Description page of Project Settings.

#include "GitEnemyAIController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Player/GitCharacter.h"
#include "Utilities/GitCore.h"
#include "Kismet/KismetMathLibrary.h"

void AGitEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AGitEnemyAIController::Tick(float DeltaTime)
{
	for (auto& POI : POIsMemory)
	{
		POI.Duration -= DeltaTime;
	}
	POIsMemory.RemoveAll([](const FPointOfInterest& CurrentPOI) { return CurrentPOI.Duration <= 0.0f; });

	Super::Tick(DeltaTime);
}

bool AGitEnemyAIController::HasLOS(const AActor* ToActor)
{
	if (ToActor == nullptr)
	{
		return false;
	}

	FVector Start = GetPawn()->GetActorLocation();
	FVector End = ToActor->GetActorLocation();

	FHitResult HitResult;	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetPawn());

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams);
	bool bToActorWasHit = HitResult.GetActor()->IsA(ToActor->GetClass());
	if (bToActorWasHit)
	{
		return HitResult.bBlockingHit;
	}

	return  HitResult.bBlockingHit;
}

void AGitEnemyAIController::AddPOIToMemory(FVector Location, float Duration/*= 60.0f*/)
{
	POIsMemory.Push(FPointOfInterest(Location, Duration));
}

void AGitEnemyAIController::RemovePOIFromMemory(FVector LocationToRemove, float Tolerance/*= 100.0f*/)
{
	POIsMemory.RemoveAll([&](const FPointOfInterest& CurrentPOI){ return CurrentPOI.Location.Equals(LocationToRemove, Tolerance); });
}

TArray<FPointOfInterest> AGitEnemyAIController::GetPOIsMemory() const
{
	return POIsMemory;
}

FVector AGitEnemyAIController::GetLastPOIInMemory(bool& bArrayIsEmpty) const
{
	if (POIsMemory.Num() >= 1)
	{
		bArrayIsEmpty = false;
		return POIsMemory.Last().Location;
	}

	bArrayIsEmpty = true;
	return FVector::ZeroVector;	
}

AActor* AGitEnemyAIController::SelectBestTarget(const TArray<AActor*>& PerceivedActors)
{	
	AActor* ClosestActorWithLOS = nullptr;
	float ClosestDistance = BIG_NUMBER;

	for (const auto& Actor : PerceivedActors)
	{
		if (HasLOS(Actor))
		{
			float Distance = UKismetMathLibrary::VSize(Actor->GetActorLocation() - GetPawn()->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActorWithLOS = Actor;
			}
		}
	}	

	return ClosestActorWithLOS;
}
