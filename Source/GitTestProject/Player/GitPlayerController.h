// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GitPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GITTESTPROJECT_API AGitPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	virtual void On_LMB_Pressed();

	virtual void On_RMB_Pressed();

	//AActor* GetClickedOnActor();
	
public:
	UPROPERTY(BlueprintReadWrite)
	class ACameraBase* ActiveCamera;


	
	
};
