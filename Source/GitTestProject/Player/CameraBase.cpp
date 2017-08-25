// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraBase.h"
#include "Utilities/GitStatics.h"
#include "Utilities/GitCore.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ACameraBase::ACameraBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

// Called when the game starts or when spawned
void ACameraBase::BeginPlay()
{
	Super::BeginPlay();	

	FRotator ActorRotation = GetActorRotation();
	CorrectActorRotation();
	SetCameraPitch(ActorRotation.Pitch);

	InitCameraMovementKeys();
}

void ACameraBase::AttachCameraToObject(AActor* targetActor)
{
	SetActorLocation(targetActor->GetActorLocation());
}

void ACameraBase::ResetCameraRotationToDefault()
{
	
}

void ACameraBase::InitCameraMovementKeys()
{
	APlayerController* TmpPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (TmpPlayerController)
	{
		EnableInput(TmpPlayerController);

		if (InputComponent)
		{
			InputComponent->BindAxis("CameraMovementForward", this, &ACameraBase::MoveCameraX);
			InputComponent->BindAxis("CameraMovementSide", this, &ACameraBase::MoveCameraY);
			InputComponent->BindAxis("CameraZoom", this, &ACameraBase::ZoomCamera);
		}
	}
}

// Called every frame
void ACameraBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoCameraMovement(DeltaTime);
}

void ACameraBase::DoCameraMovement(const float& DeltaTime)
{
	//Move the camera
	if (CameraMovementVector.SizeSquared() > 0.0f)
	{
		AddActorLocalOffset(CameraMovementVector * DeltaTime * CameraKeyboardMovementSpeed);
		CameraMovementVector = FVector(0.0f);
	}

	//Zoom the camera
	if (CameraZoom != 0.0f)
	{
		if (Camera)
		{
			//Via the FOV
			/*if ((Camera->FieldOfView + CameraZoom * DeltaTime >= CameraLowestZoomValue)&&(Camera->FieldOfView + CameraZoom * DeltaTime <= CameraHighestZoomValue))
				Camera->FieldOfView += CameraZoom * DeltaTime;*/

			//Via the ArmLength
			if (((SpringArm->TargetArmLength + (CameraZoom * DeltaTime) > CameraLowestZoomValue)) && ((SpringArm->TargetArmLength + (CameraZoom * DeltaTime) < CameraHighestZoomValue)))
			{
				SpringArm->TargetArmLength += CameraZoom * DeltaTime;

				CameraZoom = 0.0f;
			}
		}
	}
}

void ACameraBase::MoveCameraX(float Value)
{
	CameraMovementVector.X =  Value;
}

void ACameraBase::MoveCameraY(float Value)
{
	CameraMovementVector.Y =  Value;
}

void ACameraBase::ZoomCamera(float Value)
{
	CameraZoom = CameraZoomSpeed * Value;
}

void ACameraBase::BecomeViewTarget(APlayerController * PC)
{
	PlayerController = PC;
}

void ACameraBase::EndViewTarget(APlayerController * PC)
{
	PlayerController = nullptr;
}

void ACameraBase::UpdateCamera()
{
	if(!PlayerController)
	{
		return;
	}

	if(Player)
	{
		if (bLockCameraToPlayer || bLockCameraToPlayerPermanent)
		{
			SetActorLocation(Player->GetActorLocation());
		}		
	}
	 
	CorrectActorRotation();

	float MouseX = -1.0f;
	float MouseY = -1.0f;
	PlayerController->GetMousePosition(MouseX, MouseY);
	FVector2D ViewportSize = UGitStatics::GetViewportSize();
	
	float UpperBorder = 0.0f + BorderMargin;
	float LowerBorder = ViewportSize.Y - BorderMargin;
	float LeftBorder = 0.0f + BorderMargin;
	float RightBorder = ViewportSize.X - BorderMargin;	

	if(MouseX <= LeftBorder)
	{
		if(MouseY <= UpperBorder)
		{
			// move camera up and left		
			FVector Direction(1.0f, -1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed;
			AddActorLocalOffset(Direction);
		}
		else if (MouseY >= LowerBorder)
		{
			// move camera down and left			
			FVector Direction(-1.0f, -1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed;
			AddActorLocalOffset(Direction);
		}
		else
		{
			// move camera left		
			FVector Direction(0.0f, -1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed;
			AddActorLocalOffset(Direction);
		}
	}
	else if(MouseX >= RightBorder)
	{
		if (MouseY <= UpperBorder)
		{
			// move camera up and right		
			FVector Direction(1.0f, 1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed;
			AddActorLocalOffset(Direction);
		}
		else if (MouseY >= LowerBorder)
		{
			// move camera down and right						
			FVector Direction(-1.0f, 1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed;
			AddActorLocalOffset(Direction);
		}
		else
		{
			// move camera right					
			FVector Direction(0.0f, 1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed;
			AddActorLocalOffset(Direction);
		}
	}
	else if (MouseY <= UpperBorder)
	{
		// move camera up			
		FVector Direction(1.0f, 0.0f, 0.0f);
		Direction = Direction.GetSafeNormal();
		Direction *= CameraMoveSpeed;
		AddActorLocalOffset(Direction);
	}
	else if (MouseY >= LowerBorder)
	{
		// move camera down		
		FVector Direction(-1.0f, 0.0f, 0.0f);
		Direction = Direction.GetSafeNormal();
		Direction *= CameraMoveSpeed;
		AddActorLocalOffset(Direction);
	}	
}

void ACameraBase::AddCameraYaw(float Value)
{	
	AddActorWorldRotation(FRotator(0.0f, Value, 0.0f));		
}

void ACameraBase::AddCameraPitch(float Value)
{
	float Pitch = SpringArm->RelativeRotation.Pitch;	
	Pitch += Value;
	Pitch = FMath::Clamp(Pitch, -89.0f, -1.0f);	
	SpringArm->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));		
}

void ACameraBase::SetCameraPitch(float Value)
{
	float Pitch = Value;	
	Pitch = FMath::Clamp(Pitch, -89.0f, -1.0f);
	SpringArm->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
}

void ACameraBase::LockCameraToPlayer()
{
	bLockCameraToPlayer = true;
}

void ACameraBase::UnlockCameraFromPlayer()
{
	bLockCameraToPlayer = false;
}

void ACameraBase::CorrectActorRotation()
{
	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Pitch = 0.0f;
	ActorRotation.Roll = 0.0f;
	SetActorRotation(ActorRotation);
}
