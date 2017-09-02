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

	UFUNCTION(BlueprintCallable)
	void StartSelectionMode();

	UFUNCTION(BlueprintCallable)
	void StopSelectionMode();

	UFUNCTION(BlueprintCallable)
	void UpdateSelectionBoxData(const float& DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SetSelectedActors(TArray<APawn*> SelectedActors);

	UFUNCTION(BlueprintCallable)
	bool IsInSelectionBox(APawn* SelectedActor);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ACameraBase* ActiveCamera = nullptr;	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<APawn*> SelectedPawns;

	UPROPERTY(BlueprintAssignable)
	FUpdateThePanelsDelegate UpdateCharacterPanelsFunction;

	UPROPERTY(BlueprintReadWrite)
	bool SelectBoxMode = false;

	UPROPERTY(BlueprintReadWrite)
	float SelectionBoxX = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float SelectionBoxY = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float SelectionBoxW = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float SelectionBoxH = 0.0f;
};
