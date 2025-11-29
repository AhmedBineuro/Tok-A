#ifndef CRULES_H
#define CRULES_H
#include "./Toka.h"
#include "string.h"
/**
 * @brief This is a functional example of how to use Toke.
 * In this example, we will be parsing a c file down to its tokens
 */
#define SINGLE_CHAR_TOKEN_COUNT 23
#define KEYWORD_TOKEN_COUNT 29
static char SingleCharTokens[SINGLE_CHAR_TOKEN_COUNT] = {
    '~',
    '\"',
    '\'',
    ';',
    '+',
    '-',
    '/',
    '\\',
    '*',
    '=',
    '%',
    '>',
    '<',
    '^',
    '!',
    '(',
    ')',
    '[',
    ']',
    '.',
    ',',
    '&',
    '#'};
static char *SingleCharTokenNames[SINGLE_CHAR_TOKEN_COUNT] = {
    "TILDA",
    "DBL_QUOTE",
    "SNGL_QUOTE",
    "SEMICOLON",
    "PLUS",
    "MINUS",
    "FRWRD_SLASH",
    "BACKSLASH",
    "ASTERISK",
    "EQUALS",
    "PERCENT",
    "GREATER_THAN",
    "LESS_THAN",
    "CARET",
    "EXCALAMATION",
    "L_PAREN",
    "R_PAREN",
    "L_SBRACK",
    "R_SBRACK",
    "PERIOD",
    "COMMA",
    "AMPERSAND",
    "HASHTAG"};
static const char *Keywords[KEYWORD_TOKEN_COUNT] = {
    "void",
    "char",
    "short",
    "int",
    "long",
    "float",
    "double",
    "signed",
    "unsigned",
    "const",
    "volatile",
    "static",
    "extern",
    "typedef",
    "if",
    "else",
    "switch",
    "case",
    "default",
    "for",
    "do",
    "while",
    "break",
    "continue",
    "return",
    "struct",
    "union",
    "enum",
    "sizeof"};
enum TokenTypes
{
    TILDA,
    DBL_QUOTE,
    SNGL_QUOTE,
    SEMICOLON,
    PLUS,
    MINUS,
    FRWRD_SLASH,
    BACKSLASH,
    ASTERISK,
    EQUALS,
    PERCENT,
    GREATER_THAN,
    LESS_THAN,
    CARET,
    EXCALAMATION,
    L_PAREN,
    R_PAREN,
    L_SBRACK,
    R_SBRACK,
    PERIOD,
    COMMA,
    AMPERSAND,
    HASHTAG,
    STRING_LITERAL,
    INTEGER,
    FLOAT,
    IDENTIFIER,
};

// Parsing condition functions
bool IsAlphaNumeric(char c);
bool IsAlphabetic(char c);
bool IsNumeric(char c);
bool IsWhiteSpace(char c);
bool IsNewLine(char c);
bool IsStringStart(char c);
bool IsIdentiferStart(char c);
bool IsSingleCharToken(char c);

// This function will take a token and set the value of its type if it's a keyword
bool KeywordFilter(Token *t);
// This function will take any token and set it to an integer, or float
bool FormatFilter(Token *t);

// Parsing functions
// There are two parsing modes, one for parsing strings, the other is for parsing files
// We will define any parsing function for the file with the '_F' suffix and the the string
// parsing functions with the '_S'
// You only need to define a single set of the functions depending on whether you're planning to parse a file
// Or string

void ConsumeWhiteSpace_S(ParserContext *ctx, char c, char *str);
void ConsumeWhiteSpace_F(ParserContext *ctx, char c, FILE *file);

void ConsumeSingleCharToken_S(ParserContext *ctx, char c, char *str);
void ConsumeSingleCharToken_F(ParserContext *ctx, char c, FILE *file);

void ConsumeString_S(ParserContext *ctx, char c, char *str);
void ConsumeString_F(ParserContext *ctx, char c, FILE *file);

