// ronamsalem4@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../game/Player.hpp"
#include "../roles/Governor.hpp"
#include "../roles/Spy.hpp"
#include "../roles/Baron.hpp"
#include "../roles/General.hpp"
#include "../roles/Judge.hpp"
#include "../roles/Merchant.hpp"
#include "../game/Game.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;
using namespace coup;

/**
 * This test checks the situation where there is only one player in the game.
 */
TEST_CASE("Start a game with only one player")
{
    Game game;
    Governor governor(game, "Solo");

    CHECK_THROWS(governor.tax());

    CHECK_THROWS(game.winner());
}
/**
 * This test checks the situation where there are more than six players in the game.
 */

TEST_CASE("Start a game with more than 6 players")
{
    Game game{};
    Governor governor(game, "Ron");
    Spy spy(game, "Or");
    Baron baron(game, "Shir");
    General general(game, "Yoram");
    Judge judge(game, "Tali");
    Merchant merchant(game, "Dor");

    // Trying to add a 7th player should throw an exception
    CHECK_THROWS(Spy(game, "Boba"));
}

/**
 * This test checks the case where a player plays not on his turn.
 *According to the instructions, the order of the players is determined by who joined first
 *Therefore, the Governor's turn is first (Ron) and then the Spy's turn (Or).
 *The test checks what happens if the General (Yorem) plays instead of the Spy (Or).
 */

TEST_CASE("A player who plays out of turn")
{
    Game game{};
    Governor governor(game, "Ron");
    Spy spy(game, "Or");
    Baron baron(game, "Shir");
    General general(game, "Yoram");
    Judge judge(game, "Tali");
    Merchant merchant(game, "Dor");
    vector<Player *> players2 = {&governor, &spy, &baron, &general, &judge, &merchant};
    governor.tax();
}

/**
 * This test includes a check of the Tax operation.
 * In addition a check that the governor performs when it blocks this operation.
 */
TEST_CASE("Tax and Governor blocks tax action")
{
    Game game{};
    Governor governor(game, "Ron");
    Spy spy(game, "Or");
    Baron baron(game, "Shir");
    General general(game, "Yoram");
    Judge judge(game, "Tali");
    Merchant merchant(game, "Dor");
    vector<Player *> players2 = {&governor, &spy, &baron, &general, &judge, &merchant};
    governor.tax();
    int expected = 3;
    int real = governor.coins();
    CHECK(expected == real);
    spy.tax();
    governor.undo(spy);
    int expected1 = 0;
    int real1 = spy.coins();
    CHECK(expected1 == real1);
    baron.tax();
    int expected2 = 2;
    int real2 = baron.coins();
    CHECK(expected2 == real2);
}

/**
 * This test tests the role of the spy, i.e. the ability to see the amount of coins another player
has and prevent him from using the arrest action on his next turn.
 Keep in mind that this action does not cost coins and does not count as a turn.
 */
TEST_CASE("The role of the spy")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Baron baron(game, "Alice");
    General general(game, "Bob");
    Judge judge(game, "Avner");
    Merchant merchant(game, "Dalia");
    vector<Player *> players2 = {&governor, &spy, &baron, &general, &judge, &merchant};
    governor.tax();
    spy.watchCoins(baron);
    spy.tax();
    CHECK_THROWS(baron.arrest(general));
    baron.tax();
    general.gather();
    judge.gather();
    merchant.gather();
    governor.gather();
    spy.gather();
    baron.arrest(spy);
    CHECK(governor.coins() == 4);
    CHECK(spy.coins() == 2);
    CHECK(baron.coins() == 3);
    CHECK(general.coins() == 1);
    CHECK(judge.coins() == 1);
    CHECK(merchant.coins() == 1);
}

/**
 * This test checks the Baron's behavior when he is attacked with a sanction.
 *  He should receive one coin in compensation.
 * Checking the invest action belonging to Baron.
 */
