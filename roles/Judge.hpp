// ronamsalem4@gmail.com

#ifndef JUDGE_HPP
#define JUDGE_HPP
#include "../game/Player.hpp"
/**
 * This class describes a player role in the game -Judge inherits from Player
 * Judge - can cancel another player's bribe, causing them to lose the 4 coins they paid.
 * Additionally, if they are attacked with a sanction, the player who imposed the sanction must pay an additional coin to the cashier.
 */
namespace coup
{

    class Judge : public Player
    {
    public:
        Judge(Game &game, const string &name); // Constructs a new Judge player
        string GetRole() const override;       // Returns the role name: "Judge".
        void undo(Player &target);             // Reverses the elimination of a target player.
        void blockBribe(Player &target);       // Blocks a bribe action performed by another player.
        void gotSanctioned(Player &aggressor); // Added method to handle sanction against judge
    };

}

#endif