void ConsumeIdentifier_S(ParserContext *ctx, char c, char *str);
void ConsumeIdentifier_F(ParserContext *ctx, char c, FILE *file);

void ConsumeInteger_S(ParserContext *ctx, char c, char *str);
void ConsumeInteger_F(ParserContext *ctx, char c, FILE *file);

inline bool IsAlphaNumeric(char c)
{
    return (IsAlphabetic(c) || IsNumeric(c));
}
inline bool IsAlphabetic(char c)
{
    return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'));
}
inline bool IsNumeric(char c)
{
    return (((c >= '0') && (c <= '9')));
}
inline bool IsStringStart(char c)
{
    return (c == '\"');
}
inline bool IsIdentiferStart(char c)
{
    return (IsAlphabetic(c) || (c == '_'));
}
inline bool IsSingleCharToken(char c)
{
    for (int i = 0; i < SINGLE_CHAR_TOKEN_COUNT; i++)
    {
        if (SingleCharTokens[i] == c)
        {
            return true;
        }
    }
    return false;
}
inline bool IsWhiteSpace(char c)
{
    return ((c == ' ') || (c == '\n'));
}

inline bool KeywordFilter(Token *t)
{
    for (int i = 0; i < KEYWORD_TOKEN_COUNT; i++)
    {
        if (strcmp(Keywords[i], t->value.arr) == 0)
        {
            t->typeId = SINGLE_CHAR_TOKEN_COUNT + i;
            t->type = (char *)Keywords[i];
            return true;
        }
    }
    return false;
}
inline bool FormatFilter(Token *t)
{
    bool decimalFound = false;
    for (int i = 0; t->value.arr[i] != '\0'; i++)
    {
        if (IsNumeric(t->value.arr[i]))
            continue;
        else if (t->value.arr[i] == '.' && !decimalFound)
        {
            decimalFound = true;
        }
        else
            return false;
    }
    if (decimalFound)
        SET_TOKEN_ENUM_TYPE((*t), FLOAT)
    else
        SET_TOKEN_ENUM_TYPE((*t), INTEGER)
    return true;
}
inline void ConsumeWhiteSpace_S(ParserContext *ctx, char c, char *str)
{
    while (c != '\0')
    {
        if (c == ' ')
        {
            ctx->cursorOffset++;
            ctx->charNumber++;
        }
        else if (c == '\n')
        {
            ctx->lineNumber++;
            ctx->charNumber = 1;
        }
        else if (!IsWhiteSpace(c))
        {
            ctx->cursorOffset--;
            if (ctx->charNumber > 1)
                ctx->charNumber--;
            break;
        }
        c = str[ctx->cursorOffset];
    }
}
inline void ConsumeWhiteSpace_F(ParserContext *ctx, char c, FILE *file)
{
    while (c != '\0')
    {
        if (c == ' ')
        {
            ctx->cursorOffset++;
            ctx->charNumber++;
        }
        else if (c == '\n')
        {
            ctx->lineNumber++;
            ctx->charNumber = 1;
        }
        else if (!IsWhiteSpace(c))
        {
            ctx->cursorOffset--;
            if (ctx->charNumber > 1)
                ctx->charNumber--;
            ungetc(c, file);
            break;
        }
        c = fgetc(file);
    }
}
inline void ConsumeSingleCharToken_S(ParserContext *ctx, char c, char *str)
{
    Token t;
    INIT_ARRAY(char, t.value, 2);
    APPEND_TO_ARRAY(char, t.value, c)
    APPEND_TO_ARRAY(char, t.value, '\0')
    for (int i = 0; i < SINGLE_CHAR_TOKEN_COUNT; i++)
    {
        if (c == SingleCharTokens[i])
        {
            t.typeId = i;
            t.type = (char *)SingleCharTokenNames[i];
            t.at = ctx->charNumber;
            t.line = ctx->lineNumber;
            break;
        }
    }
    ctx->charNumber++;
    ctx->cursorOffset++;
    APPEND_TO_ARRAY(Token, ctx->tokens, t)
}
inline void ConsumeSingleCharToken_F(ParserContext *ctx, char c, FILE *file)
{
    // We will use the string parser since there is access to the pointer
    ConsumeSingleCharToken_S(ctx, c, NULL);
}
void ConsumeString_S(ParserContext *ctx, char c, char *str)
{
    Token t;
    if (c != '\"')
        return;
    t.line = ctx->lineNumber;
    t.at = ctx->charNumber;
    INIT_ARRAY(char, t.value, 0);
    long at = ctx->charNumber;
    long cursorPos = ctx->cursorOffset;
    bool firstQuote = true;
    while ((c != '\0') && (c != '\n'))
    {
        APPEND_TO_ARRAY(char, t.value, c)
        if (c == '\"')
        {
            if (!firstQuote)
            {
                cursorPos += 1;
                ctx->cursorOffset = cursorPos;
                ctx->charNumber = at;
                SET_TOKEN_ENUM_TYPE(t, STRING_LITERAL)
                break;
            }
            else
                firstQuote = false;
        }
        at++;
        cursorPos++;
        c = str[cursorPos];
    }
    // Failed parse (Free string)
    if (ctx->cursorOffset != cursorPos)
    {
        FREE_ARRAY(t.value);
    }
    else
    {
        APPEND_TO_ARRAY(char, t.value, '\0')
        APPEND_TO_ARRAY(Token, ctx->tokens, t);
    };
}
void ConsumeString_F(ParserContext *ctx, char c, FILE *file)
{
    Token t;
    if (c != '\"')
        return;
    t.line = ctx->lineNumber;
    t.at = ctx->charNumber;
    INIT_ARRAY(char, t.value, 0);
    long at = ctx->charNumber;
    long cursorPos = ctx->cursorOffset;
    bool firstQuote = true;
    while ((c != EOF) && (c != '\n'))
    {
        APPEND_TO_ARRAY(char, t.value, c)
        if (c == '\"')
        {
            if (!firstQuote)
            {
                cursorPos += 1;
                ctx->cursorOffset = cursorPos;
                ctx->charNumber = at;
                SET_TOKEN_ENUM_TYPE(t, STRING_LITERAL)
                break;
            }
            else
                firstQuote = false;
        }
        at++;
        cursorPos++;
        c = fgetc(file);
    }
    // Failed parse (Free string)
    if (ctx->cursorOffset != cursorPos)
    {
        FREE_ARRAY(t.value);
        fseek(file, ctx->cursorOffset, SEEK_SET);
    }
    else
    {
        APPEND_TO_ARRAY(char, t.value, '\0')
        APPEND_TO_ARRAY(Token, ctx->tokens, t);
    };
}

