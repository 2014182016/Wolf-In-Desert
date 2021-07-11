// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DesertFoxPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DESERTFOX_API ADesertFoxPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// {{ APlayerController interface
	virtual void SetupInputComponent() override;
	// }} APlayerController interface

	// {{ Bind Axis Function
	void MoveForward(const float Value);
	void MoveRight(const float Value);
	void Turn(const float Value);
	void LookUp(const float Value);
	// }} Bind Axis Function

	// {{ Bind Action Function
	void StartJump();
	void StopJump();
	void StartRun();
	void StopRun();
	void StartSlowWalk();
	void StopSlowWalk();
	// }} Bind Action Function

protected:
	/** The speed at which the screen is rotated depends on the this variable */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float MouseSensitivity = 45.0f;

	/* If true, Pawn looks in the direction the controller sees. If not, Pawn rotates according to the inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		bool bUsePawnControlRotation = true;
};
