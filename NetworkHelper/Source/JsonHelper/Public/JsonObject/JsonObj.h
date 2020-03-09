// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonObj.generated.h"
class FJsonObject;
class FJsonValue;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class JSONHELPER_API UJsonObj : public UObject
{
	GENERATED_BODY()

public:

	void SetJsonObj(TSharedPtr<FJsonObject> NewObj);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
	bool AsJsonStr(FString& JsonStr);

public:

	/**
	* Test validity of object
	*
	* @return	Return true if the object is usable: non-null and not pending kill
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool IsValid()const;

	/**
	 * Checks whether a field with the specified name exists in the object.
	 *
	 * @param FieldName The name of the field to check.
	 * @return true if the field exists, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool HasField(const FString& FieldName) const;

	/**
	 * Removes the field with the specified name.
	 *
	 * @param FieldName The name of the field to remove.
	 * @return Success
	 */
	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool RemoveField(const FString& FieldName);

	/**
	 * Gets the field with the specified name as a number.
	 *
	 * Ensures that the field is present and is of type Json number.
	 *
	 * @param FieldName The name of the field to get.
	 * @return Success
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool GetNumberField(const FString& FieldName, float& OutNumber) const;

	/**
	 * Gets a numeric field and casts to an int32
	 * @return Success
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool GetIntegerField(const FString& FieldName, int32 &OutInteger) const;

	/** Get the field named FieldName as a string
	 *
	 * @return Success
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool GetStringField(const FString& FieldName, FString& OuStr) const;

	/**
	 * Gets the field with the specified name as a boolean.
	 *
	 * Ensures that the field is present and is of type Json number.
	 *
	 * @param FieldName The name of the field to get.
	 * @return The field's value as a boolean.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool GetBoolField(const FString& FieldName, bool& OutBool) const;

	/**
	 * Gets the field with the specified name as a Json object.
	 *
	 * Ensures that the field is present and is of type Json number.
	 *
	 * @param FieldName The name of the field to get.
	 * @return The field's value as a Json object.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		UJsonObj* GetObjectField(const FString& FieldName) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool GetObjectArray(const FString& FieldName, TArray<UJsonObj*>& OutArray);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool GetNumberArray(const FString& FieldName, TArray<bool>& OutArray);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool GetIntegerArray(const FString& FieldName, TArray<int32>& OutArray);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JsonHelper|JsonObj")
		bool GetStringArray(const FString& FieldName, TArray<FString>& OutArray);

public:

	/** Set a boolean field named FieldName and value of InValue */
	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetBoolField(const FString& FieldName, bool InValue);

	/** Add a field named FieldName with Number as value */
	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetIntegerField(const FString& FieldName, int32 Number);

	/** Add a field named FieldName with Number as value */
	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetNumberField(const FString& FieldName, float Number);

	/** Add a field named FieldName with value of StringValue */
	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetStringField(const FString& FieldName, const FString& StringValue);

	/** Set an ObjectField named FieldName and value of JsonObject */
	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetObjectField(const FString& FieldName, const UJsonObj* JsonObject);

	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetObjectArrayField(const FString& FieldName, const TArray<UJsonObj*>& InArray);

	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetNumberArrayField(const FString& FieldName, const TArray<float>& InArray);

	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetIntegerArrayField(const FString& FieldName, const TArray<int32>& InArray);

	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetStringArrayField(const FString& FieldName, const TArray<FString>& InArray);

	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetBoolArrayField(const FString& FieldName, const TArray<bool>& InArray);


public:

	UFUNCTION(BlueprintGetter, Category = "JsonHelper|JsonObj")
		bool GetVector3DField(const FString& FieldName, FVector& OutVector) const;

	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetVector3DField(const FString& FieldName, const FVector& InVector);

	UFUNCTION(BlueprintGetter, Category = "JsonHelper|JsonObj")
		bool GetVector2DField(const FString& FieldName, FVector2D& OutVector2D) const;

	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetVector2DField(const FString& FieldName, const FVector2D& InVector2D);

	UFUNCTION(BlueprintGetter, Category = "JsonHelper|JsonObj")
		bool GetRotatorField(const FString& FieldName, FRotator& OutRotator) const;

	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetRotatorField(const FString& FieldName, const FRotator& InRotator);

	UFUNCTION(BlueprintGetter, Category = "JsonHelper|JsonObj")
		bool GetTransformField(const FString& FieldName, FTransform& OutTransform) const;

	UFUNCTION(BlueprintCallable, Category = "JsonHelper|JsonObj")
		bool SetTransformField(const FString& FieldName, const FTransform& InTransform);
public:

	UFUNCTION(BlueprintGetter, Category = "JsonHelper|JsonObj")
		FString ToString();

protected:

	TSharedPtr<FJsonObject> GetJsonObj()const;

	bool SetArrayField(const FString& FieldName, const TArray<TSharedPtr<FJsonValue>>& Array);

	/** Try to get the field named FieldName as an array, or return false if it's another type */
	bool GetJsonValueArray(const FString& FieldName, const TArray< TSharedPtr<FJsonValue> >*& OutArray);


protected:

	TSharedPtr<FJsonObject> Obj;
	
};
