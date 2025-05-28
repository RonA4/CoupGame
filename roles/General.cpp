// ronamsalem4@gmail.com
#include "General.hpp"
#include "../game/Game.hpp"
namespace coup
{

    /**
     * Constructs a new General player.
     * @param game ---> Reference to the game instance.
     * @param name ---> The player's name.
     */
    General::General(Game &game, const string &name) : Player(game, name) {}

    /**
     * @Returns the role name of the player.
     * @return ---> "General"
     */
    string General::GetRole() const
    {
        return "General";
    }
    /**
     *  Blocks a coup against the given player.
     * The General pays 5 coins to prevent a coup.
     * The targeted player is brought back to the game (if eliminated),
     * and the player who attempted the coup loses their coins.
     *
     * @param target---> The player who was targeted by the coup.
     * @throws ---> invalid_argument If the General does not have at least 5 coins.
     */
    void General::BlockCoup(Player &target)
    {
        if (coins() < 5)
            throw invalid_argument("You can't block the coup, not enough coins");

        if (target.Getstillingame())
            throw invalid_argument("Target is still in the game, cannot block coup");

        DecreaseCoins(5);

        //  target.returnToGame();
        GetGame().returnPlayer(&target);

        std::cout << " BlockCoup successful! " << target.GetName() << " returned to the game.\n";
        GetGame().advanceTurn();
    }
    /**
     * Called when the General is arrested.
     * The General is refunded the coin that was taken from him during the arrest.
     */
    void General::Gotarrested()
    {
        AddCoins(1); // Gets back the coin lost due to arrest
    }
}