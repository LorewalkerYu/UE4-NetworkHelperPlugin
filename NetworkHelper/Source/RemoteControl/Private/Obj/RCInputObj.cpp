// Fill out your copyright notice in the Description page of Project Settings.


#include "Obj/RCInputObj.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "Types/SlateStructs.h"
#include "Framework/Application/SlateApplication.h"

bool URCInputObj::SetMousePosition(int32 X, int32 Y)
{
#if PLATFORM_WINDOWS
	// Client Area Rect
	FSlateRect ClientRect = GEngine->GameViewport->GetWindow()->GetClientRectInScreen();

	// x y coordinate convert
	int cx_screen = ::GetSystemMetrics(SM_CXSCREEN);  //屏幕 宽
	int cy_screen = ::GetSystemMetrics(SM_CYSCREEN);  //     高

	int real_x = 65535 * (X + int32(ClientRect.Left)) / cx_screen;  //转换后的 x
	int real_y = 65535 * (Y + int32(ClientRect.Top)) / cy_screen;  //         y

	INPUT InputMouse;
	InputMouse.type = INPUT_MOUSE;
	InputMouse.mi.mouseData = 0;
	InputMouse.mi.dx = real_x;
	InputMouse.mi.dy = real_y;
	InputMouse.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	SendInput(1, &InputMouse, sizeof(INPUT));

	return true;
#endif
	
	// Be aware, this will not effect MouseX MouseY axis value
	bool bRet = true;
	if (!PlayerController)
	{
		return false;
	}

	PlayerController->SetMouseLocation(X, Y);
	return false;


}

bool URCInputObj::InputKey(const FName KeyName, const EInputEvent InputEvent)
{
	FKey Key(KeyName);
	if (!Key.IsValid())
	{
		return false;
	}
#if PLATFORM_WINDOWS
	INPUT Input;
	int KeyCode = GetInputKeyCode(Key);
	if(Key.IsMouseButton())
	{
		Input.type = INPUT_MOUSE;

		// Left Mouse Button
		if (KeyCode == VK_LBUTTON)
		{
			if (InputEvent == EInputEvent::IE_Pressed)
			{
				Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			}
			if (InputEvent == EInputEvent::IE_Released)
			{
				Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			}
		}
		
		// Right Mouse Button
		if (KeyCode == VK_RBUTTON)
		{
			if (InputEvent == EInputEvent::IE_Pressed)
			{
				Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			}
			if (InputEvent == EInputEvent::IE_Released)
			{
				Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			}
		}

		// Middle Mouse Button
		if (KeyCode == VK_MBUTTON)
		{
			if (InputEvent == EInputEvent::IE_Pressed)
			{
				Input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
			}
			if (InputEvent == EInputEvent::IE_Released)
			{
				Input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
			}
		}
	}
	else
	{
		Input.type = INPUT_KEYBOARD;
		Input.ki.wVk = GetInputKeyCode(Key);
		if (InputEvent == EInputEvent::IE_Released)
		{
			Input.ki.dwFlags = KEYEVENTF_KEYUP;
		}
	}
	
	
	SendInput(1, &Input, sizeof(INPUT));
	return true;
#endif

	// Some problem to be fixed;
	//    1, Modifier key
	//    2, Backspace on text block
	//    3, etc...
	if (InputChar(Key, InputEvent))
	{
		return true;
	}

	if (!InputKeyGamePlay(Key, InputEvent))
	{
		
		return false;
	}

	// UI input
	if (Key.IsMouseButton())
	{
		return InputMouseButtonUI(Key, InputEvent);
	}
	else
	{
		return InputKeyUI(Key, InputEvent);
	}
}

bool URCInputObj::InputKeyTest(const FName KeyName, const EInputEvent InputEvent)
{
	bool bRet = false;

	return bRet;
}

bool URCInputObj::InputChar(const FKey& Key, const EInputEvent& InputEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();
	if (!SlateApp.GetKeyboardFocusedWidget().IsValid())
	{
		return false;
	}

	if (!SlateApp.GetKeyboardFocusedWidget()->SupportsKeyboardFocus())
	{
		return false;
	}

	if (InputEvent == EInputEvent::IE_Released)
	{
		return false;
	}
	FString KeyName = Key.GetFName().ToString();
	TCHAR Character = NULL;
	Character = GetInputCharacter(Key);

	if (Character == NULL)
	{
		return false;
	}

	FCharacterEvent CharacterEvent
	(
		Character,
		SlateApp.GetPlatformApplication()->GetModifierKeys(),
		0,
		false
	);

	return SlateApp.ProcessKeyCharEvent(CharacterEvent);
}

