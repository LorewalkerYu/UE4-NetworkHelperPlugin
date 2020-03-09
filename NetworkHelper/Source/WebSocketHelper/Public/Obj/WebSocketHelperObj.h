// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WebSocketHelperObj.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogWebsocketHelper, Log, All);

UENUM(BlueprintType)
enum class EFrameOpcode : uint8
{
	EFT_EMPTY_FRAME   = 0xF0     UMETA(DisplayName = "EmptyFrame"),
	EFT_ERROR_FRAME   = 0xF1     UMETA(DisplayName = "ErrorFrame"),
	EFT_TEXT_FRAME    = 0x01     UMETA(DisplayName = "TextFrame"),
	EFT_BINARY_FRAME  = 0x02     UMETA(DisplayName = "BinaryFrame"),
	EFT_PING_FRAME    = 0x09     UMETA(DisplayName = "PingFrame"),
	EFT_PONG_FRAME    = 0x0A     UMETA(DisplayName = "PongFrame"),
	EFT_OPENING_FRAME = 0xF3     UMETA(DisplayName = "OpeningFrame"),
	EFT_CLOSING_FRAME = 0x08     UMETA(DisplayName = "ClosingFrame")
};

USTRUCT(BlueprintType)
struct FWebSocketFrame
{
	GENERATED_USTRUCT_BODY()
public:

	FWebSocketFrame() {}

	FWebSocketFrame(int32 InFIN, EFrameOpcode InOpcode, int32 InMASK, const FString& InMessage) :
		FIN(InFIN),
		Opcode(InOpcode),
		MASK(InMASK),
		Message(InMessage)
	{

	}
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FIN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RSV1 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RSV2 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RSV3 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFrameOpcode Opcode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MASK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Message;
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WEBSOCKETHELPER_API UWebSocketHelperObj : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "WebSocketPlugin|WebSocketPack")
		void ServerHandshakeResponse(const FString& ClientHandShakeRequest, TArray<uint8>& OutResponse);

	FString GenerateSecWebSocketAcceptStr(const FString& SecWebSocketKey) const;

public:

	UFUNCTION(BlueprintCallable, Category = "WebSocketPlugin|WebSocketPack")
		void DecodeFrame(const TArray<uint8>& InFrame, FWebSocketFrame& FrameContent);

	UFUNCTION(BlueprintCallable, Category = "WebSocketPlugin|WebSocketPack")
		int32 EncodeFrame(const FWebSocketFrame& FrameContent, TArray<uint8>& OutFrames);

};
