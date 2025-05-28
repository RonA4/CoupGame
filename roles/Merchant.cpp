// ronamsalem4@gmail.com
#include "Merchant.hpp"
#include "../game/Game.hpp"

namespace coup
{
    /**
     *  Constructs a new Merchant player.
     * @param game ---> Reference to the game instance.
     * @param name ---> The player's name.
     */
    Merchant::Merchant(Game &game, const string &name) : Player(game, name) {}
    /**
     * Returns the role name of the player.
     * @return ---> "Merchant"
     */
    string Merchant::GetRole() const
    {
        return "Merchant";
    }
    /**
     * Performs the gather action with Merchant's bonus.
     * If the Merchant has 3 or more coins, they receive +1 bonus coin before the regular gather.
     * Then delegates to the base Player::gather() implementation.
     * @throws ---> invalid_argument If it's not the Merchant's turn.
     */
    void Merchant::gather()
    {
        checkCoupMandatory();
        if (!GetGame().isPlayerTurn(*this))
            throw invalid_argument("Not your turn");
        checkCoupMandatory();
        if (coins() >= 3)
            AddCoins(1); // Bonus
        Player::gather();
    }
    /**
     *  Performs the tax action with Merchant's bonus.
     * If the Merchant has 3 or more coins, they receive +1 bonus coin before the regular tax.
     * Then delegates to the base Player::tax() implementation.
     * @throws ---> invalid_argument If it's not the Merchant's turn.
     */
    void Merchant::tax()
    {
        checkCoupMandatory();
        if (!GetGame().isPlayerTurn(*this))
            throw invalid_argument("Not your turn");
        checkCoupMandatory();

        if (coins() >= 3)
            AddCoins(1); // Bonus
        Player::tax();
    }
    /**
     *  Performs the bribe action with Merchant's bonus.
     * If the Merchant has 3 or more coins, they receive +1 bonus coin before the regular bribe.
     * Then delegates to the base Player::bribe() implementation.
     * @throws ---> invalid_argument If it's not the Merchant's turn.
     */
    void Merchant::bribe()
    {
        checkCoupMandatory();
        if (!GetGame().isPlayerTurn(*this))
            throw invalid_argument("Not your turn");
        checkCoupMandatory();
        if (coins() >= 3)
            AddCoins(1); // Bonus
        Player::bribe();
    }

    /**
     * Called when the Merchant is arrested.
     * If the Merchant is under arrest status, they lose 2 coins as penalty.
     */
    void Merchant::getArrested()
    {
        if (ArrestStatus())
        {
            DecreaseCoins(2); // Pay to bank
        }
    }
}
