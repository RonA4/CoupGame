// ronamsalem4@gmail.com
#ifndef GAME_HPP
#define GAME_HPP
#include <vector>
#include <string>
#include <stdexcept>
#include <map>
using namespace std;
/**
 * @class game
 * @namespace coup
 * Manages the overall state and flow of the Coup game.
 * The game class keeps track of all players participating in the game, manages the order of turns, enforces the game start/end rules, and determines the winner.
 * It acts as a central controller through which players interact and take turns.
 */

namespace coup
{
    class Player;

    class Game
    {
    private:
        std::map<std::string, int> extra_turns;
        vector<Player *> list_players; // List of all players who have joined the game.
        size_t index;                  // Index of the current player's turn in the list.
        bool startGame;                // flag indicating whether the game has started.
        Player *lastArrestedVictim = nullptr;

    public:
        /**
         *  Constructs a new Game object.
         * Initializes the game with an empty player list,
         * turn index set to 0, and the game not yet started.
         */
        Game();

        /**
         * Adds a new player to the game.
         * Can only be called before the game starts. The player is added to the internal list of players, and their name will appear in turn order.
         * @param player ---> Pointer to the Player to add.
         * @throws std::runtime_error If the game has already started  or the maximum number of players has been reached.
         */
        void addPlayer(Player *player);

        /**
         *  Eliminates a player from the game.
         * Marks the player as inactive.
         *  Eliminated players are skipped during turn progression and cannot perform actions.
         * @param player ---> Pointer to the Player to eliminate.
         */
        void eliminatePlayer(Player *player);

        /**
         * Returns a previously eliminated player to the game.
         * Useful for undo mechanics or testing. Marks the player as active again.
         * @param player ---> Pointer to the Player to return.
         */
        void returnPlayer(Player *player);
        /**
         *  Checks if it's currently the given player's turn.
         *
         * @param player ---> The player to check.
         * @return ---> true if it is the player's turn, false otherwise.
         */
        bool isPlayerTurn(const Player &player) const;

        /**
         * Advances the turn to the next active player.
         * Automatically skips eliminated players. Called after each valid action.
         */
        void advanceTurn();
        /**
         * Gets a list of names of players still in the game.
         *
         * Only includes players who are currently active (not eliminated).
         *
         * @return --->  Vector of player names as strings.
         */

        vector<string> players() const;
        /**
         * @return---> The name of the current player.
         * @throws --->  std::runtime_error If no players are in the game.
         */
        string turn() const;

        /**
         * @return ---> The name of the winner.
         * @throws ---> std::runtime_error If the game is not over yet.
         */
        string winner() const;

        void addExtraTurns(const std::string &playerName, int count); // Adds extra turns to the given player.
        bool hasExtraTurn(const std::string &playerName) const;       // Checks if the player has any extra turns left.
        void useExtraTurn(const std::string &playerName);             // Consumes one of the player's extra turns, if any.
        void removeExtraTurns(const std::string &playerName);         // Removes all extra turns from the given player.
        void setLastArrestedVictim(Player *player);                   // Sets the last player who was arrested.
        Player *getLastArrestedVictim() const;                        // Retrieves the last player who was arrested.
        void resetLastArrestedVictim();                               // Clears the record of the last arrested player.
        bool hasEnoughPlayers() const;                                // Checks if the game has at least two players.
    };
}
#endif