// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketLibrary.h"
#include "SecureHash.h"
#include "Base64.h"
#include <string>
#include <SocketSubsystem.h>
#include <IPAddress.h>



void UWebSocketLibrary::StringToBytes(const FString& message, TArray<uint8>& DataBuffer)
{

	std::string _str = TCHAR_TO_UTF8(*message);

	DataBuffer.SetNum(_str.size() + 1);
	DataBuffer[DataBuffer.Num() - 1] = 0x00;

	FMemory::Memcpy(DataBuffer.GetData(), _str.c_str(), _str.size());

}

void UWebSocketLibrary::BytesToString(const TArray<uint8>& InByte, FString& OutStr)
{
	if (InByte.Num() <= 0)
	{
		return;
	}
	if (InByte[InByte.Num() - 1] == 0x00)
	{
		OutStr = UTF8_TO_TCHAR(InByte.GetData());
		return;
	}

	TArray<uint8> tempBuffer;
	tempBuffer.SetNum(InByte.Num() + 1);
	FMemory::Memcpy(tempBuffer.GetData(), InByte.GetData(), InByte.Num());
	tempBuffer[tempBuffer.Num() - 1] = 0x00;

	OutStr = UTF8_TO_TCHAR(tempBuffer.GetData());
}

FString UWebSocketLibrary::SHA1ThenBase64(const FString& InStr)
{
	return SHA1ToBase64(StrToSHA1(InStr).Hash);
}

FSHAHash UWebSocketLibrary::StrToSHA1(const FString& InStr)
{
	FSHAHash StringHash;
	FSHA1::HashBuffer(TCHAR_TO_ANSI(*InStr), InStr.Len(), StringHash.Hash);
	return StringHash;
}

FString UWebSocketLibrary::SHA1ToBase64(const uint8* InHash)
{
	return FBase64::Encode(InHash, 20);
}

void UWebSocketLibrary::Int32ToUInt8(int32 InValue, TArray<uint8>& OutBytes)
{
	FMemory::Memcpy(OutBytes.GetData(), &InValue, 4);
}

FString UWebSocketLibrary::GetLocalIP()
{
	bool canBind = false;
	TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	return (localIp->IsValid() ? localIp->ToString(false) : "");
}


