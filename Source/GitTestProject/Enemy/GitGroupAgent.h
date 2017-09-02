// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GitGroupAgent.generated.h"

UENUM(BlueprintType)		
enum class EFormationTypes : uint8
{
	FT_LineFormation 	UMETA(DisplayName = "Line Formation")	
};


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

public:
	// Updates positon of each member in formation
	void UpdateFormation();

	// Adds a unique pawn to the array.
	// See also: RemoveMember
	UFUNCTION(BlueprintCallable)
	void AddMember(APawn* NewMember);

	// See also: AddMember
	UFUNCTION(BlueprintCallable)
	void RemoveMember(APawn* Member);	

	UFUNCTION(BlueprintCallable)
	void RemoveAllMembers();

	// Return false if given Pawn is not a member
	// See also: GetAllMembersPositions
	UFUNCTION(BlueprintPure)
	bool GetMemberPosition(APawn* Member, UPARAM(DisplayName = "Position") FVector& OutPosition);

	UFUNCTION(BlueprintCallable)
	void AddWaypointAsHead(class AWaypoint* NewWaypoint);

	UFUNCTION(BlueprintCallable)
	void AddWaypointAsTail(AWaypoint* NewWaypoint);
	
	UFUNCTION(BlueprintCallable)
	void RemoveWaypoint(AWaypoint* WaypointToRemove);

	UFUNCTION(BlueprintCallable)
	void RemoveAllWaypoints();

	// See also: GetMemberPosition
	UFUNCTION(BlueprintPure)
	TArray<FVector> GetAllMembersPositions();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class AWaypoint*> BP_Waypoints;

	TDoubleLinkedList<AWaypoint*> Waypoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EFormationTypes FormationType = EFormationTypes::FT_LineFormation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float UnitSizeX = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float UnitSizeY = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PatrolSpeed = 250.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TurnSpeed = 0.025f;



private:
	// This arrays exists only for convenience, represents all pawns from level that will be added on begin play to the actual array.
	// To add members at runtime use AddMember instead.
	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Members"))
	TArray<APawn*> BP_Members;

	UPROPERTY()
	TArray<APawn*> Members;		

	TArray<FVector> Positions;

	FVector PreviousLocation = FVector::ZeroVector;
};


