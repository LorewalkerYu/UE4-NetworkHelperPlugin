// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WebSocketLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WEBSOCKETHELPER_API UWebSocketLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "WebSocketPlugin|Library")
		static void StringToBytes(const FString& InStr, TArray<uint8>& OutByte);

	UFUNCTION(BlueprintPure, Category = "WebSocketPlugin|Library")
		static void BytesToString(const TArray<uint8>& InByte, FString& OutStr);

	UFUNCTION(BlueprintPure, Category = "WebSocketPlugin|Library")
		static FString SHA1ThenBase64(const FString& InStr);

protected:
	static FSHAHash StrToSHA1(const FString& InStr);

	static FString SHA1ToBase64(const uint8* InHash);
	UFUNCTION(BlueprintPure, Category = "WebSocketPlugin|Library")
		static void Int32ToUInt8(int32 InValue, TArray<uint8>& OutBytes);

	UFUNCTION(BlueprintPure, Category = "WebSocketPlugin|Library")
		static FString GetLocalIP();
};
