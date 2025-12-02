# Tok-A

## What is Tok-A

Tok-A is the more flexible general purpose parsing engine. It can be seen as the more hands on version of my previous tokenizer **[Toke](https://github.com/AhmedBineuro/Toke)**. Tok-A provides a super basic parsing system and interface used to tokenize any file, any way you like it!

## What can you do with *Tok-A*

With Tok-A, you finally be able to define your own tokenizing functions and conditions. Instead of just defining the tokens like before, you now define a *parsing condition* (the function to evaluate if it is good to parse) and the *parsing function* (the function that consumes the characters and evaluates the formatting). This tokenizer comes a handful of useful macros to speed up development and prevent redundant code.

## When to use **[Toke](https://github.com/AhmedBineuro/Toke)** Vs Tok-A

If you want simple parsing with disregard to white space and new lines then **[Toke](https://github.com/AhmedBineuro/Toke)** will speed up your development.
On the other hand, if you want to have total control over the parsing priorities and behaviours then **Toka-A** is your engine of choice.

## Documentation

A rudementary version can be found [here](./ThingsToLearn.md).