bool URCInputObj::InputKeyGamePlay(const FKey& Key, const EInputEvent& InputEvent)
{
	FInputKeyEventArgs InputArgs
	{
		GEngine->GameViewport->Viewport,
		0,
		Key,
		InputEvent
	};
	
	return GEngine->GameViewport->InputKey(InputArgs);
	// return PlayerController->InputKey(Key, InputEvent, 0, Key.IsGamepadKey());
}

bool URCInputObj::InputKeyUI(const FKey& Key, const EInputEvent& InputEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	FKeyEvent KeyEvent
	{
		Key,
		SlateApp.GetPlatformApplication()->GetModifierKeys(),
		0,
		false,
		0,
		0
	};
	if (InputEvent == EInputEvent::IE_Pressed)
	{
		return SlateApp.ProcessKeyDownEvent(KeyEvent);
	}
	if (InputEvent == EInputEvent::IE_Released)
	{
		return SlateApp.ProcessKeyUpEvent(KeyEvent);
	}
	return false;
}

bool URCInputObj::InputMouseButtonUI(const FKey& Key, const EInputEvent& InputEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	TSharedPtr<FGenericWindow> GenWindow;

	FPointerEvent PointerEvent(
		0,
		SlateApp.CursorPointerIndex,
		SlateApp.GetCursorPos(),
		SlateApp.GetLastCursorPos(),
		SlateApp.GetPressedMouseButtons(),
		Key,
		0,
		SlateApp.GetPlatformApplication()->GetModifierKeys()
	);

	if (InputEvent == EInputEvent::IE_Pressed)
	{
		return SlateApp.ProcessMouseButtonDownEvent(GenWindow, PointerEvent);
	}

	if (InputEvent == EInputEvent::IE_Released)
	{
		return SlateApp.ProcessMouseButtonUpEvent(PointerEvent);
	}
	return false;
}

int URCInputObj::GetInputKeyCode(const FKey& Key)
{
	FInputKeyManager Manager = FInputKeyManager::Get();

	const uint32* KeyCode;
	const uint32* CharCode;
	FString KeyName = Key.GetFName().ToString();

	if (KeyName.Len() == 1)
	{
		return KeyName.GetCharArray()[0];
	}

	Manager.GetCodesFromKey(Key, KeyCode, CharCode);

	if (KeyCode)
	{
		return *KeyCode;
	}

	if (CharCode)
	{
		return *CharCode;
	}

	if (KeyName.Contains("Num"))
	{
		FString NumPad = KeyName.Replace(TEXT("Num"), TEXT(""));
		if (NumPad.IsNumeric())
		{
			return FCString::Atoi(*NumPad) + 0x60;
		}
		if (NumPad == "/")
		{
			return VK_DIVIDE;
		}
		if (NumPad == "*")
		{
			return VK_MULTIPLY;
		}
		if (NumPad == "-")
		{
			return VK_SUBTRACT;
		}
		if (NumPad == "+")
		{
			return VK_ADD;
		}
		if (NumPad == ".")
		{
			return VK_DECIMAL;
		}
	}

	return 0;
}

TCHAR URCInputObj::GetInputCharacter(const FKey& Key)
{
	FInputKeyManager&  InputKeyManager = FInputKeyManager::Get();
	FSlateApplication& SlateApp        = FSlateApplication::Get();

	const uint32* KeyCode  = nullptr;
	const uint32* CharCode = nullptr;

	InputKeyManager.GetCodesFromKey(Key, KeyCode, CharCode);
	
	if (CharCode)
	{
		// A ~ Z
		if (*CharCode >= 65 && *CharCode <= 106)
		{
			return (SlateApp.GetPlatformApplication()->GetModifierKeys().AreCapsLocked() || SlateApp.GetPlatformApplication()->GetModifierKeys().IsLeftShiftDown() || SlateApp.GetPlatformApplication()->GetModifierKeys().IsRightShiftDown()) ? (*CharCode + 32) : (*CharCode);
		}
		
		if (SlateApp.GetPlatformApplication()->GetModifierKeys().IsLeftShiftDown() || SlateApp.GetPlatformApplication()->GetModifierKeys().IsRightShiftDown())
		{
			if (*CharCode >= 48 && *CharCode <= 57)
			{
				

			}
		}
		// 0 ~ 9 
		
		
		return *CharCode;
	}

	FString KeyName = Key.GetFName().ToString();
	if (KeyName.Contains("Num"))
	{
		FString NumPad = KeyName.Replace(TEXT("Num"), TEXT(""));
		if (NumPad.IsNumeric())
		{
			return FCString::Atoi(*NumPad);
		}
		return NumPad[0];
	}
	return NULL;
}
