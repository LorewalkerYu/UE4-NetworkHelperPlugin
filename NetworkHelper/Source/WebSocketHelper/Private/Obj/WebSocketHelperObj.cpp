// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketHelperObj.h"
#include "WebSocketLibrary.h"
DEFINE_LOG_CATEGORY(LogWebsocketHelper);
void UWebSocketHelperObj::ServerHandshakeResponse(const FString& ClientHandShakeRequest, TArray<uint8>& OutResponse)
{
	if (ClientHandShakeRequest.Len() <= 0)
	{
		return;
	}
	// Find Sec-WebSocket-Key
	FString SecWebSocketKey;
	TArray<FString> RequestArray;
	ClientHandShakeRequest.ParseIntoArrayLines(RequestArray);

	for (int32 i = 0; i < RequestArray.Num(); ++i)
	{
		if (!RequestArray[i].Contains(TEXT("Sec-WebSocket-Key")))
		{
			continue;
		}
		FString TargetStr;
		TargetStr = RequestArray[i].Replace(TEXT("Sec-WebSocket-Key: "), TEXT(""));
		TargetStr = TargetStr.Replace(TEXT("\r"), TEXT(""));
		TargetStr = TargetStr.Replace(TEXT("\n"), TEXT(""));
		SecWebSocketKey = TargetStr;

		break;
	}

	// Make Response String
	FString RetStr = FString();
	RetStr.Append("HTTP/1.1 101 Switching Protocols\r\n");
	RetStr.Append("Upgrade: websocket\r\n");
	RetStr.Append("Connection: Upgrade\r\n");
	RetStr.Append("Sec-WebSocket-Accept: " + GenerateSecWebSocketAcceptStr(SecWebSocketKey) + "\r\n");
	RetStr.Append("\r\n");

	OutResponse.Empty();
	OutResponse.Append(RetStr.GetCharArray());

	// Remove '\0' , if with one
	/*if (OutResponse[OutResponse.Num() - 1] == '\0')
	{
		OutResponse.RemoveAt(OutResponse.Num() - 1);
	}*/
}

