// ronamsalem4@gmail.com
#pragma once
#include "../game/Player.hpp"
/**
 * This class describes a player's role in the game --->Baron inherits from Player.
 * Baron - can "invest" 3 coins and receive 6 coins in return. Additionally, if he is attacked by a sanction, he receives 1 coin as compensation.
 */
namespace coup
{
    class Baron : public Player
    {
    private:
        bool investedThisTurn = false;

    public:
        Baron(Game &game, const std::string &name);           // A constructor through which we will create a Baron object
        std::string GetRole() const override;                 // Accepting the role of the actor
        void invest();                                        // Baron's upgrade operation
        void getscanction();                                  // Receiving compensation in the event of an attack by a scanction
        void resetInvestFlag();                               // Resets the investment flag .
        void onSanction() override;                           // Checking whether the player is being targeted by a sanction
        bool hasInvested() const { return investedThisTurn; } // Checks if the Baron has already invested this turn.
        void resetRoleState() override;                       // Resets the Baron's role-specific state at the end of their turn.
    };
}
