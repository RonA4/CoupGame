// ronamsalem4@gmail.com

#include "Baron.hpp"
#include "../game/Game.hpp"
#include <stdexcept>

namespace coup
{

    /**
     * Constructs a new Baron player and registers them to the game.
     * @param game ---> Reference to the Game instance.
     * @param name ---> The player's name.
     */
    Baron::Baron(Game &game, const std::string &name) : Player(game, name), investedThisTurn(false) {}

    /**
     * Returns the role name of the player.
     * @return  ---> "Baron"
     */
    std::string Baron::GetRole() const
    {
        return "Baron";
    }

    /**
     * Performs the Baron's invest action: pay 3 coins to gain 6.
     * Can only be used if the player has at least 3 coins.
     * Sets the investment flag to prevent reuse this turn.
     * Ends the player's turn.
     * @throws ---> std::runtime_error if the player doesn't have enough coins.
     */
    void Baron::invest()
    {
        checkCoupMandatory();
        if (coins() < 3)
        {
            throw std::runtime_error("Not enough coins to invest");
        }
        DecreaseCoins(3);
        AddCoins(6);
        investedThisTurn = true;
        GetGame().advanceTurn();
    }

    /**
     *Called when the Baron is sanctioned.
     * If the Baron has not yet invested this turn, they receive 1 coin compensation.
     */
    void Baron::onSanction()
    {
        if (!investedThisTurn)
        {
            AddCoins(1);
        }
    }

    /**
     * Resets the investment flag at the end of the turn.
     * Allows the Baron to invest again in the next turn.
     */

    void Baron::resetInvestFlag()
    {
        investedThisTurn = false;
    }

    /**
     * Resets the Baron's role-specific state at the end of their turn.
     * Currently resets the invest action flag to allow investment in future turns.
     */
    void Baron::resetRoleState()
    {
        resetInvestFlag();
    }
}
