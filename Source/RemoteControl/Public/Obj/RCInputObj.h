// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/PlayerController.h"

#include "RCInputObj.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FMouseMove, int32, X, int32, Y);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FKeyInput, const FName, KeyName, const EInputEvent, InputEvent);
/**
 * 
 */
UENUM(BlueprintType)
enum class ERCType : uint8
{
	ERC_None     UMETA(DisplayName = "None"),
	ERC_Sender   UMETA(DisplayName = "Sender"),
	ERC_Receiver UMETA(DisplayName = "Receiver"),
};


UCLASS(NotBlueprintType)
class REMOTECONTROL_API URCInputObj : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "NetworkHelperPlugin|RemoteControl|Input")
		void InitilizeRCForWindows(ERCType RemoteControlType, UPARAM(DisplayName = "MouseMoveEvent") FMouseMove MouseMove, UPARAM(DisplayName = "KeyInputEvent") FKeyInput KeyInput);

	/** Positions the mouse cursor in screen space, in pixels. */
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected = "true"), Category = "NetworkHelperPlugin|RemoteControl|Input")
		bool SetMousePosition(int32 X, int32 Y, int32 OtherViewportSizeX, int32 OtherViewportSizeY);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected = "true"), Category = "NetworkHelperPlugin|RemoteControl|Input")
		bool InputMouseWheel(bool bForward);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected = "true"), Category = "NetworkHelperPlugin|RemoteControl|Input")
		bool InputKey(const FName KeyName, const EInputEvent InputEvent);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NetworkHelperPlugin|RemoteControl|Input")
		void GetScreenSize(int32& X, int32& Y);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NetworkHelperPlugin|RemoteControl|Input")
		void GetMousePositionInScreen(int32& X, int32& Y);
protected:

	int   GetInputKeyCode     (const FKey& Key);
	TCHAR GetInputCharacter   (const FKey& Key);


	bool  InputChar           (const FKey& Key, const EInputEvent& InputEvent);
	bool  InputKeyGamePlay    (const FKey& Key, const EInputEvent& InputEvent);
	bool  InputKeyUI          (const FKey& Key, const EInputEvent& InputEvent);
	bool  InputMouseButtonUI  (const FKey& Key, const EInputEvent& InputEvent);

	bool  SendInputMouseMove  (int32 X, int32 Y, int32 OtherViewportSizeX, int32 OtherViewportSizeY);
	bool  SendInputKey        (const FKey& Key, const EInputEvent& InputEvent);
	bool  SendInputMouseWheel (bool bForward);

protected:

	int ScreenSizeX;
	int ScreenSizeY;

public:

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		APlayerController* PlayerController = nullptr;

	ERCType RCType;

	FMouseMove MouseMoveDele;
	FKeyInput KeyInputDele;
};
