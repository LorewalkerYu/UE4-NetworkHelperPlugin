// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/PlayerController.h"
#include "RCInputObj.generated.h"

/**
 * 
 */

UCLASS()
class REMOTECONTROL_API URCInputObj : public UObject
{
	GENERATED_BODY()

public:

	/** Positions the mouse cursor in screen space, in pixels. */
	UFUNCTION(BlueprintCallable, Category = "NetworkHelperPlugin|RemoteControl|Input")
		bool SetMousePosition(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable, Category = "NetworkHelperPlugin|RemoteControl|Input")
		bool InputKey(const FName KeyName, const EInputEvent InputEvent);

	UFUNCTION(BlueprintCallable, Category = "NetworkHelperPlugin|RemoteControl|Input")
		bool InputKeyTest(const FName KeyName, const EInputEvent InputEvent);

protected:

	bool  InputChar(const FKey& Key, const EInputEvent& InputEvent);
	bool  InputKeyGamePlay(const FKey& Key, const EInputEvent& InputEvent);
	bool  InputKeyUI(const FKey& Key, const EInputEvent& InputEvent);
	bool  InputMouseButtonUI(const FKey& Key, const EInputEvent& InputEvent);
	int   GetInputKeyCode(const FKey& Key);
	TCHAR GetInputCharacter(const FKey& Key);

public:

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		APlayerController* PlayerController = nullptr;

};
