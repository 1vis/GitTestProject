// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GitCharacter.generated.h"

UCLASS(meta = (ShortTooltip = "A Character that can be 'controlled' when selected with left mouse button."))
class GITTESTPROJECT_API AGitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGitCharacter();

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

	UFUNCTION(BlueprintCallable)
	bool HasAssignedPanel();

	UFUNCTION(BlueprintCallable)
	void AssignPanel();

	UFUNCTION(BlueprintPure)
	FVector GetDestination() const;

	void MoveGitCharacterToDestination();	

private:	
	FVector Destination;	

	UPROPERTY()
		bool AssignedPanel = false;
};
