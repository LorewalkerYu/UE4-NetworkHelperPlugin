// Fill out your copyright notice in the Description page of Project Settings.


#include "Obj/RCInputObj.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "Types/SlateStructs.h"
#include "Framework/Application/SlateApplication.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsApplication.h"
DECLARE_DELEGATE_ThreeParams(FMessageDelegate, uint32, WPARAM, LPARAM);

class FRCWindowsMessageHandler
	: public IWindowsMessageHandler
{
public:

	// Just Listen, so always return false
	virtual bool ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam, int32& OutResult) override
	{
		MessageDelegate.ExecuteIfBound(msg, wParam, lParam);
		return false;
	}

public:

	FMessageDelegate MessageDelegate;
};
FRCWindowsMessageHandler RCHandler;

WPARAM MapLeftRightKeys(WPARAM vk, LPARAM lParam)
{
	WPARAM new_vk = vk;
	UINT scancode = (lParam & 0x00ff0000) >> 16;
	int extended = (lParam & 0x01000000) != 0;

	switch (vk) {
	case VK_SHIFT:
		new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
		break;
	case VK_CONTROL:
		new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		new_vk = extended ? VK_RMENU : VK_LMENU;
		break;
	default:
		// not a key we map from generic to left/right specialized
		//  just return it.
		new_vk = vk;
		break;
	}

	return new_vk;
}
#endif

void URCInputObj::InitilizeRCForWindows(ERCType RemoteControlType, FMouseMove MouseMove, FKeyInput KeyInput)
{
	this->RCType = RemoteControlType;
#if PLATFORM_WINDOWS
	switch (RCType)
	{
	case ERCType::ERC_Receiver:
	{

	}break;
	case ERCType::ERC_Sender:
	{
		
		this->MouseMoveDele = MouseMove;
		this->KeyInputDele = KeyInput;
		RCHandler.MessageDelegate.BindLambda([this](uint32 msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_CHAR:
			{
				UE_LOG(LogTemp, Error, TEXT("Char"));
			}break;
			case WM_KEYDOWN:
			{
				const uint32 KeyCode = MapLeftRightKeys(wParam, lParam);
				const uint32 CharCode = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
				FKey Key = FInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharCode);
				this->KeyInputDele.ExecuteIfBound(Key.GetFName(), EInputEvent::IE_Pressed);

				if (!(lParam & 0x40000000))
				{
				}
			}break;
			case WM_KEYUP:
			{
				const uint32 KeyCode = MapLeftRightKeys(wParam, lParam);;
				const uint32 CharCode = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
				FKey Key = FInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharCode);
				this->KeyInputDele.ExecuteIfBound(Key.GetFName(), EInputEvent::IE_Released);
			}break;
			case WM_LBUTTONDOWN:
			{
				const uint32 KeyCode = wParam;
				const uint32 CharCode = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
				FKey Key = FInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharCode);
				
				this->KeyInputDele.ExecuteIfBound(Key.GetFName(), EInputEvent::IE_Pressed);
				
			}break;
			case WM_LBUTTONUP:
			{
				const uint32 KeyCode = VK_LBUTTON;
				const uint32 CharCode = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
				FKey Key = FInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharCode);
				this->KeyInputDele.ExecuteIfBound(Key.GetFName(), EInputEvent::IE_Released);
			}break;
			case WM_RBUTTONDOWN:
			{
				const uint32 KeyCode = wParam;
				const uint32 CharCode = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
				FKey Key = FInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharCode);
				
				this->KeyInputDele.ExecuteIfBound(Key.GetFName(), EInputEvent::IE_Pressed);
				
			}break;
			case WM_RBUTTONUP:
			{
				const uint32 KeyCode = VK_RBUTTON;
				const uint32 CharCode = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
				FKey Key = FInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharCode);
				this->KeyInputDele.ExecuteIfBound(Key.GetFName(), EInputEvent::IE_Released);
			}break;
			case WM_MBUTTONDOWN:
			{
				const uint32 KeyCode = VK_MBUTTON;
				const uint32 CharCode = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
				FKey Key = FInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharCode);
				
				this->KeyInputDele.ExecuteIfBound(Key.GetFName(), EInputEvent::IE_Pressed);
				
			}break;
			case WM_MBUTTONUP:
			{
				const uint32 KeyCode = VK_MBUTTON;
				const uint32 CharCode = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
				FKey Key = FInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharCode);
				this->KeyInputDele.ExecuteIfBound(Key.GetFName(), EInputEvent::IE_Released);
			}break;
			case WM_MOUSEMOVE:
			{
				this->MouseMoveDele.ExecuteIfBound(LOWORD(lParam), HIWORD(lParam));
			}break;
			case WM_MOUSEWHEEL:

			default:
				break;
			}

		});

		FWindowsApplication* WindowsApp = (FWindowsApplication*)FSlateApplication::Get().GetPlatformApplication().Get();
		if (WindowsApp)
		{
			WindowsApp->AddMessageHandler(RCHandler);
		}

	}break;
	}
