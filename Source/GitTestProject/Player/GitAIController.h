// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GitAIController.generated.h"

/**
 * 
 */
UCLASS()
class GITTESTPROJECT_API AGitAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;	
};
