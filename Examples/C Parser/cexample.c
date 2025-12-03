#include "./CRules.h"

int main(int argc, char **argv)
{

    printf("To tokenize files add the paths of the files as arguments to this executable!\n");

    ParserContext ctx = CreateParserContext(true);
    /*
    Parsing function is a union that can either
    be a file parsing function pointer or a string parsing
    function
    */
    ParsingFunction pf;

    pf.fileFunction = ConsumeString_F;
    AddParseRule(&ctx, "String", IsStringStart, pf);

    pf.fileFunction = ConsumeComment_F;
    AddParseRule(&ctx, "String", IsCommentStart, pf);

    pf.fileFunction = ConsumeChar_F;
    AddParseRule(&ctx, "Char", IsCharStart, pf);
    pf.fileFunction = ConsumeNumber_F;
    AddParseRule(&ctx, "Number", IsNumeric, pf);

    pf.fileFunction = ConsumeIdentifier_F;
    AddParseRule(&ctx, "Identifier", IsIdentiferStart, pf);
    pf.fileFunction = ConsumeSingleCharToken_F;
    AddParseRule(&ctx, "Single Character token", IsSingleCharToken, pf);

    pf.fileFunction = ConsumeWhiteSpace_F;
    AddParseRule(&ctx, "White space muncher", IsWhiteSpace, pf);

    if (argc == 1)
    {
        Parse(&ctx, "test.c");
        for (int i = 0; i < ctx.tokens.size; i++)
        {
            printf("[%ld,%ld]: %s: %s\n", ctx.tokens.arr[i].line, ctx.tokens.arr[i].at, ctx.tokens.arr[i].type, ctx.tokens.arr[i].value.arr);
        }
    }
    else
    {

        for (int i = 1; i < argc; i++)
        {
            printf("================File %d: %s================\n", i - 1, argv[i]);
            Parse(&ctx, argv[i]);
            for (int i = 0; i < ctx.tokens.size; i++)
            {
                printf("[%ld,%ld]: %s: %s\n", ctx.tokens.arr[i].line, ctx.tokens.arr[i].at, ctx.tokens.arr[i].type, ctx.tokens.arr[i].value.arr);
            }
            ResetContext(false, &ctx);
        }
    }
    FreeParserContext(&ctx);
}