// Fill out your copyright notice in the Description page of Project Settings.


#include "RCInputObj.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/HideWindowsPlatformTypes.h"

void URCInputObj::SetMousePosition(APlayerController* PlayerController, int32 X, int32 Y)
{
	if (!PlayerController)
	{
		return;
	}
	PlayerController->SetMouseLocation(X, Y);
}

void URCInputObj::ProcessKeyInput(APlayerController* PlayerController, const FName KeyName, const EInputEvent InputEvent)
{
	if (!PlayerController)
	{
		return;
	}
	FKey Key(KeyName);
	FInputKeyEventArgs InputArgs
	{
		GEngine->GameViewport->Viewport,
		0,
		Key,
		InputEvent
	};
	GEngine->GameViewport->InputKey(InputArgs);
	//Get our slate application
	FSlateApplication& SlateApp = FSlateApplication::Get();

	// create a pointer event
	FPointerEvent MouseDownEvent(
		0,
		SlateApp.CursorPointerIndex,
		SlateApp.GetCursorPos(),
		SlateApp.GetLastCursorPos(),
		SlateApp.GetPressedMouseButtons(),
		Key,
		0,
		SlateApp.GetPlatformApplication()->GetModifierKeys()
	);

	//send the mouse event to the slate handler
	TSharedPtr<FGenericWindow> GenWindow;
	SlateApp.ProcessMouseButtonDownEvent(GenWindow, MouseDownEvent);

	SlateApp.ProcessMouseButtonUpEvent(MouseDownEvent);
}