FString UWebSocketHelperObj::GenerateSecWebSocketAcceptStr(const FString& SecWebSocketKey) const
{
	return UWebSocketLibrary::SHA1ThenBase64(SecWebSocketKey + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
}

void UWebSocketHelperObj::DecodeFrame(const TArray<uint8>& InFrame, FWebSocketFrame& FrameContent)
{
	if (InFrame.Num() < 2)
	{
		return;
	}
	TArray<uint8> WebSocketFrame;
	WebSocketFrame.Append(InFrame);

	//    0               1               2               3           
	//    0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
	//   +-+-+-+-+-------+-+-------------+-------------------------------+
	//   |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
	//   |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
	//   |N|V|V|V|       |S|             |   (if payload len==126/127)   |
	//   | |1|2|3|       |K|             |                               |
	//   +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
	//   |     Extended payload length continued, if payload len == 127  |
	//   + - - - - - - - - - - - - - - - +-------------------------------+
	//   |                               |Masking-key, if MASK set to 1  |
	//   +-------------------------------+-------------------------------+
	//   | Masking-key (continued)       |          Payload Data         |
	//   +-------------------------------- - - - - - - - - - - - - - - - +
	//   :                     Payload Data continued ...                :
	//   + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
	//   |                     Payload Data continued ...                |
	//   +---------------------------------------------------------------+

	  // Byte 0
	uint8 FIN = (WebSocketFrame[0] & 0x80) >> 7;   // 1000 0000
	uint8 RSV1 = (WebSocketFrame[0] & 0x40) >> 6;   // 0100 0000
	uint8 RSV2 = (WebSocketFrame[0] & 0x20) >> 5;   // 0010 0000
	uint8 RSV3 = (WebSocketFrame[0] & 0x10) >> 4;   // 0001 0000
	uint8 Opcode = WebSocketFrame[0] & 0xf;          // 0000 1111

	// Byte 1
	uint8 MASK = (WebSocketFrame[1] & 0x80) >> 7;   // 1000 0000
	uint8 PayloadLen = WebSocketFrame[1] & 0x7f;         // 0111 1111


	uint64 PayloadDataLength = 0;
	int32  PayloadDataBeginIndex = 2;
	/*if (MASK == 1)
	{
		PayloadDataBeginIndex =
	}*/
	if (PayloadLen <= 125)
	{
		PayloadDataLength = PayloadLen;
	}
	else
	{
		int LoopCount = 0;
		uint64 Len = WebSocketFrame[2];
		if (PayloadLen == 126)
		{
			LoopCount = 1;
			PayloadDataBeginIndex += 2;
		}
		else if (PayloadLen == 127)
		{
			LoopCount = 8;
			PayloadDataBeginIndex += 8;

		}
		for (int i = 0; i < LoopCount; ++i)
		{
			Len = (Len << 8) | WebSocketFrame[3 + i];
		}
		PayloadDataLength = Len;
	}

	TArray<uint8> MaskingByte;
	TArray<uint8> PayloadData;
	TArray<uint8> Message;
	FString MessageStr;
	if (MASK == 1)
	{
		for (int32 i = PayloadDataBeginIndex; i < PayloadDataBeginIndex + 4; ++i)
		{
			MaskingByte.Add(WebSocketFrame[i]);
		}
		PayloadDataBeginIndex += 4;
		for (int32 i = PayloadDataBeginIndex; i < PayloadDataBeginIndex + PayloadDataLength; ++i)
		{
			PayloadData.Add(WebSocketFrame[i]);
		}
		for (int i = 0; i < PayloadData.Num(); i++)
		{
			Message.Add((uint8)(PayloadData[i] ^ MaskingByte[i % 4]));
			// BufferToString
		}
		UWebSocketLibrary::BytesToString(Message, MessageStr);
	}

	FrameContent = FWebSocketFrame(FIN, static_cast<EFrameOpcode>(Opcode), MASK, MessageStr);
	UE_LOG(LogWebsocketHelper, Error, TEXT("Fin : %d, Opcode : %d, MASK : %d, PayloadLen : %d, Message : %s"), FIN, Opcode, MASK, PayloadDataLength, *MessageStr);

}

int32 UWebSocketHelperObj::EncodeFrame(const FWebSocketFrame& FrameContent, TArray<uint8>& OutFrames)
{
	TArray<uint8> MessageByte;


	uint8 FINByte = (uint8)FrameContent.FIN;
	uint8 RSV1Byte = (uint8)FrameContent.RSV1;
	uint8 RSV2Byte = (uint8)FrameContent.RSV2;
	uint8 RSV3Byte = (uint8)FrameContent.RSV3;
	uint8 MASKByte = (uint8)FrameContent.MASK;

	uint8 OpcodeByte = static_cast<uint8>(FrameContent.Opcode);

	uint8 FirstByte = ((FINByte << 7) & 0xff) + ((RSV1Byte << 6) & 0xff) + ((RSV2Byte << 5) & 0xff) + ((RSV3Byte << 4) & 0xff) + OpcodeByte;
	MessageByte.Add(FirstByte);


	uint64 PayloadDataLen = 0;
	TArray<uint8> PayloadData;
	UWebSocketLibrary::StringToBytes(FrameContent.Message, PayloadData);


	if (PayloadData.Num() <= 125)
	{
		PayloadDataLen = PayloadData.Num();
		MessageByte.Add(((MASKByte << 7) & 0x40) + PayloadDataLen);
	}
	else
	{
		if (PayloadData.Num() > 125 && PayloadData.Num() <= 65535)
		{
			PayloadDataLen = 126;
			MessageByte.Add(((MASKByte << 7) & 0x7f) + PayloadDataLen);
			MessageByte.Add(PayloadData.Num() >> 8);
			MessageByte.Add(PayloadData.Num() & 0xff);

		}
		else if (PayloadData.Num() > 65535)
		{
			PayloadDataLen = 127;
			MessageByte.Add(((MASKByte << 7) & 0x7f) + PayloadDataLen);

			uint64 DataLength = PayloadData.Num();
			for (int i = 8; i > 0; --i)
			{
				uint64 a = DataLength << (8 - i) * 8;
				MessageByte.Add(a >> 56);
			}
		}
		else
		{
			UE_LOG(LogWebsocketHelper, Error, TEXT("Message is out of renge"));
			return 1;
		}
	}
	if (MASKByte)
	{
		UE_LOG(LogWebsocketHelper, Error, TEXT("As server, MASK should not be set"));
	}
	else
	{
		for (const uint8& elem : PayloadData)
		{
			MessageByte.Add(elem);
		}
	}

	OutFrames = MessageByte;
	return 0;
}
