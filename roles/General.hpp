// ronamsalem4@gmail.com
#ifndef GENERAL_HPP
#define GENERAL_HPP
#include "../game/Player.hpp"

/**
 *This class describes a player role in the game -General inherits from Player.
 *And to pay 5 coins to prevent a coup against another player (or against himself) in this case the player who carried out the coup loses the coins he paid.
 * In addition, if he is harmed by an arrest, he gets back the coin that was taken from him.
 */
namespace coup
{

    class General : public Player
    {
    public:
        General(Game &game, const string &name); // Constructs a new General player.
        string GetRole() const override;         // Returns the role name: "General".
        void BlockCoup(Player &target);          // Blocks a coup targeting the given player by paying 5 coins.
        void Gotarrested();                      // Called when the General is arrested.
    };
}

#endif