# Tennis Kata

_This Kata is an adaptation to ReasonML of [Mark Seeman's serie of articles about property-based testing](http://blog.ploeh.dk/2016/02/10/types-properties-software/). Most of his article's exemples are un F#. I highly recommand the reading of his blog if you're interested by functional programming._

## About type systems

[Hindley–Milner type systems](https://en.wikipedia.org/wiki/Hindley%E2%80%93Milner_type_system) implemented in ML languages such as ReasonML, OCaml, Haskell or F#, offers more safety from static typing alone.

With the algebraic data types available in ReasonML, you can design your types so that illegal states are unrepresentable. You can see this as "free" tests for your application.

We will illustrate it by the Tennis Kata

## Draw your racket

To develop you need a Linux, macOs or Win10 + WSL ... older win are not suitable for a developer in 2020.

While it should be your first time with ReasonML you will need some time to [setup your computer](./setup.md).

#### Code

Library code must be in [lib/Game.re](../lib/Game.re)

#### Developing

```
npm install -g esy
git clone <this-repo>
esy install
esy build
```

#### Running Binary

After building the project, you can run the main binary that is produced.

```
esy x TennisKataApp.exe
```

#### Running Tests

```
# Runs the "test" command in `package.json`.
esy test
```

## Tennis

A tennis match consists of multiple sets that again are played as several games, in the kata, you only have to implement the scoring system for a single game:

- Each player can have either of these points in one game: Love, 15, 30, 40.
- If you have 40 and you win the ball, you win the game. There are, however, special rules.
- If both have 40, the players are deuce.
- If the game is in deuce, the winner of a ball will have advantage and game ball.
- If the player with advantage wins the ball, (s)he wins the game.
- If the player without advantage wins, they are back at deuce.

This problem is easy enough that it's fun to play with, but difficult enough that it's fun to play with.
<br/>(∩ ｀-´)⊃━ ☆ .\*･｡ﾟ

### Players

In tennis, there are two players, which we can easily model with a discriminated union (a variant) :

```reason
type player =
  | PlayerOne
  | PlayerTwo;
```

### Points

#### Naive point attempt with a type alias

```reason
type point = int;
```

This easily enables you to model some of the legal point values:

```reason
let p : point = 15;
let p : point = 30;
```

It looks good so far, but how do you model love? It's not really an integer. Still, both players start with love, so it's intuitive to try to model love as 0 ...It's a hack, but it works. But your illegal values are not unrepresentable :

```reason
let p : point = 1000;
let p : point = -20;
```

For a 32-bit integer, this means that we have four legal representations (0, 15, 30, 40), and 4,294,967,291 illegal representations of a tennis point. Clearly this doesn't meet the goal of making illegal states unrepresentable. ლ(ಠ_ಠლ)

#### Second point attempt with a variant

You may see that love, 15, 30, and 40 aren't numbers, but rather labels. No arithmetic is performed on them. It's easy to constrain the domain of points with a variant

```reason
type point =
  | Love
  | Fifteen
  | Thirty
  | Forty;
```

A Point value isn't a score. A score is a representation of a state in the game, with a point to each player. You can model this with a record:

```reason
type pointsData = {
  playerOne: point,
  playerTwo: point
};
```

You can experiment with this type:

```reason
let s1 = {playerOne: Love, playerTwo: Love};
let s2 = {playerOne: Fifteen, playerTwo: Love};
let s3 = {playerOne: Thirty, playerTwo: Love};
```

What happens if players are evenly matched?

```reason
let even = {playerOne: Forty, playerTwo: Forty};
```

_Forty-forty_ isn't a valid tennis score; it's called _deuce_.

If you're into [Domain-Driven Design](https://www.infoq.com/minibooks/domain-driven-design-quickly), you prefer using the ubiquitous language of the domain. When the tennis domain language says that it's not called forty-forty, but deuce, the code should reflect that.

#### Final attempt at a point type

The love-love, fifteen-love, etc. values that you can represent with the above PointsData type are all valid. Only when you approach the boundary value of forty do problems appear. A solution is to remove the offending Forty case from Point. (⊙_☉)

At this point, it may be helpful to recap what we have :

```reason
type player =
  | PlayerOne
  | PlayerTwo;

type point =
  | Love
  | Fifteen
  | Thirty;

type pointsData = {
  playerOne: point,
  playerTwo: point
};
```

While this enables you to keep track of the score when both players have less than forty points, the following phases of a game still remain:

- One of the players have forty points.
- Deuce.
- Advantage to one of the players.
- One of the players won the game.

You can design the first of these with another record type:

```reason
type fortyData = {
  player: player, /* The player who have forty points */
  otherPlayerPoint: point
};
```

For instance, this value indicates that PlayerOne has forty points, and PlayerTwo has Love :

```reason
let fd : fortyData = {player: PlayerOne, otherPlayerPoint: Love};
```

This is a legal score. Other values of this type exist, but none of them are illegal.

### Score

Now you have two distinct types, PointsData and FortyData, that keep track of the score at two different phases of a tennis game. You still need to model the remaining three phases, and somehow turn all of these into a single type. This is an undertaking that can be surprisingly complicated in C# or Java, but is trivial to do with a variant:

```reason
type score =
| Points(pointsData)
| Forty(fortyData)
| Deuce
| Advantage(player)
| Game(player);
```

As an example, the game starts with both players at love:

```reason
let startScore : score = Points({playerOne: Love, playerTwo: Love});
```

PlayerOne has forty points, and PlayerTwo has thirty points, you can create this value:

```reason
let anotherScore : score = Forty({player: PlayerTwo, otherPlayerPoint:Thirty });
```

This model of the tennis score system enables you to express all legal values, while making illegal states unrepresentable.

```reason
/* All exemple above don't work */
let impossibleScore1 : score = Points({playerOne: Seven, playerTwo: Eleven});
let impossibleScore2 : score = Points({playerOne: Forty, playerTwo: Forty});
let impossibleScore3 : score = Forty({player: PlayerTwo, otherPlayerPoint:Forty });
```

These types govern what can be stated in the domain, but they don't provide any rules for how values can transition from one state to another.

## Transition

While the types defined in the previously make illegal states unrepresentable, they don't enforce any rules about how to transition from one state into another. A state transition should be a function that takes a current Score and the winner of a ball and returns a new Score. More formally, it should have the type `(Score, Player) => Score`.

We will apply Test-Driven Development following the Red/Green/Refactor cycle, using [Rely](https://reason-native.com/docs/rely/quickstart).

We will define a smaller function for each case, and test the properties of each of these functions.

### Deuce property

The case of deuce is special, because there's no further data associated with the state; when the score is deuce, it's deuce. This means that when calling scoreWhenDeuce, you don't have to supply the current state of the game.

In [TestGame.re](../test/TestGame.re) :

```reason
open Framework;
open TennisKata.Game;

describe("Test Game transitions", ({test}) => {
  test("Given deuce when PlayerOne wins then score is correct", ({expect}) => {
    expect.equal(scoreWhenDeuce(PlayerOne), Advantage(PlayerOne))
  });
});
```

NB : `open` is the keyword to import a [module](https://reasonml.github.io/docs/en/module.html) in a scope

The test fails because we don't have implement the function `scoreWhenDeuce` yet :

```reason
let scoreWhenDeuce: player => score = _ => Advantage(PlayerOne);
```

Just in case, we will test the opposite

```reason

    test("Given deuce when PlayerTwo wins then score is correct", ({expect}) => {
    expect.equal(scoreWhenDeuce(PlayerTwo), Advantage(PlayerTwo))
  });
```

Ooops we need to fix our function

```reason
let scoreWhenDeuce: player => score = winner => Advantage(winner);
```

Now the test pass ! (•̀ᴗ•́)و

### Winning the game

#### Advantage

When one of the players have the advantage in tennis, the result can go one of two ways: either the player with the advantage wins the ball, in which case he or she wins the game, or the other player wins, in which case the next score is deuce.

We will add a second test :

```reason
  test(
    "Given advantage when advantaged player wins then score is correct",
    ({expect}) => {
    let advantagedPlayer = PlayerOne;
    let winner = advantagedPlayer;
    expect.equal(
      scoreWhenAdvantage(advantagedPlayer, winner),
      Game(advantagedPlayer),
    );
  });
```

The test fails because we don't have implement the function `scoreWhenDeuce` yet :

```reason
let scoreWhenAdvantage: (player, player) => score =
  (advantagedPlayer, winner) => Game(winner);
```

Now the test pass ! (•̀ᴗ•́)و

Add a new test :

```reason
  test("Given advantage when the other player wins then score is Deuce",
  ({expect}) => {
    let advantagedPlayer = PlayerOne;
    let winner = other(advantagedPlayer);
    expect.equal(
      scoreWhenAdvantage(advantagedPlayer, winner),
      Deuce,
    );
  });
```

The test fails because we don't have implement the tool function `other` :

```reason
/* This time we infer that the function type is (player) => player */
let other = player =>
  switch player {
  | PlayerOne => PlayerTwo
  | PlayerTwo => PlayerOne
  };
```

The test fails again (⊙_☉)
The above implementation of scoreWhenAdvantage is obviously incorrect, because it always claims that the advantaged player wins the game, regardless of who wins the ball.

```reason
let scoreWhenAdvantage: (player, player) => score =
  (advantagedPlayer, winner) =>
    advantagedPlayer == winner ? Game(winner) : Deuce;
```

Now the test pass ! (•̀ᴗ•́)و

#### Forty

When one of the players have forty points, there are three possible outcomes of the next ball:

- If the player with forty points wins the ball, (s)he wins the game.
- If the other player has thirty points, and wins the ball, the score is deuce.
- If the other player has less than thirty points, and wins the ball, his or her points increases to the next level (from love to fifteen, or from fifteen to thirty).

The first property is the easiest :

```reason
  test(
    "Given player: 40 when wins then score is Game for this player",
    ({expect}) => {
    let fortyThirty = {player: PlayerOne, otherPlayerPoint: Thirty};
    expect.equal(
      scoreWhenForty(fortyThirty, fortyThirty.player),
      Game(fortyThirty.player),
    );
  });
```

The test fails, now add an implementation :

```reason
let scoreWhenForty = (current, winner) => Game(winner);
```

Now the test pass ! (•̀ᴗ•́)و

Add a test for the second :

```reason
  test(
    "Given player: 40 | other : 30 when other wins then score is Deuce",
    ({expect}) => {
    let fortyThirty = {player: PlayerOne, otherPlayerPoint: Thirty};
    expect.equal(
      scoreWhenForty(fortyThirty, other(fortyThirty.player)),
      Deuce
    );
  });
```

Iterate our implementation of `scoreWhenForty` to make the test pass ! (•̀ᴗ•́)و

Add a test for the third property :

```reason
  test("Given player: 40 | other : 0 when other wins then score is fortyFifteen",
  ({expect}) => {
    let fortyLove = {player: PlayerOne, otherPlayerPoint: Love};
    let fortyFifteen = {player: PlayerOne, otherPlayerPoint: Fifteen};
    expect.equal(
      scoreWhenForty(fortyLove, other(fortyLove.player)),
      Forty(fortyFifteen)
    );
  });
```

Iterate our implementation :

```reason
/* We add a tool function to increment point */
let incrementPoint: point => option(point) =
  point =>
    switch point {
    | Love => Some(Fifteen)
    | Fifteen => Some(Thirty)
    | Thirty => None
    };
```

We introduce a new variant type [option](https://reasonml.github.io/docs/en/newcomer-examples.html#using-the-option-type). It means that a variable of this time may be something or nothing (ie a nullable variable).

Now we can use pattern matching to implement our function :

```reason
let scoreWhenForty = (current, winner) =>
  current.player == winner ?
    Game(winner) :
    (
      switch (incrementPoint(current.otherPlayerPoint)) {
      | Some(p) => Forty({...current, otherPlayerPoint: p})
      | None => Deuce
      }
    );
```

Now the test pass ! (ﾉ ◕ ヮ ◕)ﾉ\*:・ﾟ ✧

#### Points

Add a test for points :

```reason
  test(
    "Given player: 15 | other : 15 when player wins then score is 30/15",
    ({expect})) => {
    let fifteenFifteen = {playerOne: Fifteen, playerTwo: Fifteen};
    let thirtyFifteen = {playerOne: Thirty, playerTwo: Fifteen};
    expect.equal(
      scoreWhenPoints(fifteenFifteen, PlayerOne),
      Points(thirtyFifteen)
    );
    expect(scoreWhenPoints(fifteenFifteen, PlayerOne))
    |> toEqual(Points(thirtyFifteen));
  });

  test("Given player: 0 | other : 15 when other wins then score is 0/30",
  ({expect})) => {
    /* YOUR TURN TO DO THE TEST */
  });
  test(
    "Given player: 30 | other : 15 when player wins then score is 40/15",
    ({expect})) => {
    /* YOUR TURN TO DO THE TEST */
  });
```

Iterate our implementation :

```reason
let pointTo = (player, point, current) =>
  switch player {
  | PlayerOne => {...current, playerOne: point}
  | PlayerTwo => {...current, playerTwo: point}
  };

let pointFor = (player, current) =>
  switch player {
  | PlayerOne => current.playerOne
  | PlayerTwo => current.playerTwo
  };

let scoreWhenPoints = (current, winner) =>
  switch (current |> pointFor(winner) |> incrementPoint) {
  | Some(np) => Points(pointTo(winner, np, current))
  | None =>
    Forty({
      player: winner,
      otherPlayerPoint: current |> pointFor(other(winner))
    })
  };
```

Now the test pass ! (ﾉ ◕ ヮ ◕)ﾉ\*:・ﾟ ✧

### Composing the general function

What you need to implement is a state transition of the type `(Score, Player ) => Score`.
<br/>What you have so far are the following functions:

- scoreWhenPoints : `(PointsData, Player) => Score`
- scoreWhenForty : `(FortyData, Player) => Score`
- scoreWhenDeuce : `(Player) => Score`
- scoreWhenAdvantage : `(Player, Player) -> Score`

You can implement the desired function by clicking the pieces together:

```reason
let score = (current, winner) =>
  switch current {
  | Points(p) => scoreWhenPoints(p, winner)
  | Forty(f) => scoreWhenForty(f, winner)
  | Deuce => scoreWhenDeuce(winner)
  | Advantage(a) => scoreWhenAdvantage(a, winner)
  | Game(g) => scoreWhenGame(g)
  };
```

You can notice we add a new function `scoreWhenGame`. It's because a pattern matching must be exaustive. We should also use `_` which mean _default_. In our case, `current` type is `score` so its value may also be `Game` but when score is Game it means a playe win the game, score will no more change. Add the implementation :

```reason
let scoreWhenGame = winner => Game(winner);
```

Finally you can initialize a new Game :

```reason
let newGame = Points({playerOne: Love, playerTwo: Love});
```

## Exercice

Develop 3 functions : `string_of_player`, `string_of_point`, `string_of_score` that return string from a data of type player, point or score.
