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

UENUM(BlueprintType)
enum class EPatrolType : uint8
{
	PT_PingPong 	UMETA(DisplayName = "PingPong"),
	PT_Circular UMETA(DisplayName = "Circular")
};

/**
 * 
 */
UCLASS()
class GITTESTPROJECT_API AGitGroupAgentAIController : public AAIController
{
	GENERATED_BODY()

public:

	// Patrols the waypoints.
	UFUNCTION(BlueprintCallable)
	void Patrol(EPatrolType PatrolType = EPatrolType::PT_PingPong, float AcceptanceRadius = 100.0f);

private:
	UPROPERTY()
	class AWaypoint* CurrentWaypoint = nullptr;

	ETraverseDirection CurrentTraverseDirection = ETraverseDirection::TD_Forward;
};


