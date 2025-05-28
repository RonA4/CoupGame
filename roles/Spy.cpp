// ronamsalem4@gmail.com

#include "Spy.hpp"
#include "../game/Game.hpp"

namespace coup
{
    /**
     *  Constructs a new Spy player.
     * @param game ---> Reference to the game.
     * @param name ---> The player's name.
     */

    Spy::Spy(Game &game, const string &name) : Player(game, name) {}

    /**
     * Returns the role name of the player.
     * @return --->  "Spy"
     */

    string Spy::GetRole() const
    {
        return "Spy";
    }

    /**
     *  Blocks the target player from performing an arrest on their next turn.
     * This action does not cost coins and does not consume the Spy's turn.
     * @param target ---> The player to restrict.
     * @throws ---> invalid_argument If the target player is not in the game.
     */
    void Spy::blockarrestfromplayer(Player &target) const
    {
        if (!target.Getstillingame())
            throw new invalid_argument(target.GetName() + " is not part of the game");
        target.Activateblockarrestturn();
    }

    /**
     * Displays the target player's coin count and prevents them from using the arrest action on their next turn.     * This action does not cost coins and does not consume the Spy's turn.
     * @param target  ---> The player whose coin count is being checked.
     * @throws ---> invalid_argument If the target player is not in the game.
     */
    void Spy::watchCoins(Player &target) const
    {
        if (!target.Getstillingame())
            throw new invalid_argument(target.GetName() + " is not part of the game");
        cout << target.GetName() << " has " << target.coins() << " coins" << endl;

        target.Activateblockarrestturn();
    }

}