TEST_CASE("The role of the Baron- One coin compensation due to sanction + invest action ")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Baron baron(game, "Alice");
    General general(game, "Bob");
    Judge judge(game, "Avner");
    Merchant merchant(game, "Dalia");
    vector<Player *> players2 = {&governor, &spy, &baron, &general, &judge, &merchant};
    governor.tax();
    spy.watchCoins(baron);
    spy.tax();
    CHECK_THROWS(baron.arrest(general));
    baron.tax();
    general.gather();
    judge.gather();
    merchant.gather();
    governor.gather();
    spy.gather();
    baron.arrest(spy);
    general.tax();
    judge.tax();
    merchant.tax();
    governor.sanction(baron, "tax");
    spy.tax();
    baron.invest();
    CHECK(governor.coins() == 1);
    CHECK(spy.coins() == 4);
    CHECK(baron.coins() == 7);
    CHECK(general.coins() == 3);
    CHECK(judge.coins() == 3);
    CHECK(merchant.coins() == 3);
}

/**
 * This test checks the general's ability to block another player's coup.
 * That is, he can pay 5 coins to prevent a coup against another player (or against himself), in which case the player who carried out the coup loses the coins he paid.
 */

TEST_CASE("The role of the General -->Blocking a coup ")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Baron baron(game, "Alice");
    General general(game, "Bob");
    Judge judge(game, "Avner");
    Merchant merchant(game, "Dalia");
    vector<Player *> players2 = {&governor, &spy, &baron, &general, &judge, &merchant};
    governor.tax();
    spy.tax();
    baron.tax();
    general.tax();
    judge.tax();
    merchant.tax();
    governor.tax();
    spy.tax();
    baron.gather();
    general.tax();
    judge.gather();
    merchant.gather();
    governor.tax();
    spy.gather();
    baron.gather();
    general.gather();
    judge.gather();
    merchant.gather();
    governor.coup(spy);
    general.BlockCoup(spy);
    spy.gather();
    CHECK(governor.coins() == 2);
    CHECK(spy.coins() == 6);
    CHECK(baron.coins() == 4);
    CHECK(general.coins() == 0);
    CHECK(judge.coins() == 4);
    CHECK(merchant.coins() == 5);
}

/**
 * This test checks the case where a general is attacked by an arrest,
 *  meaning that if he is hit by an arrest, he gets back the coin that was taken from him.
 */
TEST_CASE("The role of the General -->Harm through arrest ")
{
    Game game{};
    General general(game, "Miran");
    Spy spy(game, "Diana");
    Baron baron(game, "Alice");
    vector<Player *> players2 = {&general, &spy, &baron};
    general.tax();
    spy.tax();
    baron.tax();
    general.gather();
    spy.arrest(general);
    CHECK(general.coins() == 3);
    CHECK(spy.coins() == 3);
    CHECK(baron.coins() == 2);
}

/**
 * This test examines the case where the referee blocks another player from using the bribery action.
 * The attacking player will still lose 4 coins.
 */
TEST_CASE("The role of the Judge --> Canceling a bribery operation ")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Judge judge(game, "Dana");
    vector<Player *> players2 = {&governor, &spy, &judge};
    governor.tax();
    spy.tax();
    judge.tax();
    governor.tax();
    spy.gather();
    judge.tax();
    governor.bribe();
    judge.blockBribe(governor);
    CHECK(game.turn() == "Diana");
    spy.gather();
    CHECK(governor.coins() == 2);
    CHECK(spy.coins() == 4);
    CHECK(judge.coins() == 4);
}
/**
 * This test checks the case where a referee is attacked with a sanction.
 * If he is attacked with a sanction, the player who imposed the sanction must pay an additional coin to the cashier.
 */
TEST_CASE("The role of the Judge --> sanction attack ")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Judge judge(game, "Dana");
    vector<Player *> players2 = {&governor, &spy, &judge};
    governor.tax();
    spy.tax();
    judge.tax();
    governor.tax();
    spy.gather();
    judge.tax();
    governor.sanction(judge, "tax");
    CHECK(governor.coins() == 2);
    CHECK(spy.coins() == 3);
    CHECK(judge.coins() == 4);
}
/**
 * This test tests the Merchant - if he starts a turn with at least 3 coins, he receives an additional coin for free.
 */
