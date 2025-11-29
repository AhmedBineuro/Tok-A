/**
 * @file Toka.h
 * @author Ahmed Abdalla (dynamic.bineuro@gmail.com)
 * @brief A more advanced tokenization framework design to speed up complex tokenization
 * @version 2.0
 * @date 2025-11-27
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef TOKE_H
#define TOKE_H
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
////////////////////////MACROS//////////////////////
#define ENUM_STRINGIFY(ENUM) #ENUM
#define ARRAY(TYPE, TYPENAME)    \
    typedef struct               \
    {                            \
        TYPE *arr;               \
        uint64_t size, capacity; \
    } TYPENAME;

#define INIT_ARRAY(TYPE, ARRAY, CAPACITY)                       \
    {                                                           \
        if (CAPACITY > 0)                                       \
            ARRAY.arr = (TYPE *)calloc(CAPACITY, sizeof(TYPE)); \
        else                                                    \
            ARRAY.arr = NULL;                                   \
        ARRAY.size = 0;                                         \
        ARRAY.capacity = CAPACITY;                              \
    }

#define EXPAND_ARRAY(TYPE, ARRAY)                                                  \
    {                                                                              \
        if (ARRAY.capacity == 0)                                                   \
        {                                                                          \
            ARRAY.arr = (TYPE *)calloc(2, sizeof(TYPE));                           \
            ARRAY.capacity = 2;                                                    \
        }                                                                          \
        else if ((ARRAY.size == ARRAY.capacity))                                   \
        {                                                                          \
            ARRAY.capacity *= 2;                                                   \
            ARRAY.arr = (TYPE *)realloc(ARRAY.arr, sizeof(TYPE) * ARRAY.capacity); \
        }                                                                          \
    }

#define APPEND_TO_ARRAY(TYPE, ARRAY, VALUE)                          \
    {                                                                \
        if ((ARRAY.capacity == 0) || (ARRAY.size == ARRAY.capacity)) \
            EXPAND_ARRAY(TYPE, ARRAY)                                \
        ARRAY.arr[ARRAY.size] = VALUE;                               \
        ARRAY.size += 1;                                             \
    }
#define IS_EQUAL_ARRAY(ARRAY1, ARRAY2, BOOL)                     \
    {                                                            \
        if ((ARRAY1.size == ARRAY2.size))                        \
        {                                                        \
            BOOL = true;                                         \
            for (int i = 0; i < ARRAY1.size; i++)                \
                BOOL = BOOL && (ARRAY1.arr[i] == ARRAY1.arr[i]); \
            if (!BOOL)                                           \
                break;                                           \
        }                                                        \
        else                                                     \
            BOOL = false;                                        \
    }
#define FREE_ARRAY(ARRAY)      \
    {                          \
        if ((ARRAY.size != 0)) \
            free(ARRAY.arr);   \
        ARRAY.size = 0;        \
        ARRAY.capacity = 0;    \
        ARRAY.arr = NULL;      \
    }
#define SET_TOKEN_ENUM_TYPE(TOKEN, ENUM)   \
    {                                      \
        TOKEN.typeId = ENUM;               \
        TOKEN.type = ENUM_STRINGIFY(ENUM); \
    }
////////////////////////MACROS END//////////////////////

typedef struct _ParserCTX ParserContext;
typedef struct _Token Token;

typedef bool (*ParsingCondition)(char c);
typedef void (*StringParsingFunction)(ParserContext *, char, char *);
typedef void (*FileParsingFunction)(ParserContext *, char, FILE *);
typedef union _ParsingFunction
{
    FileParsingFunction fileFunction;     // If yes parse it as ____
    StringParsingFunction stringFunction; // If yes parse it as ____
} ParsingFunction;

/** @brief A parsing rule determines the initial condition of parsing a specific token
    and how to parse it
    @note Each Parsing function will be responsible to restore the state of the object being
    parsed upon failure to parse
    @note The decision on whether to pick a file rule or string rule is based on the context fileMode flag
*/
typedef struct
{
    ParsingCondition condition; // Can I parse this as ___
    ParsingFunction func;
    int id;
    char *name;
} ParsingRule;

// Despite the macro's ability to create the type, write it for
// the autocomplete features
/*ARRAY STRUCTS*/
typedef struct
{
    char *arr;
    uint64_t size, capacity;
} String;

typedef struct
{
    Token *arr;
    uint64_t size, capacity;
} TokenArray;

