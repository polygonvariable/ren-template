// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "LuauSyntaxHighlighter.h"

// Engine Headers
#include "Widgets/Text/STextBlock.h"
#include "Framework/Text/SyntaxTokenizer.h"
#include "Framework/Text/SlateTextRun.h"
#include "Styling/CoreStyle.h"


TSharedRef<FLuauSyntaxHighlighter> FLuauSyntaxHighlighter::Create()
{
    TArray<FSyntaxTokenizer::FRule> TokenizerRules;

    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("function")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("if")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("and")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("or")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("then")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("end")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("return")));

    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("string")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("number")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("boolean")));

    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("=")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("+")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("-")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("*")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("/")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT(">")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("<")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT("(")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT(")")));
    TokenizerRules.Emplace(FSyntaxTokenizer::FRule(TEXT(":")));
    
    TSharedRef<ISyntaxTokenizer> Tokenizer = FSyntaxTokenizer::Create(TokenizerRules);

    return MakeShareable(new FLuauSyntaxHighlighter(Tokenizer));
}

void FLuauSyntaxHighlighter::ParseTokens(const FString& SourceString, FTextLayout& TargetTextLayout, TArray<ISyntaxTokenizer::FTokenizedLine> TokenizedLines)
{
    TArray<FTextLayout::FNewLineData> LinesToAdd;
    

    for (const ISyntaxTokenizer::FTokenizedLine& Line : TokenizedLines)
    {
        TSharedRef<FString> ModelString = MakeShared<FString>();
        TArray<TSharedRef<IRun>> Runs;

        for (const ISyntaxTokenizer::FToken& Token : Line.Tokens)
        {
            FString TokenString = SourceString.Mid(Token.Range.BeginIndex, Token.Range.Len());

            FTextRange LocalRange;
            LocalRange.BeginIndex = ModelString->Len();
            LocalRange.EndIndex = LocalRange.BeginIndex + TokenString.Len();

            ModelString->Append(TokenString);

            const FTextBlockStyle* RunStyle = &NormalStyle;

            if (Token.Type == ISyntaxTokenizer::ETokenType::Syntax)
            {
                if (Keywords.Contains(TokenString))
                {
                    RunStyle = &KeywordStyle;
                }
                else if (Symbols.Contains(TokenString))
                {
                    RunStyle = &SymbolStyle;
                }
            }

            TSharedRef<FSlateTextRun> Run = FSlateTextRun::Create(FRunInfo(), ModelString, *RunStyle, LocalRange);
            Runs.Add(Run);
        }

        LinesToAdd.Emplace(ModelString, Runs);
    }

    TargetTextLayout.AddLines(LinesToAdd);
}

FLuauSyntaxHighlighter::FLuauSyntaxHighlighter(TSharedPtr<ISyntaxTokenizer> InTokenizer) : FSyntaxHighlighterTextLayoutMarshaller(InTokenizer)
{
    Keywords.Add(TEXT("function"));
    Keywords.Add(TEXT("if"));
    Keywords.Add(TEXT("and"));
    Keywords.Add(TEXT("or"));
    Keywords.Add(TEXT("then"));
    Keywords.Add(TEXT("end"));
    Keywords.Add(TEXT("return"));

    Keywords.Add(TEXT("string"));
    Keywords.Add(TEXT("number"));
    Keywords.Add(TEXT("boolean"));

    Symbols.Add(TEXT("="));
    Symbols.Add(TEXT("+"));
    Symbols.Add(TEXT("-"));
    Symbols.Add(TEXT("*"));
    Symbols.Add(TEXT("/"));
    Symbols.Add(TEXT(">"));
    Symbols.Add(TEXT("<"));
    Symbols.Add(TEXT("("));
    Symbols.Add(TEXT(")"));
    Symbols.Add(TEXT(":"));

    NormalStyle = FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");

    KeywordStyle = NormalStyle;
    SymbolStyle = NormalStyle;

    NormalStyle.SetColorAndOpacity(FSlateColor(FLinearColor(FColor::FromHex("#C7696E"))));
    KeywordStyle.SetColorAndOpacity(FSlateColor(FLinearColor(FColor::FromHex("#705BCF"))));
    SymbolStyle.SetColorAndOpacity(FSlateColor(FLinearColor(FColor::FromHex("#3F91E8"))));
}