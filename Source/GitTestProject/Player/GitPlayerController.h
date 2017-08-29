// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GitPlayerController.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateThePanelsDelegate, const TArray<APawn*>&, SelectedChar);
/**
 * 
 */
UCLASS()
class GITTESTPROJECT_API AGitPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGitPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	void MoveSelectedPawnToDestination();

	UFUNCTION(BlueprintCallable)
	void HighlightCharacterPanels(TArray<APawn*> SelectedActors);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ACameraBase* ActiveCamera = nullptr;	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<APawn*> SelectedPawns;

	UPROPERTY(BlueprintAssignable)
	FUpdateThePanelsDelegate UpdateCharacterPanelsFunction;
};
