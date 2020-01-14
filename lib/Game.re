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

type fortyData = {
  player: player, /* The player who have forty points */
  otherPlayerPoint: point
};

let fd : fortyData = {player: PlayerOne, otherPlayerPoint: Love};

type score =
| Points(pointsData)
| Forty(fortyData)
| Deuce
| Advantage(player)
| Game(player);

let startScore : score = Points({playerOne: Love, playerTwo: Love});

let anotherScore : score = Forty({player: PlayerTwo, otherPlayerPoint:Thirty });

let scoreWhenDeuce: player => score = winner => Advantage(winner);

let scoreWhenAdvantage: (player, player) => score =
  (advantagedPlayer, winner) =>
    advantagedPlayer == winner ? Game(winner) : Deuce;

  /* This time we infer that the function type is (player) => player */
let other = player =>
switch player {
| PlayerOne => PlayerTwo
| PlayerTwo => PlayerOne
};

/* We add a tool function to increment point */
let incrementPoint: point => option(point) =
  point =>
    switch point {
    | Love => Some(Fifteen)
    | Fifteen => Some(Thirty)
    | Thirty => None
    };

let scoreWhenForty = (current, winner) =>
current.player == winner ?
Game(winner) :
(
    switch (incrementPoint(current.otherPlayerPoint)) {
    | Some(p) => Forty({...current, otherPlayerPoint: p})
    | None => Deuce
  }
);

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

let scoreWhenGame = winner => Game(winner);  

let score = (current, winner) =>
switch current {
| Points(p) => scoreWhenPoints(p, winner)
| Forty(f) => scoreWhenForty(f, winner)
| Deuce => scoreWhenDeuce(winner)
| Advantage(a) => scoreWhenAdvantage(a, winner)
| Game(g) => scoreWhenGame(g)
};

let newGame = Points({playerOne: Love, playerTwo: Love});

let string_of_player : (player) => string = player =>
switch player {
| PlayerOne => "I'm PlayerOne"
| PlayerTwo => "I'm PlayerTwo"
};

let string_of_point : (point) => string = point =>
switch point {
  | Love => "Love"
  | Fifteen => "Fifteen points"
  | Thirty => "Thirty points";

};

 
let string_of_score : (score) => string = score =>
switch score {
| Points(pointsData) => "Player one has " ++ string_of_point(pointsData.playerOne) ++ " and player two has " ++  string_of_point(pointsData.playerTwo)
| Forty(fortyData) => switch fortyData.player {
  | PlayerOne=> string_of_player(PlayerOne) ++ " with 40 points, " ++ "I'm playerTwo with " ++ string_of_point(fortyData.otherPlayerPoint);
  | PlayerTwo=> string_of_player(PlayerTwo) ++ " with 40 points, " ++ "I'm playerOne with " ++ string_of_point(fortyData.otherPlayerPoint);
}
| Deuce => "Deuce between the two players!"
| Advantage(player) => string_of_player(player) ++ " and I've the advantage"
| Game(player) =>string_of_player(player) ++ " and I win the game" ;
};