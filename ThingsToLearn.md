# Things to learn about

- Important variables and data structures:
  - `ParserContext` : is a struct that contains the parsing rules (more on that and the file mode later), the output tokens, and the cursor, column and line positions.
  - `ParsingCondition`:  a pointer to a function that does one character matching (*i.e. it takes in one character as a parameter and returns a boolean value*)
  - `ParsingFunction` : is a union struct that stores the parsing function of a rule. It can either be interpreted as a `StringParsingFunction` or a `FileParsingFunction`. The main difference between the two is that one tries to parse a string so it takes in a string as a paramter, while the other takes a `FILE` pointer.
    - **IMPORTANT NOTICE**: The parsing functions are resposible to keep the various positional variables in the context updated (cursor offset, character number, line number). The reason behind this is because Tok-A detects failed parsing by seeing if the file position (from `ftell`) is the same value as the one in the context.
    - **IMPORTANT NOTICE**: When detecting a parsing success (the context's `cursorOffset` variable has not changed since before the parsing attempt) then the Tok-A engine will fetch the next character and feed it to the engine reiterating over all of the parsing rules. On a parsing failure, all the other rules will be tested for success and anything that can't be parsed by any of the rules will be ignored.
  - `ParsingRule` : is a struct containing storing the parsing condition function, the parsing function, the rule name and its non-unique id.
- **VERY IMPORTANT NOTICE**: the order of the parsing rules changes how the file will be parsed as the engine prioritizes a successfully parsed token over a maximally parsed token. Optimally ordering the rules can be generally described as adding the rules with the lowest chance of success (format matching rules and white space eating) first then adding the more probable parsing rules (matching a single character, or parsing an identifier)
  - The example given of a C parser provides a really good showcase on one way to use the library and what kind of things you need to do while parsing. Feel free to use the parsing functions from that example (and any other example I make in the future) to epedite your parser development process
- Usefule macros
  - Generic array description for macros to work (variable names have to match too)

```c
struct{
    type* arr;
    size_t size,capacity;
}
```

  - `INIT_ARRAY(TYPE,ARR,CAPACITY)`: initializes a generic array to a specific capacity
  - `APPEND_TO_ARRAY(TYPE,ARR,VALUE)`: adds value to the end of an array (The arrays auto expand)
  - `ARRAY_SHRINK(TYPE,ARR)`: shrinks array's capacity to match the actual size
  - `FREE_ARRAY(ARR)`: frees all the resources used by an array