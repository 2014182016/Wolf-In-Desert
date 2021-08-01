// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/OptionDetailWidget.h"
#include "GameplayOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class WID_API UGameplayOptionWidget : public UOptionDetailWidget
{
	GENERATED_BODY()
	
protected:
	// {{ UUserWidget Interface
	virtual void NativeConstruct() override;
	// }} UUserWidget Interface

protected:
	// {{ UOptionDetailWidget Interface
	virtual void UpdateGameSettings() override;
	// }} UOptionDetailWidget Interface

};