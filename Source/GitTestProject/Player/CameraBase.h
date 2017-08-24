// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraBase.generated.h"

UCLASS()
class GITTESTPROJECT_API ACameraBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BecomeViewTarget(APlayerController* PC) override;

	virtual void EndViewTarget(APlayerController* PC) override;

	UFUNCTION(BlueprintCallable)
	void UpdateCamera();

	UFUNCTION(BlueprintCallable)
	void AddCameraYaw(float Value);

	UFUNCTION(BlueprintCallable)
	void AddCameraPitch(float Value);

	UFUNCTION(BlueprintCallable)
	void SetCameraPitch(float Value);

	UFUNCTION(BlueprintCallable)
	void LockCameraToPlayer();

	UFUNCTION(BlueprintCallable)
	void UnlockCameraFromPlayer();

private:
	void CorrectActorRotation();

public:
	UPROPERTY(VisibleDefaultsOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
	class UCameraComponent* Camera;	
	
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* Player;

	// how fast will camera move when cursor is on the edge of the screen
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CameraMoveSpeed = 5.0f;

	// offset from edge of the screen in pixels
	UPROPERTY(EditDefaultsOnly)
	float BorderMargin = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bLockCameraToPlayerPermanent = false;

private:
	bool bLockCameraToPlayer = false;	
};
