// ronamsalem4@gmail.com
#ifndef MERCHANT_HPP
#define MERCHANT_HPP
#include "../game/Player.hpp"

/**
 *  * This class describes a player's role in the game --->Merchant inherits from Player.
 * Merchant - If he starts a turn with at least 3 coins, he gets an extra coin for free. Additionally, if he is attacked in a hold, he pays two coins to the cashier instead of losing one coin to another player.
 */
namespace coup
{

    class Merchant : public Player
    {
    public:
        Merchant(Game &game, const string &name); // onstructs a new Merchant player.
        string GetRole() const override;          // Returns the role name: "Merchant".
        void bribe();                             // Executes the Merchant's version of the bribe action.
        void checkTurn();                         // Checks if it's currently the Merchant's turn.
        void getArrestded();                      // andles logic when the Merchant is targeted by arrest.
        void getArrested();                       // Handles logic when the Merchant is arrested.
        void gather();                            // Performs the gather action (earn 1 coin).
        void tax();                               // erforms the tax action
    };
}

#endif