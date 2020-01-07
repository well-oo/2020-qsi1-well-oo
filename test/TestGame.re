open Framework;
open TennisKata.Game;

describe("Test Game transitions", ({test}) => {
  test("Given PlayerOne then PlayerTwo is different", ({expect}) => {
    expect.notEqual(PlayerOne, PlayerTwo)
  })
});