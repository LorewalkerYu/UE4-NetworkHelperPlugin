// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/PlayerController.h"
#include "RCInputObj.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class REMOTECONTROL_API URCInputObj : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "NetworkHelperPlugin|RemoteControl|Input")
		void SetMousePosition(APlayerController* PlayerController, int32 X, int32 Y);

	UFUNCTION(BlueprintCallable, Category = "NetworkHelperPlugin|RemoteControl|Input")
		void ProcessKeyInput(APlayerController* PlayerController, const FName KeyName, const EInputEvent InputEvent);
};