TEST_CASE("The role of the Merchant --> Extra coin for free")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Merchant merchant(game, "Dana");
    vector<Player *> players2 = {&governor, &spy, &merchant};
    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
    merchant.tax();
    CHECK(merchant.coins() == 2);
    governor.tax();
    CHECK(governor.coins() == 6);
    spy.gather();
    CHECK(spy.coins() == 3);
    merchant.tax();
    CHECK(merchant.coins() == 4);
    governor.tax();
    CHECK(governor.coins() == 9);
    spy.gather();
    CHECK(spy.coins() == 4);
    merchant.gather();
    CHECK(merchant.coins() == 6);
}

/**
 * This test tests the Merchant - meaning if he is attacked with an arrest, he pays two coins to the cashier instead of losing one to another player.
 */
TEST_CASE("The role of the Merchant --> sanction attack")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Merchant merchant(game, "Dana");
    vector<Player *> players2 = {&governor, &spy, &merchant};
    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
    merchant.tax();
    CHECK(merchant.coins() == 2);
    governor.arrest(merchant);
    CHECK(governor.coins() == 3);
    CHECK(merchant.coins() == 0);
}
/**
 * This test checks the bribe action
 * which indeed causes the player who uses it to pay 4 coins in exchange for 2 turns
 */
TEST_CASE("Using action bribe")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Merchant merchant(game, "Dana");
    vector<Player *> players2 = {&governor, &spy, &merchant};
    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
    merchant.tax();
    CHECK(merchant.coins() == 2);

    governor.tax();
    CHECK(governor.coins() == 6);
    spy.tax();
    CHECK(spy.coins() == 4);
    merchant.tax();
    CHECK(merchant.coins() == 4);

    governor.bribe();
    CHECK(game.turn() == "Miran");
    governor.gather();
    CHECK(governor.coins() == 3);
    CHECK(game.turn() == "Miran");
    governor.arrest(spy);
    CHECK(governor.coins() == 4);
    CHECK(spy.coins() == 3);
    CHECK(game.turn() == "Diana");
    spy.tax();
    CHECK(spy.coins() == 5);
}
/**
 * A test that shows a game in which there is a winner
 */
TEST_CASE("Someone winner")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Merchant merchant(game, "Dana");
    vector<Player *> players2 = {&governor, &spy, &merchant};
    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
    merchant.tax();
    CHECK(merchant.coins() == 2);

    governor.tax();
    CHECK(governor.coins() == 6);
    spy.tax();
    CHECK(spy.coins() == 4);
    merchant.tax();
    CHECK(merchant.coins() == 4);

    governor.tax();
    CHECK(governor.coins() == 9);
    spy.tax();
    CHECK(spy.coins() == 6);
    merchant.tax();
    CHECK(merchant.coins() == 7);

    governor.tax();
    CHECK(governor.coins() == 12);
    spy.tax();
    CHECK(spy.coins() == 8);
    merchant.tax();
    CHECK(merchant.coins() == 10);

    governor.coup(merchant);
    CHECK(governor.coins() == 5);
    spy.tax();
    CHECK(spy.coins() == 10);

    governor.tax();
    CHECK(governor.coins() == 8);

    spy.coup(governor);
    CHECK(spy.coins() == 3);
    CHECK(game.winner() == "Diana");
}

/**
 * This checksum ensures that if the player has 10 or more coins and does not perform a flip then an error is thrown
 */

