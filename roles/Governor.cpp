// ronamsalem4@gmail.com

#include "Governor.hpp"
#include "../game/Game.hpp"
namespace coup
{

    /**
     * Constructs a new Governor player.
     * @param game ---> Reference to the game instance.
     * @param name ---> The player's name.
     */
    Governor::Governor(Game &game, const string &name) : Player(game, name) {}

    /**
     *  Returns the role name of the player.
     * @return ---> "Governor"
     */
    string Governor::GetRole() const
    {
        return "Governor";
    }
    /**
     * Performs a special tax action.
     * The Governor takes 3 coins from the treasury (instead of 2).
     * Sets the last action and advances the game turn.
     */
    void Governor::tax()
    {
        checkCoupMandatory();
        AddCoins(3);
        SetLastAction("tax");
        GetGame().advanceTurn();
    }
    /**
     *  Cancels a tax action performed by the target player.
     * If the last action of the target was "tax", removes 2 or 3 coins
     * from them depending on their role.
     * @param target ---> The player whose tax action is being undone.
     * @throws ---> invalid_argument If the last action was not tax, or the target  doesn't have enough coins to undo.
     */
    void Governor::undo(Player &target)
    {
        if (target.GetLastAction() != "tax")
        {
            throw invalid_argument("You can only undo a tax action.");
        }

        int refund = (target.GetRole() == "Governor") ? 3 : 2;
        if (target.coins() < refund)
        {
            throw invalid_argument("Target doesn't have enough coins to undo.");
        }

        target.DecreaseCoins(refund);
    }
}
