# A REASONABLE INTRODUCTION

## First tour

Login on [sketch](https://sketch.sh/) with your Github account and create a sketch to take note and try some code.

## Let bindings

Value declarations and assignements use `let` binding :

```reason
    let greeting : string = "hello";
    let score : int = 10;
```

Bidings are immutables which means you can't modify a value :

```reason
    let greeting : string = "hello";
    greeting = "bye"; /* Error : The value greeting is not an instance variable */
```

But you can wrap it with a ref, which is like a box whose content can change.

```reason
    let score : ref(int) = ref(10);
    score := 5;
    let five : int = score^;
    print_endline(string_of_int(five));
```

However, you may create a new binding of the same name which shadows the previous binding :

```reason
    let greeting : string = "hello";
    print_endline(greeting); /* print hello */
    let greeting : string = "bye";
    print_endline(greeting); /* print bye */
```

Unbinds function calls are implicit let bindings to `unit` type (similar to Void in some langs) which may be represent by `_` identifier.

```reason
    let _ : unit = print_endline(greeting);
```

Functions are declared with an arrow and return the expression.

```reason
    let add : (int, int) => int = (x, y) => x + y;
    add(1, 3); /* 4 */
```

## Types

The type system is completely "sound". This means that, as long as your code compiles fine, every type guarantees that it's not lying about itself. In a conventional, best-effort type system, just because the type says it's e.g. "an integer that's never null", doesn't mean it's actually never null. In contrast, a pure Reason program has no null bugs.

#### Inferrence

ReasonML types can be inferred. The type system deduces the types for you even if you don't manually write them down.

```reason
    let twelve = 12; /* let twelve : int */
    let addInt = (a, b) => a + b; /* let addInt : (int, int) => int */
    let addFloat = (a, b) => a +. b; /* let addInt : (float, float) => float */
    let aListOfInt = [1, 2, 5]; /* let aListOfInt : list(int) */
```

> `+` is the sum function for interger, `+.` is the sum function for float which made inferrence possible.

#### Alias types

Alias types can refer to a type by a different name. They'll be equivalent:

```reason
    type scoreType = int;
    let score : scoreType = 10;
```

#### Parameterized types

Types may be parameterized, similar to generic in other languages. Type Parameters always start by `'`:

````reason
    type coordinate('a) = ('a, 'a, 'a);
    type intCoordinate = coordinate(int);
    let int3DPoint: intCoordinate = (1, 2, 10);
    type floatCoordinate = coordinate(float);
    let float3DPoint: floatCoordinate = (1., 2., 10.);

    type transfert('a, 'b) = coordinate('a) => coordinate('b);
    type intToFloatTransfert = transfert(int, float);
    let intToFloatCoodinate: intToFloatTransfert = ((x, y, z)) => (float_of_int(x), float_of_int(y), float_of_int(z));
```

Reason provides two list primitives : `list('a)` and `array('a)`.

```reason
    /* Lists are homogeneous, immutable, fast at prepending items */
    let heroes : list(string) = ["Geralt", "Dandelion", "Triss"];
    let moreHeroes = ["Yennefer", ...heroes]; /* spread operator like javascript */

    /* Arrays are homogeneous, mutable, fast at random access & updates */
    let heroesArray : array(string) = [|"Geralt", "Dandelion", "Triss"|];
    let geralt : string = heroesArray[0];
    heroesArray[0] = "Yennefer";
````

#### Product types

```reason
/* Tuples and n-uples */
    let nameAndHeart: (string, int) = ("Negan", 10);
    type coord3d = (int, int, int);
    let warehouseCoord : coord3d = (1, 4, 18);

    /* Records */
    type hero = {
    heart: int,
    name: string
    };
    let geralt = {
        heart : 10,
        name : "Geralt"
    };
    let name = "Triss";
    let triss = {
        name, /* punning like javascript */
        heart : 8
    };
```

#### Variant types

Variant types are also named Sum types or discriminated union.

```reason
    type lifeVariant =
      | Alive
      | Dead
      | Mutant;

    let howAreYou = Alive;
```

Alive, Dead and Mutant are called "constructors" (or "tag"). The `|` bar separates each constructor. A variant's constructors need to be capitalized and may be parameterized.

Variant comes with one of the most important features of Reason : **pattern matching**

```reason
    let message =
    switch (howAreYou) {
    | Zombi => "Aaaaaarg !"
    | Alive => "Great !"
    | Dead => "!!!"
    };
    /* message is "Great!" */
```

## Functions

ReasonML function are auto-curried. The multiply function is nothing but syntactic sugar for multiplyDesugar :

```reason
    let multiply = (x, y) => x * y;
    let multiplyDesugar = (x) => (y) => x * y;
```

So Reason functions can automatically be partially called.

```reason
    let multiply2 = multiply(2); /* let multiply2: int => int */
    let multiply5 = multiply(5); /* let multiply5: int => int */
    let ten = multiply5(2);
```

Another great feature is the pipe opertator `|>` which can compose function by piping them

```reason
    let twenty = multiply2(5) |> multiply2;
```

It works like Unix pipe operator `|` which means the left value is pass as the rightest paramter to the right expression.

```reason
    let compose = (f, g, x) => g(f(x));
    /* === */
    let compose2 = (f, g, x) => f(x) |> g;

    let foo = (f,g) => compose(f,g, 5);
    /* === */
    let bar = (f,g) => 5 |> compose(f,g);

    /* parenthesis delimit expression */
    let thirty = 3 |> (multiply5 |> compose(multiply2));
```

## Time for kata

Now you have all you need to understand the tennis kata.
[Start the kata](./kata.md)

## Going Further

_Not mandatory for this trainning_

You can read my blog post if your want to know [more on types](https://oteku.github.io/reasonml-adt/) [FR]