TEST_CASE("10 coins or more and no coup")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Merchant merchant(game, "Dana");
    vector<Player *> players2 = {&governor, &spy, &merchant};
    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
    merchant.tax();
    CHECK(merchant.coins() == 2);

    governor.tax();
    CHECK(governor.coins() == 6);
    spy.tax();
    CHECK(spy.coins() == 4);
    merchant.tax();
    CHECK(merchant.coins() == 4);

    governor.tax();
    CHECK(governor.coins() == 9);
    spy.tax();
    CHECK(spy.coins() == 6);
    merchant.tax();
    CHECK(merchant.coins() == 7);

    governor.tax();
    CHECK(governor.coins() == 12);
    spy.tax();
    CHECK(spy.coins() == 8);
    merchant.tax();
    CHECK(merchant.coins() == 10);
    CHECK_THROWS(governor.tax());
    governor.coup(merchant);
    CHECK(governor.coins() == 5);
    spy.tax();
    CHECK(spy.coins() == 10);
}

/**
 * This test shows that two players with the same role are okay in the game.
 */
TEST_CASE("Two actors with the same role")
{
    Game game{};
    Governor governor(game, "Miran");
    Governor governor1(game, "Diana");
    Merchant merchant(game, "Dana");
    vector<Player *> players2 = {&governor, &governor1, &merchant};
    governor.tax();
    CHECK(governor.coins() == 3);
    governor1.tax();
    CHECK(governor1.coins() == 3);
    merchant.tax();
    CHECK(merchant.coins() == 2);
}
/**
 * This test shows the behavior of the sanction action.
 * Tax cancellation
 */
TEST_CASE("Using action sanction--- > tax")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Merchant merchant(game, "Dana");

    vector<Player *> players2 = {&governor, &spy, &merchant};

    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
    merchant.tax();
    CHECK(merchant.coins() == 2);

    governor.tax();
    CHECK(governor.coins() == 6);
    spy.tax();
    CHECK(spy.coins() == 4);
    merchant.tax();
    CHECK(merchant.coins() == 4);

    governor.sanction(spy, "tax");
    CHECK(governor.coins() == 3);
    CHECK_THROWS(spy.tax());

    spy.gather();
    CHECK(spy.coins() == 5);
    merchant.gather();
    CHECK(merchant.coins() == 6);
}

/**
 * This test shows the behavior of the sanction action.
 * Gather cancellation
 */
TEST_CASE("Using action sanction--- > Gather")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Merchant merchant(game, "Dana");

    vector<Player *> players2 = {&governor, &spy, &merchant};

    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
    merchant.tax();
    CHECK(merchant.coins() == 2);

    governor.tax();
    CHECK(governor.coins() == 6);
    spy.tax();
    CHECK(spy.coins() == 4);
    merchant.tax();
    CHECK(merchant.coins() == 4);

    governor.sanction(spy, "gather");
    CHECK(governor.coins() == 3);
    CHECK_THROWS(spy.gather());

    spy.tax();
    CHECK(spy.coins() == 6);
    merchant.gather();
    CHECK(merchant.coins() == 6);
    governor.gather();
    CHECK(governor.coins() == 4);
}
/**
 * This test shows the effect of arrest on different roles in the game.
 */

TEST_CASE("Using action arrest")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Merchant merchant(game, "Dana");
    General general(game, "Kim");
    vector<Player *> players2 = {&governor, &spy, &merchant, &general};
    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
    merchant.tax();
    CHECK(merchant.coins() == 2);
    general.tax();
    CHECK(general.coins() == 2);
    governor.tax();
    CHECK(governor.coins() == 6);
    spy.tax();
    CHECK(spy.coins() == 4);
    merchant.tax();
    CHECK(merchant.coins() == 4);
    general.tax();
    CHECK(general.coins() == 4);
    governor.arrest(general);
    CHECK(governor.coins() == 7);
    CHECK(general.coins() == 4);
    spy.arrest(merchant);
    CHECK(spy.coins() == 4);
    CHECK(merchant.coins() == 2);
    merchant.arrest(governor);
    CHECK(merchant.coins() == 3);
    CHECK(governor.coins() == 6);
}
/**
 *A test that checks the situation of making an arrest twice
 */