void ConsumeIdentifier_S(ParserContext *ctx, char c, char *str)
{
    Token t;
    t.line = ctx->lineNumber;
    t.at = ctx->charNumber;
    INIT_ARRAY(char, t.value, 0);
    long at = ctx->charNumber;
    long cursorPos = ctx->cursorOffset;
    while (c != '\0' && (IsAlphaNumeric(c) || c == '_'))
    {
        if ((c == '\n') || (c == ' ') || IsSingleCharToken(c))
        {
            cursorPos--;
            ctx->cursorOffset = cursorPos;
            SET_TOKEN_ENUM_TYPE(t, IDENTIFIER);
            ctx->charNumber = at;
            break;
        }
        APPEND_TO_ARRAY(char, t.value, c)
        cursorPos++;
        at++;
        c = str[cursorPos];
    }
    // Will determine if it is a keyword
    // Failed parse (Free string)
    if (ctx->cursorOffset != cursorPos)
    {
        FREE_ARRAY(t.value);
    }
    else
    {
        APPEND_TO_ARRAY(char, t.value, '\0')
        KeywordFilter(&t);
        APPEND_TO_ARRAY(Token, ctx->tokens, t);
    };
}
void ConsumeIdentifier_F(ParserContext *ctx, char c, FILE *file)
{
    Token t;
    t.line = ctx->lineNumber;
    t.at = ctx->charNumber;
    INIT_ARRAY(char, t.value, 0);
    long at = ctx->charNumber;
    long cursorPos = ctx->cursorOffset;
    while (c != EOF)
    {
        if (((!IsAlphaNumeric(c)) && (c != '_')) && ((c == '\n') || (c == ' ') || IsSingleCharToken(c)))
        {
            cursorPos--;
            ctx->cursorOffset = cursorPos;
            SET_TOKEN_ENUM_TYPE(t, IDENTIFIER);
            ctx->charNumber = at;
            ungetc(c, file);
            break;
        }
        APPEND_TO_ARRAY(char, t.value, c)
        cursorPos++;
        at++;
        c = fgetc(file);
    }
    // Will determine if it is a keyword
    // Failed parse (Free string)
    if (ctx->cursorOffset != cursorPos)
    {
        FREE_ARRAY(t.value);
        fseek(file, ctx->cursorOffset, SEEK_SET);
    }
    else
    {
        APPEND_TO_ARRAY(char, t.value, '\0')
        KeywordFilter(&t);
        APPEND_TO_ARRAY(Token, ctx->tokens, t);
    };
}
inline void ConsumeInteger_S(ParserContext *ctx, char c, char *str)
{
    Token t;
    t.line = ctx->lineNumber;
    t.at = ctx->charNumber;
    INIT_ARRAY(char, t.value, 0);
    long at = ctx->charNumber;
    long cursorPos = ctx->cursorOffset;
    while (c != '\0')
    {
        if ((!IsNumeric(c)) &&
            (IsWhiteSpace(c) ||
             IsSingleCharToken(c)))
        {
            cursorPos--;
            ctx->cursorOffset = cursorPos;
            SET_TOKEN_ENUM_TYPE(t, INTEGER);
            ctx->charNumber = at;
            break;
        }
        APPEND_TO_ARRAY(char, t.value, c)
        cursorPos++;
        at++;
        c = str[cursorPos];
    }
    // Will determine if it is a keyword
    // Failed parse (Free string)
    if (ctx->cursorOffset != cursorPos)
    {
        FREE_ARRAY(t.value);
    }
    else
    {
        APPEND_TO_ARRAY(char, t.value, '\0')
        APPEND_TO_ARRAY(Token, ctx->tokens, t);
    };
}
inline void ConsumeInteger_F(ParserContext *ctx, char c, FILE *file)
{
    Token t;
    t.line = ctx->lineNumber;
    t.at = ctx->charNumber;
    INIT_ARRAY(char, t.value, 0);
    long at = ctx->charNumber;
    long cursorPos = ctx->cursorOffset;
    while (c != EOF)
    {
        // If we found a non numeric value that is not an additional decimal point we escape
        if ((!IsNumeric(c)) &&
            (IsWhiteSpace(c) ||
             IsSingleCharToken(c)))
        {
            cursorPos--;
            ctx->cursorOffset = cursorPos;
            SET_TOKEN_ENUM_TYPE(t, INTEGER);
            ctx->charNumber = at;
            ungetc(c, file);
            break;
        }
        APPEND_TO_ARRAY(char, t.value, c)
        cursorPos++;
        at++;
        c = fgetc(file);
    } // Will produce an integer
    // Will determine if it is a keyword
    // Failed parse (Free string)
    if (ctx->cursorOffset != cursorPos)
    {
        FREE_ARRAY(t.value);
        fseek(file, ctx->cursorOffset, SEEK_SET);
    }
    else
    {
        APPEND_TO_ARRAY(char, t.value, '\0')
        APPEND_TO_ARRAY(Token, ctx->tokens, t);
    };
}

#endif