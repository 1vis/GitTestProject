// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GitGroupAgent.generated.h"

UCLASS()
class GITTESTPROJECT_API AGitGroupAgent : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGitGroupAgent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void AddMember(APawn* NewMember);

	UFUNCTION(BlueprintCallable)
	void RemoveMember(APawn* Member);
	
	// Updates positon of each member in formation
	void UpdateFormation();

	// Return false if given Pawn is not a member
	UFUNCTION(BlueprintPure)
	bool GetMemberPosition(APawn* Member, UPARAM(DisplayName = "Position") FVector& OutPosition);

	UFUNCTION(BlueprintPure)
	TArray<FVector> GetAllMembersPositions();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float UnitSizeX = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float UnitSizeY = 100.0f;

private:
	TArray<APawn*> Members;	

	TArray<FVector> Positions;

};


