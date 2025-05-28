// ronamsalem4@gmail.com

#include "game/Player.hpp"
#include "roles/Governor.hpp"
#include "roles/Spy.hpp"
#include "roles/Baron.hpp"
#include "roles/General.hpp"
#include "roles/Judge.hpp"
#include "roles/Merchant.hpp"
#include "game/Game.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;
using namespace coup;

/**
 * @ Demonstrates two full game simulations using various role-based actions.
 * This program initializes two separate game sessions (game_1 and game_2) to showcase and test
  the behavior of different roles in a role-based strategy game. Players are assigned one of six
 * unique roles: Governor, Spy, Baron, General, Judge, or Merchant—each with distinct capabilities.
 * The game flow is driven by sequential turn-based actions such as tax, gather, arrest, bribe, sanction, and coup. The state of the game is printed after each action using
 * helper functions like printTurn, printLine, and printCoinsAll, ensuring visibility of turn order and players coin balances.
 * The simulation includes edge cases such as undoing actions, blocking actions, and executing
 * illegal moves (e.g., attempting tax under sanction), with exception handling to verify
 * proper rule enforcement.
 * The main objective of this code is to serve as a test harness and visualization tool for verifying the correctness and interactions of all role mechanics in the game engine.
 * Explanation: There is no documentation in the code itself because it has to be run and in the terminal there is a breakdown after each action performed.
 * In the game, I considered the invest action (the Baron's investment action) as a turn, meaning that if he uses it, the turn moves on.
 * The second game simulates a game victory.
 */

/**
* functions for printing game state and player actions.
* These helper functions are used throughout the game simulation to display the
 current turn, player actions, and coin statuses in a consistent and visual format.
* printTurn(Game&) ---> Displays the name of the current player's turn using a turn icon.
* printCoins(Player&) ---> Prints the coin balance of a single player with a coin icon.
* printCoinsAll(vector<Player*>&) ---> Iterates through all players and prints their coin balances.
* printLine(const string&) ---> Prints a stylized description of an action being taken.
* These functions enhance the clarity of the game's output, making it easier to follow the sequence of actions and the game's progression.
*/
// print function
void printTurn(Game &game)
{
    cout << "\n\U0001F501 Turn: " << game.turn() << "\n";
}

// print function
void printCoins(Player &p)
{
    cout << "\U0001F4B0 " << p.GetName() << " has " << p.coins() << " coins\n";
}

void printCoinsAll(const vector<Player *> &players)
{
    cout << "\n\U0001F4CA Coin Status:\n";
    for (Player *p : players)
    {
        printCoins(*p);
    }
    cout << "---------------------------\n";
}

void printLine(const string &action)
{
    cout << "\n\u27A1\uFE0F " << action << "\n";
    cout << "---------------------------\n";
}

