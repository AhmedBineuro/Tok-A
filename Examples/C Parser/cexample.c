#include "CRules.h"

int main()
{
    ParserContext ctx = CreateParserContext(true);
    ParsingFunction pf;

    pf.fileFunction = ConsumeString_F;
    AddParseRule(&ctx, "String", IsStringStart, pf);

    pf.fileFunction = ConsumeNumber_F;
    AddParseRule(&ctx, "Number", IsNumeric, pf);

    pf.fileFunction = ConsumeIdentifier_F;
    AddParseRule(&ctx, "Identifier", IsIdentiferStart, pf);

    pf.fileFunction = ConsumeSingleCharToken_F;
    AddParseRule(&ctx, "Single Character token", IsSingleCharToken, pf);

    pf.fileFunction = ConsumeWhiteSpace_F;
    AddParseRule(&ctx, "White space muncher", IsWhiteSpace, pf);

    Parse(&ctx, "C:\\Users\\purea\\Documents\\Git_Projects\\PtahLib\\Tokenizer\\test.c");
    for (int i = 0; i < ctx.tokens.size; i++)
    {
        printf("[%ld,%ld]: %s: %s\n", ctx.tokens.arr[i].line, ctx.tokens.arr[i].at, ctx.tokens.arr[i].type, ctx.tokens.arr[i].value.arr);
    }
    FreeParserContext(&ctx);
}