TEST_CASE("Arrest twice in a row on the same player")
{
    Game game{};
    Governor governor(game, "Miran");
    Spy spy(game, "Diana");
    Merchant merchant(game, "Dana");
    General general(game, "Kim");
    vector<Player *> players2 = {&governor, &spy, &merchant, &general};
    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
    merchant.tax();
    CHECK(merchant.coins() == 2);
    general.tax();
    CHECK(general.coins() == 2);
    governor.tax();
    CHECK(governor.coins() == 6);
    spy.tax();
    CHECK(spy.coins() == 4);
    merchant.tax();
    CHECK(merchant.coins() == 4);
    general.tax();
    CHECK(general.coins() == 4);
    governor.arrest(general);
    CHECK(governor.coins() == 7);
    CHECK(general.coins() == 4);
    CHECK_THROWS(spy.arrest(general));
    spy.arrest(merchant);
    CHECK(spy.coins() == 4);
    CHECK(merchant.coins() == 2);
}

/**
 * A test that checks if a player has been eliminated so that he cannot perform an action.
 */

TEST_CASE("Eliminated player cannot play")
{
    Game game{};
    Governor gov(game, "Miran");
    Spy spy(game, "Dana");

    gov.tax(); // 3
    spy.tax(); // 2
    gov.tax(); // 6
    spy.tax(); // 4
    gov.tax(); // 9
    spy.tax(); // 6
    gov.tax(); // 12
    spy.tax(); // 8

    gov.coup(spy); // spy eliminated

    CHECK_THROWS(spy.tax());
}
/**
 * This test checks that the player does not have enough coins.
 */
TEST_CASE("The player does not have enough coins.")
{
    Game game{};
    Governor gov(game, "Miran");
    Spy spy(game, "Dana");
    CHECK_THROWS(gov.bribe());
}

/**
 * A test that checks for an attempted coup blocking without reason, meaning a player did not attempt to turn another player but they still tried to block him.
 */
TEST_CASE("Block coup: valid and invalid cases")
{
    Game game{};
    Governor gov(game, "Miran");
    General general(game, "Dana");
    Spy spy(game, "Lina");
    // שלב 1: צבירת מטבעות
    gov.tax();     // 3
    general.tax(); // 2
    spy.tax();     // 2
    gov.tax();     // 6
    general.tax(); // 5
    spy.tax();     // 4
    gov.tax();     // 9
    general.tax(); // 8
    spy.tax();     // 6
    gov.tax();     // 12
    CHECK_THROWS(general.BlockCoup(spy));
    CHECK(spy.coins() == 6);
    CHECK_THROWS(game.winner());
}

/**
 * A test that checks whether the merchant gets a free coin if he starts his turn with 3+ coins compared to a normal player
 */
TEST_CASE("Merchant gets bonus on turn start if has 3+ coins")
{
    Game game;
    Merchant p1(game, "Kim");
    Judge p2(game, "Kris");
    p1.tax();
    CHECK(p1.coins() == 2);
    p2.tax();
    CHECK(p2.coins() == 2);
    p1.tax();
    CHECK(p1.coins() == 4);
    p2.tax();
    CHECK(p2.coins() == 4);
    p1.tax();
    CHECK(p1.coins() == 7);
    p2.tax();
    CHECK(p2.coins() == 6);
}

/**
 * This test shows the case where a blocking action is performed for TAX but it is not necessary because the player did not use it.
 */
TEST_CASE("Performing a TAX blocking operation - no need")
{
    Game game;
    Governor p1(game, "Kim");
    Judge p2(game, "Kris");
    p1.tax();
    CHECK(p1.coins() == 3);
    p2.gather();
    CHECK(p2.coins() == 1);
    CHECK_THROWS(p1.undo(p2));
}

/**
 * A test that checks whether the governor receives 3 coins when performing TAX compared to a normal player
 */
TEST_CASE("Governor tax gives 3 coins")
{
    Game game;
    Governor governor(game, "Dani");
    Spy spy(game, "Sara");
    governor.tax();
    CHECK(governor.coins() == 3);
    spy.tax();
    CHECK(spy.coins() == 2);
}
