// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GitPawn.generated.h"

UCLASS(meta = (ShortTooltip = "A Pawn that can be 'controlled' when selected with left mouse button."))
class GITTESTPROJECT_API AGitPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGitPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Function that is called when you left-click on this actor
	virtual void NotifyActorOnReleased(FKey ButtonReleased) override;

	UFUNCTION(BlueprintCallable)
	void SetDestinationToMouseCursor();

	UFUNCTION(BlueprintPure)
	FVector GetDestination() const;

	void MoveGitPawnToDestination();

private:
	FVector Destination;	
};