#endif
}

bool URCInputObj::SetMousePosition(int32 X, int32 Y, int32 OtherViewportSizeX, int32 OtherViewportSizeY)
{
#if PLATFORM_WINDOWS
	return SendInputMouseMove(X, Y, OtherViewportSizeX, OtherViewportSizeY);
#endif
	
	return false;


}

bool URCInputObj::InputMouseWheel(bool bForward)
{
#if PLATFORM_WINDOWS
	return SendInputMouseWheel(bForward);
#endif
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
	return SendInputKey(Key, InputEvent);
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
	FSlateApplication& SlateApp = FSlateApplication::Get();

	const uint32* KeyCode = nullptr;
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

bool URCInputObj::SendInputMouseMove(int32 X, int32 Y, int32 OtherViewportSizeX, int32 OtherViewportSizeY)
{
	// Client Area Rect
	FSlateRect ClientRect = GEngine->GameViewport->GetWindow()->GetClientRectInScreen();

	// x y coordinate convert
	
	double TargetX   = double(X) / double(OtherViewportSizeX) * (ClientRect.Right  - ClientRect.Left);
	double TargetY   = double(Y) / double(OtherViewportSizeY) * (ClientRect.Bottom - ClientRect.Top );

	int ScreenWidth  = ::GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	long real_x      = (TargetX + ClientRect.Left) * double(65535 / ScreenWidth);
	long real_y      = (TargetY + ClientRect.Top ) * double(65535 / ScreenHeight);

	INPUT InputMouse;
	InputMouse.type = INPUT_MOUSE;
	InputMouse.mi.mouseData = 0;
	InputMouse.mi.dx = real_x;
	InputMouse.mi.dy = real_y;
	InputMouse.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	UE_LOG(LogTemp, Error, TEXT("%d, %d  ---   %d, %d"), X, Y, real_x, real_y);
	if (GEngine)
	{
		FString str = FString::FromInt(X) + "," + FString::FromInt(Y) + "   -----   " + FString::FromInt(real_x) + "," + FString::FromInt(real_y);
		GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Blue, *str);
	}
	return ::SendInput(1, &InputMouse, sizeof(INPUT));
}

bool URCInputObj::SendInputKey(const FKey& Key, const EInputEvent& InputEvent)
{
	INPUT Input;
	int KeyCode = GetInputKeyCode(Key);

	if (Key.IsMouseButton())
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
		Input.ki.wVk = KeyCode;
		Input.ki.wScan = MapVirtualKey(KeyCode, MAPVK_VK_TO_VSC);
		Input.ki.dwFlags = 0;
		if (InputEvent == EInputEvent::IE_Released)
		{
			Input.ki.dwFlags = KEYEVENTF_KEYUP;
		}
	}

	
	return ::SendInput(1, &Input, sizeof(INPUT));;
}

bool URCInputObj::SendInputMouseWheel(bool bForward)
{
	INPUT Input;
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	if (bForward)
	{
		Input.mi.mouseData = WHEEL_DELTA;
	}
	else
	{
		Input.mi.mouseData = WHEEL_DELTA * -1;
	}
	
	return ::SendInput(1, &Input, sizeof(INPUT));
}

