// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WIDMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

void FMovemenetStateInfo::Set(UWIDMovementComponent* OtherMovement) const
{
	if (OtherMovement)
	{
		if (bOrientRotationToMovement.IsSet())
			OtherMovement->bOrientRotationToMovement = bOrientRotationToMovement.GetValue();
		if (RotationRate.IsSet())
			OtherMovement->RotationRate = RotationRate.GetValue();
		if (JumpZVelocity.IsSet())
			OtherMovement->JumpZVelocity = JumpZVelocity.GetValue();
		if (Speed.IsSet())
			OtherMovement->MaxWalkSpeed = Speed.GetValue();
	}
}

void UWIDMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	SetMovementState(EWIDMovementState::Idle);
}

void UWIDMovementComponent::SetMovementState(const EWIDMovementState NewMovmenetState, bool bSaveMovement)
{
	if (CurrentMovementState == NewMovmenetState)
		return;
	
	const FMovemenetStateInfo* const NewMovementStateInfo = MovementStateInfoMap.Find(NewMovmenetState);
	if (NewMovementStateInfo == nullptr)
		return;

	const FMovemenetStateInfo* const CurrentMovementStateInfo = MovementStateInfoMap.Find(CurrentMovementState);
	if (CurrentMovementStateInfo != nullptr)
	{
		if (CurrentMovementStateInfo->AllowTargetState != EWIDMovementState::None && CurrentMovementStateInfo->AllowTargetState != NewMovmenetState)
			return;
	}

	NewMovementStateInfo->Set(this);

	if (bSaveMovement)
	{
		SaveMovementState(CurrentMovementState);
	}

	CurrentMovementState = NewMovmenetState;
}

void UWIDMovementComponent::SaveMovementState(const EWIDMovementState OldMovmenetState)
{
	if (!CanSaveMovemenetState(OldMovmenetState))
		return;

	LastMovementState = OldMovmenetState;
}

void UWIDMovementComponent::RestoreMovmenetState()
{
	if (CurrentMovementState == LastMovementState)
		return;

	if (!CanRestoreMovementState(CurrentMovementState, LastMovementState))
		return;

	if (LastMovementState == EWIDMovementState::None)
	{
		LastMovementState = EWIDMovementState::Idle;
	}

	if (IsValid(CharacterOwner))
	{
		APlayerController* PlayerController = CharacterOwner->GetController<APlayerController>();
		if (IsValid(PlayerController))
		{
			// When returning from a special state, return to the idle state unless the corresponding key is pressed
			if ((LastMovementState == EWIDMovementState::Running) && !PlayerController->IsInputKeyDown(EKeys::LeftShift))
				LastMovementState = EWIDMovementState::Idle;
			if ((LastMovementState == EWIDMovementState::SlowWalking) && !PlayerController->IsInputKeyDown(EKeys::LeftControl))
				LastMovementState = EWIDMovementState::Idle;
		}
	}

	SetMovementState(LastMovementState);
}

void UWIDMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	if (IsValid(CharacterOwner))
	{
		FHitResult HitResult(ForceInit);

		FVector CapsuleHalfHeight = FVector(0.0f, 0.0f, CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector StartLoc = CharacterOwner->GetActorLocation() - CapsuleHalfHeight;
		FVector EndLoc = StartLoc + (FVector::DownVector * WID::CheckFallingDistance);

		if (CharacterOwner->GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility))
		{
			DistanceToFloorWhileFalling = HitResult.Distance;
		}
	}

	Super::PhysFalling(deltaTime, Iterations);
}

void UWIDMovementComponent::SetPostLandedPhysics(const FHitResult& Hit)
{
	Super::SetPostLandedPhysics(Hit);

	SetMovementState(EWIDMovementState::Idle, false);
	RestoreMovmenetState();
}

bool UWIDMovementComponent::CanRun() const
{
	return IsWalking() && CurrentMovementState != EWIDMovementState::Jumping;
}

bool UWIDMovementComponent::CanWalk() const
{
	return IsWalking() && CurrentMovementState != EWIDMovementState::Jumping;
}

bool UWIDMovementComponent::CanSlowWalk() const
{
	return IsWalking() && CurrentMovementState != EWIDMovementState::Jumping;
}

bool UWIDMovementComponent::CanJump() const
{
	return IsWalking() && CurrentMovementState != EWIDMovementState::Jumping && CurrentMovementState != EWIDMovementState::Sleeping;
}

bool UWIDMovementComponent::CanSaveMovemenetState(const EWIDMovementState MovmenetState) const
{
	return MovmenetState != EWIDMovementState::None && MovmenetState != EWIDMovementState::Jumping && MovmenetState != EWIDMovementState::Sleeping;
}

bool UWIDMovementComponent::CanRestoreMovementState(const EWIDMovementState CurrentMovmenetState, const EWIDMovementState LastMovmenetState) const
{
	return !IsJumping() && CurrentMovmenetState != EWIDMovementState::Sleeping;
}

void UWIDMovementComponent::PendingMovementState(const EWIDMovementState NewMovmenetState, const float PendingTime)
{
	SaveMovementState(NewMovmenetState);

	if (PendingTime > 0.0f)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UWIDMovementComponent::RestoreMovmenetState, PendingTime, false);
	}
}

float UWIDMovementComponent::GetMaxSpeed() const
{
#if	!UE_BUILD_SHIPPING
	if (CheatSpeed > 0.0f)
		return CheatSpeed;
#endif // !UE_BUILD_SHIPPING

	return Super::GetMaxSpeed();
}