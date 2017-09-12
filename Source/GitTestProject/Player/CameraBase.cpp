// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraBase.h"
#include "Utilities/GitStatics.h"
#include "Utilities/GitCore.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Player/GitPlayerController.h"

// Sets default values
ACameraBase::ACameraBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));	
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

	FVector CurrentLocation = GetActorLocation();
	float DesiredX = FMath::Clamp(CurrentLocation.X, -25835.226563f, 26006.773438f);
	float DesiredY = FMath::Clamp(CurrentLocation.Y, -25856.558594f, 25643.109375f);
	SetActorLocation(FVector(DesiredX, DesiredY, CurrentLocation.Z));
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
	if (LockInput)
	{
		return;
	}

	CameraMovementVector.X =  Value;
}

void ACameraBase::MoveCameraY(float Value)
{
	if (LockInput)
	{
		return;
	}

	CameraMovementVector.Y =  Value;
}

void ACameraBase::ZoomCamera(float Value)
{
	if (LockInput)
	{
		return;
	}

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
	AGitPlayerController* GitPC = Cast<AGitPlayerController>(UGitStatics::GetGitPlayerController(this));

	if(!PlayerController)
	{
		return;
	}

	if (LockInput)
	{
		return;
	}
	
	if(GitPC->SelectedPawns.Num() > 0)
	{

		TArray<AActor*> DowncastedArray;
		for (const auto& Pawn : GitPC->SelectedPawns)
		{
			DowncastedArray.Add(Pawn);
		}		
		const FVector AverageLocation = UGameplayStatics::GetActorArrayAverageLocation(DowncastedArray);
		if (bLockCameraToPlayer || bLockCameraToPlayerPermanent)
		{
			SetActorLocation(AverageLocation);
		}		
	}
	 
	CorrectActorRotation();

	float MouseX = -1.0f;
	float MouseY = -1.0f;
	PlayerController->GetMousePosition(MouseX, MouseY);
	const FVector2D ViewportSize = UGitStatics::GetViewportSize();
	
	const float UpperBorder = 0.0f + BorderMargin;
	const float LowerBorder = ViewportSize.Y - BorderMargin;
	const float LeftBorder = 0.0f + BorderMargin;
	const float RightBorder = ViewportSize.X - BorderMargin;	

	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	if(MouseX <= LeftBorder)
	{
		if(MouseY <= UpperBorder)
		{
			// move camera up and left		
			FVector Direction(1.0f, -1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed * DeltaTime;
			AddActorLocalOffset(Direction);
		}
		else if (MouseY >= LowerBorder)
		{
			// move camera down and left			
			FVector Direction(-1.0f, -1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed * DeltaTime;
			AddActorLocalOffset(Direction);
		}
		else
		{
			// move camera left		
			FVector Direction(0.0f, -1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed * DeltaTime;
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
			Direction *= CameraMoveSpeed * DeltaTime;;
			AddActorLocalOffset(Direction);
		}
		else if (MouseY >= LowerBorder)
		{
			// move camera down and right						
			FVector Direction(-1.0f, 1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed * DeltaTime;
			AddActorLocalOffset(Direction);
		}
		else
		{
			// move camera right					
			FVector Direction(0.0f, 1.0f, 0.0f);
			Direction = Direction.GetSafeNormal();
			Direction *= CameraMoveSpeed * DeltaTime;
			AddActorLocalOffset(Direction);
		}
	}
	else if (MouseY <= UpperBorder)
	{
		// move camera up			
		FVector Direction(1.0f, 0.0f, 0.0f);
		Direction = Direction.GetSafeNormal();
		Direction *= CameraMoveSpeed * DeltaTime;
		AddActorLocalOffset(Direction);
	}
	else if (MouseY >= LowerBorder)
	{
		// move camera down		
		FVector Direction(-1.0f, 0.0f, 0.0f);
		Direction = Direction.GetSafeNormal();
		Direction *= CameraMoveSpeed * DeltaTime;
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

