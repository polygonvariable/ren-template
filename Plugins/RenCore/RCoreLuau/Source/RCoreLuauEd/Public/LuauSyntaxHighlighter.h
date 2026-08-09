// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Framework/Text/SyntaxHighlighterTextLayoutMarshaller.h"


/*
 *
 */
class FLuauSyntaxHighlighter : public FSyntaxHighlighterTextLayoutMarshaller
{

public:

	static TSharedRef<FLuauSyntaxHighlighter> Create();

protected:

	// ~ FSyntaxHighlighterTextLayoutMarshaller
	virtual void ParseTokens(const FString& SourceString, FTextLayout& TargetTextLayout, TArray<ISyntaxTokenizer::FTokenizedLine> TokenizedLines) override;
	// ~ End of FSyntaxHighlighterTextLayoutMarshaller

private:

	FLuauSyntaxHighlighter(TSharedPtr<ISyntaxTokenizer> InTokenizer);

	TSet<FString> Keywords;
	TSet<FString> Symbols;

	FTextBlockStyle NormalStyle;
	FTextBlockStyle KeywordStyle;
	FTextBlockStyle SymbolStyle;

};

