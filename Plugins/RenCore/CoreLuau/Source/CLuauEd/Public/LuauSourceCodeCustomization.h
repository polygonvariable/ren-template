// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "IPropertyTypeCustomization.h"

// Forward Declaration
struct FLuauSourceCode;


/*
 *
 */
class FLuauSourceCodeCustomization : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// ~ IPropertyTypeCustomization
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	// ~ End of IPropertyTypeCustomization

protected:

	FReply CompileCode();
	FReply ClearCode();

private:

	TSharedPtr<IPropertyHandle> ParentHandle = nullptr;
	TSharedPtr<IPropertyHandle> CodeProperty = nullptr;
	TSharedPtr<IPropertyHandle> BytecodeProperty = nullptr;
	TSharedPtr<IPropertyHandleArray> BytecodeArrayProperty = nullptr;

	bool GetParent(FLuauSourceCode*& Struct) const;

};

