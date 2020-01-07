# TENNIS KATA

## Goals of this training

- How a statically typed language with functional programming pattern could help ?
- How Type Driven Design and Test Driven Development could secure developments ?
- Discovery of ReasonML

## Meet ReasonML

ReasonML was designed by [Jordan Walke](https://twitter.com/jordwalke), engineer at Facebook and the creator of [reactjs](https://reactjs.org/).

The promise :

> Reason lets you write simple, fast and quality type safe code while leveraging both the JavaScript & OCaml ecosystems.

[ReasonML](https://reasonml.github.io/) is not a new language; it's a new syntax and toolchain powered by the battle-tested language, OCaml. Reason gives OCaml a familiar syntax geared toward JavaScript programmers. Reason may compile to native code, OCaml byte code or javascript.
While young it is rising fast and is already used by companies like Facebook, o1labs, ahrefs, SAP, Tezos, Jane Street or Bloomberg.

## Plan

- [Introduction to ReasonML](./doc/reason.md)
- [The tennis kata](./doc/kata.md)

**Contains the following libraries and executables:**

```
tennis_kata@0.0.0
│
├─test/
│   name:    TestTennisKata.exe
│   main:    TestTennisKata
│   require: tennis_kata.lib
│
├─lib/
│   library name: tennis_kata.lib
│   namespace:    TennisKata
│   require:
│
└─bin/
    name:    TennisKataApp.exe
    main:    TennisKataApp
    require: tennis_kata.lib
```
