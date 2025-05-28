// ronamsalem4@gmail.com
#ifndef SPY_HPP
#define SPY_HPP
#include "../game/Player.hpp"

/*
 *This class describes a player role in the game -Spy inherits from Player
 *Spy - can see another player's coin count and prevent them from using the arrest action on their next turn. This action does not cost coins and does not count as a turn.
 */
namespace coup
{

    class Spy : public Player
    {
    public:
        Spy(Game &game, const string &name);              // Constructs a new Spy player.
        string GetRole() const override;                  // Returns the role name: "Spy".
        void watchCoins(Player &target) const;            // target The player whose coins are being watched.
        void blockarrestfromplayer(Player &target) const; // Prevent the target player from performing an arrest on their next turn.
    };
}

#endif