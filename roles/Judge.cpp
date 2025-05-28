// ronamsalem4@gmail.com
#include "Judge.hpp"
#include "../game/Game.hpp"

namespace coup
{
    /**
     * Constructs a new Judge player.
     * @param game ---> Reference to the game instance.
     * @param name ---> The player's name.
     */
    Judge::Judge(Game &game, const string &name) : Player(game, name) {}
    /**
     *  Returns the role name of the player.
     * @return ---> "Judge"
     */
    string Judge::GetRole() const
    {
        return "Judge";
    }
    /**
     *Performs the undo action on a target player.
     * If the target is a Spy, removes 2 coins from them as a penalty.     *
     * @param target  ---> The player to act on.
     * @throws ---> invalid_argument If the target is not a Spy.
     */
    void Judge::undo(Player &target)
    {
        if (target.GetRole() != "Spy")
            throw invalid_argument("Judge cannot undo tax.");
        target.DecreaseCoins(2);
    }

    /**
     * Blocks a bribe action by the target player.
     * Resets the bribe status and causes the target to lose 4 coins.
     * @param target  ---> The player whose bribe is being blocked.
     * @throws ---> invalid_argument If there is no active bribe to block.
     */
    void Judge::blockBribe(Player &target)
    {
        if (!target.bribeStatusStatus())
            throw invalid_argument("No bribe action to block");
        target.resetBribeStatus();
        std::cout << "Judge blocked bribe by " << target.GetName() << std::endl;
        target.GetGame().removeExtraTurns(target.GetName()); // ביטול תורות נוספים אם היו
        target.GetGame().advanceTurn();
    }

    /**
     *  Called when the Judge is sanctioned.
     * In response, the attacker loses 1 additional coin as a penalty.
     * @param attacker ---> The player who applied the sanction.
     */
    void Judge::gotSanctioned(Player &attacker)
    {
        attacker.DecreaseCoins(1); // attacker pays extra to the bank
    }
}

