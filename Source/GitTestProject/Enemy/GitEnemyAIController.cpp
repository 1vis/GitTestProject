// Fill out your copyright notice in the Description page of Project Settings.

#include "GitEnemyAIController.h"
#include "Runtime/Engine/Classes/Engine/World.h"


void AGitEnemyAIController::Tick(float DeltaTime)
{
	for (auto& POI : POIsMemory)
	{
		POI.Duration -= DeltaTime;
	}
	POIsMemory.RemoveAll([](const FPointOfInterest& CurrentPOI) { return CurrentPOI.Duration <= 0.0f; });
}

bool AGitEnemyAIController::HasLOS(AActor* ToActor)
{
	FHitResult HitResult;
	FVector Start = GetPawn()->GetActorLocation();
	FVector End = ToActor->GetActorLocation();

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
	return HitResult.bBlockingHit;		
}

void AGitEnemyAIController::AddPOIToMemory(FVector Location, float Duration/*= 60.0f*/)
{
	POIsMemory.Push(FPointOfInterest(Location, Duration));
}

void AGitEnemyAIController::RemovePOIFromMemory(FVector LocationToRemove, float Tolerance/*= 100.0f*/)
{
	POIsMemory.RemoveAll([&](const FPointOfInterest& CurrentPOI){ return CurrentPOI.Location.Equals(LocationToRemove, Tolerance); });
}
