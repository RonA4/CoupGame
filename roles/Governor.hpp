// ronamsalem4@gmail.com

#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP
#include "../game/Player.hpp"

/*
*This class describes a player role in the game -Governor inherits from Player
*Governor - Takes 3 coins from the treasury instead of 2 when using a tax action.
In addition, he can cancel other players' tax actions.
 */
namespace coup
{

    class Governor : public Player
    {
    public:
        Governor(Game &game, const string &name); // onstructs a new Governor player.
        string GetRole() const override;          // Returns the role name: "Governor".
        void tax() override;                      // Performs a special tax action that earns 3 coins instead of 2.
        void undo(Player &target);                // Cancels a tax action performed by the target player.
    };
}

#endif