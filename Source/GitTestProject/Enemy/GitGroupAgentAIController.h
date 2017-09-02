// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GitGroupAgentAIController.generated.h"


UENUM(BlueprintType)		
enum class ETraverseDirection : uint8
{
	TD_Forward 	UMETA(DisplayName = "Forward"),
	TD_Backward UMETA(DisplayName = "Backward")	
};

/**
 * 
 */
UCLASS()
class GITTESTPROJECT_API AGitGroupAgentAIController : public AAIController
{
	GENERATED_BODY()

public:

	// Patrols the waypoints in Ping Pong manner.
	UFUNCTION(BlueprintCallable)
	void Patrol(float AcceptanceRadius = 100.0f);

private:
	UPROPERTY()
	class AWaypoint* CurrentWaypoint = nullptr;

	ETraverseDirection CurrentTraverseDirection = ETraverseDirection::TD_Forward;
};