typedef struct
{
    ParsingRule *arr;
    uint64_t size, capacity;
} ParsingRuleArray;
/*END OF ARRAY STRUCTS*/

/*CORE STRUCTS*/
struct _Token
{
    char *type;
    int typeId;
    long at, line;
    String value;
};
struct _ParserCTX
{
    uint64_t lineNumber, charNumber;
    long cursorOffset; // The value returned by ftell (the 'at' position)
    TokenArray tokens;
    bool fileMode;
    ParsingRuleArray rules;
};
/*END OF CORE STRUCTS*/

ParserContext CreateParserContext(bool parsingFiles);
void AddParseRule(ParserContext *ctx, char *name,
                  ParsingCondition conditionFunc, ParsingFunction parseFunc);
// Parse it as a file if fileMode is true and as a string otherwise
void Parse(ParserContext *ctx, char *src);
void ResetContext(bool resetRules, ParserContext *ctx);
void FreeParserContext(ParserContext *ctx);

//////////////FUNCTION IMPLEMENTATIONS///////////////////
ParserContext CreateParserContext(bool fileMode)
{
    ParserContext ctx;
    ctx.fileMode = fileMode;
    INIT_ARRAY(ParsingRule, ctx.rules, 0);
    INIT_ARRAY(Token, ctx.tokens, 0);
    ctx.cursorOffset = 0;
    ctx.lineNumber = 1;
    ctx.charNumber = 1;
    return ctx;
}

inline void AddParseRule(ParserContext *ctx, char *name,
                         ParsingCondition conditionFunc, ParsingFunction parseFunc)
{
    ParsingRule rule;
    rule.name = name;
    rule.id = ctx->rules.size + 1;
    rule.condition = conditionFunc;
    rule.func = parseFunc;
    APPEND_TO_ARRAY(ParsingRule, ctx->rules, rule)
}

/** @todo Condense this to eliminate code duplication */
inline void Parse(ParserContext *ctx, char *src)
{
    if (ctx->fileMode)
    {
        FILE *file = fopen(src, "r");
        char c = fgetc(file);
        while (c != EOF)
        {
            for (int i = 0; i < ctx->rules.size; i++)
            {
                if (ctx->rules.arr[i].condition(c))
                {
                    long cursorPos = ftell(file);
                    ctx->rules.arr[i].func.fileFunction(ctx, c, file);
                    if (ctx->cursorOffset != cursorPos)
                    {
                        break;
                    }
                }
            }
            c = fgetc(file);
        }
        fclose(file);
    }
    else
    {
        while (src[ctx->cursorOffset] != '\0')
        {
            for (int i = 0; i < ctx->rules.size; i++)
            {
                char c = src[ctx->cursorOffset];
                if (ctx->rules.arr[i].condition(c))
                {
                    long cursorPos = ctx->cursorOffset;
                    ctx->rules.arr[i].func.stringFunction(ctx, c, src);
                    // If parsing succeeded add token, else continue testing
                    if (ctx->cursorOffset != cursorPos)
                    {
                        break;
                    }
                }
            }
        }
    }
    ctx->lineNumber = 1;
    ctx->charNumber = 1;
}

inline void ResetContext(bool resetRules, ParserContext *ctx)
{
    for (int i = 0; i < ctx->tokens.size; i++)
    {
        FREE_ARRAY(ctx->tokens.arr[i].value)
    }
    FREE_ARRAY(ctx->tokens)
    INIT_ARRAY(Token, ctx->tokens, 0);
    if (resetRules)
    {
        FREE_ARRAY(ctx->rules)
        INIT_ARRAY(ParsingRule, ctx->rules, 0);
    }
    ctx->cursorOffset = 0;
    ctx->lineNumber = 1;
    ctx->charNumber = 1;
}

inline void FreeParserContext(ParserContext *ctx)
{
    FREE_ARRAY(ctx->rules)
    for (int i = 0; i < ctx->tokens.size; i++)
    {
        FREE_ARRAY(ctx->tokens.arr[i].value)
    }
    FREE_ARRAY(ctx->tokens)
}

/////////////////////////////////////////////////////////

/**
 * @todo
 * - [X] Come up with token definition
 * - [X] Come up with parser state definition
 *      - Contains:
 *          - [X] Line number
 *          - [X] Character number
 *          - [X] Token list
 *          - [X] Rule list
 * - [X] Come up with parsing engine
 * - [X] Come up with parsing engine context
 * - [ ] Come up with testing framework
 */

#endif