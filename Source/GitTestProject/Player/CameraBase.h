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

	UFUNCTION()
	void InitCameraMovementKeys();

	UFUNCTION()
	void DoCameraMovement(const float& DeltaTime);

	UFUNCTION()
	void MoveCameraX(float Value);

	UFUNCTION()
	void MoveCameraY(float Value);

	UFUNCTION()
	void ZoomCamera(float Value);

	UFUNCTION()
	void AttachCameraToObject(AActor* targetActor);

	UFUNCTION()
	void ResetCameraRotationToDefault();

private:
	void CorrectActorRotation();

public:
	UPROPERTY(VisibleDefaultsOnly)
	class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	class UCameraComponent* Camera = nullptr;
	
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	// how fast will camera move when cursor is on the edge of the screen
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CameraMoveSpeed = 500.0f;

	// offset from edge of the screen in pixels
	UPROPERTY(EditDefaultsOnly)
	float BorderMargin = 20.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bLockCameraToPlayerPermanent = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CameraKeyboardMovementSpeed = 2000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CameraMovementVector = FVector(0.0f);

	UPROPERTY(BlueprintReadWrite)
	float CameraZoom = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CameraZoomSpeed = 350.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CameraLowestZoomValue = 650.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CameraHighestZoomValue = 1100.0f;

	UPROPERTY(BLueprintReadWrite, EditAnywhere)
	bool LockInput = false;

private:
	bool bLockCameraToPlayer = false;	
};