int main()
{
    Game game_1{};

    Governor governor(game_1, "Moshe");
    Spy spy(game_1, "Yossi");
    Baron baron(game_1, "Meirav");
    General general(game_1, "Reut");
    Judge judge(game_1, "Gilad");
    Merchant merchant(game_1, "Dana");

    vector<Player *> allPlayers = {&governor, &spy, &baron, &general, &judge, &merchant};

    cout << "\n🎮 Players in the game:\n";
    for (const string &name : game_1.players())
    {
        cout << "- " << name << "\n";
    }

    cout << "\n===========================\n";
    cout << "\U0001F4A1 Checking the role of Governor\n";

    printTurn(game_1);
    printLine("Moshe performs tax");
    governor.tax();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Yossi performs tax");
    spy.tax();
    printLine("Governor undoes Spy's tax");
    governor.undo(spy);
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Meirav performs tax");
    baron.tax();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Reut performs gather");
    general.gather();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Gilad performs gather");
    judge.gather();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Dana performs gather");
    merchant.gather();
    printCoinsAll(allPlayers);

    cout << "\n\U0001F575 Checking the role of Spy\n";

    printTurn(game_1);
    printLine("Moshe performs tax");
    governor.tax();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Yossi watches Moshe");
    spy.watchCoins(governor);
    printLine("Yossi performs tax");
    spy.tax();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Meirav performs tax");
    baron.tax();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Reut performs tax");
    general.tax();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Gilad performs tax");
    judge.tax();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Dana performs tax");
    merchant.tax();
    printCoinsAll(allPlayers);

    cout << "\n\U0001F6A8 Moshe tries to make an arrest\n";
    printTurn(game_1);
    printLine("Moshe attempts arrest on Meirav");
    try
    {
        governor.arrest(baron);
        cout << "✅ Arrest succeeded.\n";
    }
    catch (const std::exception &e)
    {
        cerr << "❌ " << e.what() << "\n";
    }
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Moshe performs tax");
    governor.tax();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Yossi performs tax");
    spy.tax();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Meirav performs gather");
    baron.gather();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Reut performs gather");
    general.gather();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Gilad performs gather");
    judge.gather();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Dana performs gather");
    merchant.gather();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Moshe performs arrest on Yossi");
    governor.arrest(spy);
    printCoinsAll(allPlayers);

    cout << "\n\U0001F3E0 Checking the role of Baron\n";
    printTurn(game_1);
    printLine("Yossi performs gather");
    spy.gather();
    printCoinsAll(allPlayers);

    printLine("Meirav invests");
    baron.invest();
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Reut performs tax");
    general.tax();
    printCoinsAll(allPlayers);

    cout << "Before sanction:\n";
    printCoinsAll(allPlayers);
    printLine("Gilad sanctions Meirav (tax)");
    judge.sanction(baron, "tax");
    printCoinsAll(allPlayers);

    cout << "\n\U0001F396 Checking the role of General and Coup\n";
    printLine("Dana performs gather");
    merchant.gather();
    printCoinsAll(allPlayers);

    printLine("Moshe performs tax");
    try
    {
        governor.tax();
        cout << "✅ tax succeeded.\n";
    }
    catch (const std::exception &e)
    {
        cerr << "❌ " << e.what() << "\n";
    }
    printCoinsAll(allPlayers);

    printLine("Moshe coups Yossi");
    governor.coup(spy);
    printCoinsAll(allPlayers);

    printLine("Reut blocks coup");
    general.BlockCoup(spy);
    printCoinsAll(allPlayers);

    cout << "\n\U0001F3AE Remaining players:\n";
    for (const string &name : game_1.players())
    {
        cout << "- " << name << "\n";
    }

    printLine("Meirav attempts arrest on Reut");
    try
    {
        baron.arrest(general);
        cout << "✅ arrest succeeded.\n";
    }
    catch (const std::exception &e)
    {
        cerr << "❌ " << e.what() << "\n";
    }
    printCoinsAll(allPlayers);

    printTurn(game_1);
    printLine("Yossi performs tax");
    spy.tax();
    printCoinsAll(allPlayers);

    printLine("Meirav arrests Reut again");
    baron.arrest(general);
    printCoinsAll(allPlayers);

    printLine("Reut performs gather");
    general.gather();
    printCoinsAll(allPlayers);

    printLine("Gilad performs gather");
    judge.gather();
    printCoinsAll(allPlayers);

    cout << "\n\U0001F9D1‍⚖️ Checking the role of Judge\n";
    printLine("Dana sanctions Gilad (tax)");
    merchant.sanction(judge, "tax");
    printCoinsAll(allPlayers);

    printLine("Moshe performs tax");
    governor.tax();
    printCoinsAll(allPlayers);

    printLine("Yossi performs bribe");
    spy.bribe();
    printCoinsAll(allPlayers);

    printLine("Gilad blocks bribe");
    judge.blockBribe(spy);
    printCoinsAll(allPlayers);

    printTurn(game_1);
    cout << "\n\U0001F4B5 Checking Merchant and Coup logic\n";
    printLine("Meirav coups Moshe");
    baron.coup(governor);
    printCoinsAll(allPlayers);

    cout << "\n🎮 Players in the game:\n";
    for (const string &name : game_1.players())
    {
        cout << "- " << name << "\n";
    }

    printLine("Reut arrests Dana");
    general.arrest(merchant);
    printCoinsAll(allPlayers);

    // start a new game
    printLine("\U0001F3F8 New game");

    Game game_2{};

    Governor governor1(game_2, "Ron");
    Spy spy1(game_2, "Or");
    Baron baron1(game_2, "Shir");
    General general1(game_2, "Yoram");
    Judge judge1(game_2, "Tali");
    Merchant merchant1(game_2, "Dor");

    vector<Player *> players2 = {&governor1, &spy1, &baron1, &general1, &judge1, &merchant1};

    cout << "\n🎮 Players in the game:\n";
    for (const string &name : game_2.players())
    {
        cout << "- " << name << "\n";
    }

    cout << "\n===========================\n";

    printTurn(game_2);
    printLine("Ron performs tax");
    governor1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Or performs tax");
    spy1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Shir performs gather");
    baron1.gather();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Tali performs gather (from game 1 judge)");
    try
    {
        judge.gather();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Yoram performs tax");
    general1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Tali performs tax");
    judge1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Dor performs tax");
    merchant1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Ron performs tax again");
    governor1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Or arrests Yoram");
    spy1.arrest(general1);
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Shir performs tax");
    baron1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Yoram performs tax");
    general1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Tali performs tax");
    judge1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Dor performs tax");
    merchant1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Ron performs bribe");
    governor1.bribe();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Ron performs tax");
    governor1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Ron performs tax again");
    governor1.tax();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Or performs gather");
    spy1.gather();
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Shir arrests Ron");
    baron1.arrest(governor1);
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Yoram attempts to arrest Ron");
    try
    {
        general1.arrest(governor1);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    printCoinsAll(players2);

    printLine("Yoram arrests Gilad from previous game");
    general1.arrest(judge);
    printCoinsAll(players2);

    printLine("Tali arrests Ron");
    judge1.arrest(governor1);
    printCoinsAll(players2);

    printLine("Dor performs tax");
    merchant1.tax();
    printCoinsAll(players2);

    printLine("Ron performs tax");
    governor1.tax();
    printCoinsAll(players2);

    printLine("Or performs tax");
    spy1.tax();
    printCoinsAll(players2);

    printLine("Shir sanctions Yoram (tax)");
    baron1.sanction(general1, "tax");
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Yoram attempts tax under sanction");
    try
    {
        general1.tax();
        cout << "✅ Tax succeeded.\n";
    }
    catch (const std::exception &e)
    {
        cerr << "❌ " << e.what() << "\n";
    }
    printCoinsAll(players2);

    printTurn(game_2);
    printLine("Yoram performs gather");
    general1.gather();
    printCoinsAll(players2);

    printLine("Tali performs tax");
    judge1.tax();
    printCoinsAll(players2);

    printLine("Dor performs tax");
    merchant1.tax();
    printCoinsAll(players2);

    printLine("Ron performs tax");
    governor1.tax();
    printCoinsAll(players2);

    printLine("Or performs tax");
    spy1.tax();
    printCoinsAll(players2);

    printLine("Shir performs tax");
    baron1.tax();
    printCoinsAll(players2);

    printLine("Yoram performs tax");
    general1.tax();
    printCoinsAll(players2);

    printLine("Tali performs coup");
    judge1.coup(baron1);
    printCoinsAll(players2);

    cout << "\n🎮 Players in the game:\n";
    for (const string &name : game_2.players())
    {
        cout << "- " << name << "\n";
    }

    printLine("Dor performs coup");
    merchant1.coup(judge1);
    printCoinsAll(players2);

    cout << "\n🎮 Players in the game:\n";
    for (const string &name : game_2.players())
    {
        cout << "- " << name << "\n";
    }

    printLine("Ron performs coup");
    governor1.coup(merchant1);
    printCoinsAll(players2);

    cout << "\n🎮 Players in the game:\n";
    for (const string &name : game_2.players())
    {
        cout << "- " << name << "\n";
    }

    printLine("Or performs coup");
    spy1.coup(general1);
    printCoinsAll(players2);

    cout << "\n🎮 Players in the game:\n";
    for (const string &name : game_2.players())
    {
        cout << "- " << name << "\n";
    }

    printLine("Ron performs tax");
    governor1.tax();
    printCoinsAll(players2);

    printLine("Or performs tax");
    spy1.tax();
    printCoinsAll(players2);

    printLine("Ron performs coup");
    governor1.coup(spy1);
    printCoinsAll(players2);

    printTurn(game_2);

    try
    {
        string w = game_2.winner();
        cout << "\n🏆 The winner of Game 2 is: " << w << "!\n";
    }
    catch (const std::exception &e)
    {
        cout << "\nNo winner yet in Game 2.\n";
    }
    cout << "\n=======================\n";
}
