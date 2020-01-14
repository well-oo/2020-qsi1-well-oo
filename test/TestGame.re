open Framework;
open TennisKata.Game;

describe("Test Game transitions", ({test}) => {
  test("Given deuce when PlayerOne wins then score is correct", ({expect}) => {
    expect.equal(scoreWhenDeuce(PlayerOne), Advantage(PlayerOne))
  });
  test("Given deuce when PlayerTwo wins then score is correct", ({expect}) => {
    expect.equal(scoreWhenDeuce(PlayerTwo), Advantage(PlayerTwo))
  });
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
  test("Given advantage when the other player wins then score is Deuce",
  ({expect}) => {
    let advantagedPlayer = PlayerOne;
    let winner = other(advantagedPlayer);
    expect.equal(
      scoreWhenAdvantage(advantagedPlayer, winner),
      Deuce,
    );
  });
  test(
    "Given player: 40 when wins then score is Game for this player",
    ({expect}) => {
    let fortyThirty = {player: PlayerOne, otherPlayerPoint: Thirty};
    expect.equal(
      scoreWhenForty(fortyThirty, fortyThirty.player),
      Game(fortyThirty.player),
    );
  });
  test(
    "Given player: 40 | other : 30 when other wins then score is Deuce",
    ({expect}) => {
    let fortyThirty = {player: PlayerOne, otherPlayerPoint: Thirty};
    expect.equal(
      scoreWhenForty(fortyThirty, other(fortyThirty.player)),
      Deuce
    );
  });
  test("Given player: 40 | other : 0 when other wins then score is fortyFifteen",
  ({expect}) => {
    let fortyLove = {player: PlayerOne, otherPlayerPoint: Love};
    let fortyFifteen = {player: PlayerOne, otherPlayerPoint: Fifteen};
    expect.equal(
      scoreWhenForty(fortyLove, other(fortyLove.player)),
      Forty(fortyFifteen)
    );
  });
});

describe("Test for points", ({test}) => {
  test("Given player: 15 | other : 15 when player wins then score is 30/15",
    ({expect}) => {
    let fifteenFifteen = {playerOne: Fifteen, playerTwo: Fifteen};
    let thirtyFifteen = {playerOne: Thirty, playerTwo: Fifteen};
    expect.equal(
      scoreWhenPoints(fifteenFifteen, PlayerOne),
      Points(thirtyFifteen)
    );
  });
  test("Given player: 0 | other : 15 when other wins then score is 0/30",
  ({expect}) => {
    let loveFifteen = {playerOne: Love, playerTwo: Fifteen};
    let loveThirty = {playerOne: Love, playerTwo: Thirty};
    expect.equal(
      scoreWhenPoints(loveFifteen, PlayerTwo),
      Points(loveThirty)
    );
  });
  test("Given player: 30 | other : 15 when player wins then score is 40/15",
    ({expect}) => {
    let thirtyFifteen = {playerOne: Thirty, playerTwo: Fifteen};
    let fortyData = {player: PlayerOne, otherPlayerPoint: Fifteen};
    expect.equal(
      scoreWhenPoints(thirtyFifteen, PlayerOne),
      Forty(fortyData)
    );
  });
});


describe("Test for string", ({test}) => {
  test("Test string of player one",
  ({expect}) => {
    expect.equal(string_of_player(PlayerOne), "I'm PlayerOne");
  });
  test("Test string of player two",
  ({expect}) => {
    expect.equal(string_of_player(PlayerTwo), "I'm PlayerTwo");
  });
  test("Test string of point Love",
  ({expect}) => {
    expect.equal(string_of_point(Love), "Love");
  });
  test("Test string of point Fifteen",
  ({expect}) => {
    expect.equal(string_of_point(Fifteen), "Fifteen points");
  });
  test("Test string of point Thirty",
  ({expect}) => {
    expect.equal(string_of_point(Thirty), "Thirty points");
  });
  test("Test string of score when Score is 0/0",
  ({expect}) => {
    let loveLove = {playerOne: Love, playerTwo: Love};
    expect.equal(string_of_score(Points(loveLove)), "Player one has Love and player two has Love");
  });
  test("Test string of score when Score is 0/15",
  ({expect}) => {
    let loveFifteen = {playerOne: Love, playerTwo: Fifteen};
    expect.equal(string_of_score(Points(loveFifteen)), "Player one has Love and player two has Fifteen points");
  });
  test("Test string of score when Score is 0/30",
  ({expect}) => {
    let loveThirty = {playerOne: Love, playerTwo: Thirty};
    expect.equal(string_of_score(Points(loveThirty)), "Player one has Love and player two has Thirty points");
  });
  test("Test string of score when Score is 15/15",
  ({expect}) => {
    let fifteenFifteen = {playerOne: Fifteen, playerTwo: Fifteen};
    expect.equal(string_of_score(Points(fifteenFifteen)), "Player one has Fifteen points and player two has Fifteen points");
  });
  test("Test string of score when Score is 15/30",
  ({expect}) => {
    let fifteenThirty = {playerOne: Fifteen, playerTwo: Thirty};
    expect.equal(string_of_score(Points(fifteenThirty)), "Player one has Fifteen points and player two has Thirty points");
  });
  test("Test string of score when Score is 30/30",
  ({expect}) => {
    let thirtyThirty = {playerOne: Thirty, playerTwo: Thirty};
    expect.equal(string_of_score(Points(thirtyThirty)), "Player one has Thirty points and player two has Thirty points");
  });
  test("Test string of score when Score is 40/30",
  ({expect}) => {
    let fortyThirty = {player: PlayerOne, otherPlayerPoint: Thirty};
    expect.equal(string_of_score(Forty(fortyThirty)), "I'm PlayerOne with 40 points, I'm playerTwo with Thirty points");
  });
  test("Test string of score when Score is 15/40",
  ({expect}) => {
    let fifteenForty = {player: PlayerTwo, otherPlayerPoint: Fifteen};
    expect.equal(string_of_score(Forty(fifteenForty)), "I'm PlayerTwo with 40 points, I'm playerOne with Fifteen points");
  });
  test("Test string of score when Deuce",
  ({expect}) => {
    expect.equal(string_of_score(Deuce), "Deuce between the two players!");
  });
  test("Test string of score when Advantage player one",
  ({expect}) => {
    expect.equal(string_of_score(Advantage(PlayerOne)), "I'm PlayerOne and I've the advantage");
  });
  test("Test string of score when Advantage player two",
  ({expect}) => {
    expect.equal(string_of_score(Advantage(PlayerTwo)), "I'm PlayerTwo and I've the advantage");
  });
  test("Test string of score when Game player one",
  ({expect}) => {
    expect.equal(string_of_score(Game(PlayerOne)), "I'm PlayerOne and I win the game");
  });
  test("Test string of score when Game player two",
  ({expect}) => {
    expect.equal(string_of_score(Game(PlayerTwo)), "I'm PlayerTwo and I win the game");
  });
});

