// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h" 
#include "GitEnemyAIController.generated.h"

USTRUCT()
struct FPointOfInterest
{
	GENERATED_BODY()

	FPointOfInterest()
	{
		Location = FVector::ZeroVector;
		Duration = 0.0f;
	}

	FPointOfInterest(FVector NewLocation, float NewDuration)
	{		
		Location = NewLocation;
		Duration = NewDuration;
	}

	FVector Location;
	float Duration;
};

/**
 * 
 */
UCLASS()
class GITTESTPROJECT_API AGitEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	bool HasLOS(AActor* ToActor);

	// @param Duration - How long will Pawn remember this position, in seconds.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add POI To Memory"))
	void AddPOIToMemory(FVector Location, float Duration = 60.0f);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove POI From Memory"))
	void RemovePOIFromMemory(FVector LocationToRemove, float Tolerance = 100.0f);


private:
	UPROPERTY()
	TArray<FPointOfInterest> POIsMemory;
	
};
