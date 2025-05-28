// ronamsalem4@gmail.com

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

/**
 * @file Player.hpp
 * @brief Abstract base class representing a player in the Coup game.
 * This class serves as the common interface and implementation for all player roles in the game ( Governor, Spy, General, Baron, Judge, Merchant).
 * It manages core player attributes such as coin balance, name, game reference, status flags (arrested, sanctioned), and last action tracking.
 * This class cannot be instantiated directly and must be inherited.
 */

namespace coup
{
    class Game;

    class Player
    {
    private:
        Game &game;                           // A reference to the game in which the player is participating.
        int amount;                           // How many coins does the player have
        string name;                          // Player name.
        bool arrestStatus;                    // Can he not make an arrest (blocked).
        bool sanctionStatus;                  // Blocked from tax/gather
        bool sanctionTax = false;             ///< Whether the player is currently sanctioned from performing the 'tax' action.
        bool sanctionGather = false;          ///< Whether the player is currently sanctioned from performing the 'gather' action.
        bool stillingame;                     // Checks whether a player is still in the game.
        Player *lastArrestedTarget = nullptr; // Who was the ultimate goal of the actor's arrest operation?
        bool blockarrestturn;                 // Checks whether someone has blocked him from performing an arrest in the current queue.
        bool bribeStatus;                     // Checks whether a player is blocked from committing a bribe.
        string lastAction;                    // The last action performed by a player.
        int extraTurns = 0;

    public:
        Player(Game &game, const string &name); // A constructor creates a new player – must receive a reference to the game and a name.
        virtual ~Player();                      // Destructor
        Player(const Player &other);            //  Copy constructor
        Player &operator=(const Player &other); //  Copy assignment

        //
        string GetName() const; // @return --->  The player's name as a string.
        Game &GetGame() const;  // @return ---> Reference to the associated Game object.
        int coins() const;      // @return ---> The player's coin count.

        /**
         Checks if the player is under arrest restriction.
         A player under arrest cannot perform arrest actions.
         @return ---> True if the player is currently arrested.
         */
        bool ArrestStatus();

        void ActivateArrest(); // Activates arrest status, preventing the player from arresting others.
        void resetArrest();    //  Clears the player's arrest status, allowing arrests again.

        /**
        Checks if the player is currently under sanction.
        A sanctioned player cannot perform economic actions (gather/tax).
        @return ---> True if sanctioned.
       */

        bool SanctionStatus();

        void ActivateSanction(const std::string &type); // Activates sanction status, blocking gather/tax actions.
        void resetSanction();                           // Removes the sanction status from the player.
        /**
         Checks if the player is blocked from performing an arrest in the current turn only.
         @return ---> True if arrest is blocked for this turn.
         */
        bool blockarrestturnStatus();

        void Activateblockarrestturn(); // Temporarily blocks the player from performing an arrest this turn.
        void resetblockarrestturn();    // Resets the one-turn arrest block status.
        bool bribeStatusStatus();       // Checks if the player is restricted from performing a bribe, @return ---> True if bribe is blocked.
        void ActivateBribeStatus();     // Activates a status that blocks the player from performing a bribe.
        void resetBribeStatus();        //  Clears the bribe restriction status.
        bool Getstillingame();          // Checks if the player is still active in the game. @return ---> True if the player has not been eliminated.
        void eliminated();              // Marks the player as eliminated (after a successful coup).
        void returnToGame();            // Restores the player to the game .

        /*
         *Pure virtual function that returns the role of the player.
         *Must be implemented by all derived role classes.
         * @return ---> The role name as a string ( "Spy", "Governor", ...).
         */
        virtual string GetRole() const = 0;

        /**
         * Increases the player's coin count by the given amount.
        // @param coins Number of coins to add.
         */
        void AddCoins(int coins);

        /**
         * Decreases the player's coin count by the given amount.
         *@param coins --->  Number of coins to subtract.
         *@throws ---> runtime_error if the player doesn't have enough coins.
         */
        void DecreaseCoins(int coins);

        /**
         * Decreases the player's coin count by the given amount.
         * @param coins ---> Number of coins to subtract.
         *  @throws ---> runtime_error if the player doesn't have enough coins.
         */
        void checkCoupMandatory() const;

        virtual void onSanction(); // called when the player is sanctioned.
        /**
         * void SetLastAction(const string &action);
           Records the last action performed by the player.
           Useful for handling blocks or reactions.
         @param ---> action The action's name
         */
        void SetLastAction(const string &action);

        string GetLastAction() const; // return ---> A string representing the last action.
        /**
         * Actions that every player can do
         * 1.gather ---> the player receives one coin from the treasury. This action has no cost and can be blocked by sanction.
         * 2. tax ---> the player receives two coins from the treasury.
         * 3. bribe ---> The player pays 4 coins to perform an additional action on his turn.
         * 4.arrest --- > The player chooses another player and takes one coin from him. It cannot be used on the same player playing continuously.
         * 5.sanction ---> The player chooses another player and prevents them from using economic actions (gather, tax) until their next turn. This action costs 3 coins.
         * 6.coup ---> The player chooses another player and completely removes them from the game. This action costs 7 coins, and can only be blocked under certain conditions.
         */
        virtual void resetRoleState();
        virtual void gather();
        virtual void tax();
        virtual void bribe();
        virtual void arrest(Player &target);
        void sanction(Player &target, const std::string &type);
        virtual void coup(Player &target);
    };
}
#endif
