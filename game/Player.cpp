// ronamsalem4@gmail.com
#include "Player.hpp"
#include "Game.hpp"
#include <stdexcept>

/**
 * @class Player
 * @namespace coup
 *  Abstract base class representing a generic player in the Coup game.
 * This class defines the common interface and shared functionality for all player roles
 * in the game. It provides mechanisms for performing core game actions such as  gather, tax, bribe, arrest, sanction, and coup.
 * The Player class manages the player's current coin count, name, and status flags
 * This class is abstract and cannot be instantiated directly.
 */
namespace coup
{
    /**
     * Creates a new player and registers it in the game.
     * @param game ---> A reference to the game the player belongs to.
     * @param name ---> The name of the player.
     * Initializes all status flags to false and the coin count to 0.
     * Automatically adds the player to the game upon creation.
     */
    Player::Player(Game &game, const std::string &name) : game(game), amount(0),
                                                          name(name),
                                                          arrestStatus(false),
                                                          sanctionStatus(false),
                                                          stillingame(true),
                                                          blockarrestturn(false),
                                                          bribeStatus(false),
                                                          lastAction("")
    {
        game.addPlayer(this);
    }

    /**
     * Destructor for the Player class.
     * This destructor is responsible for cleaning up resources used by the Player instance.
     * Since the Player class does not allocate dynamic memory explicitly, the destructor is currently empty.
     *However, it is declared and defined explicitly to fulfill   the Rule of Three and ensure proper behavior in derived classes with virtual destructors.
     */
    Player::~Player()
    {
    }

    /**
     * Copy constructor for Player.
     *
     * Creates a new Player object by copying the name and amount from another player.
     * The game reference is shallow-copied (both players point to the same Game).
     *
     * @param other ---> The Player to copy from.
     */
    Player::Player(const Player &other)
        : game(other.game) ,amount(other.amount) , name(other.name){}

    /**
 * Copy assignment operator for Player.
 * Assigns the name and amount fields from another Player instance.
 * The game reference remains unchanged (same Game object).
 * @param other ---> The Player to assign from.
 * @return ---> Reference to this Player.
 */
    Player &Player::operator=(const Player &other)
    {
        if (this != &other)
        {
            name = other.name;
            amount = other.amount;
        }
        return *this;
    }

    /**
     *@return ---> A reference to the game the player is participating in.
     */
    Game &Player::GetGame() const
    {
        return game;
    }

    /**
     * @return --->  The player's name.
     */
    std::string Player::GetName() const
    {
        return name;
    }

    /**
     * @return ---> The current amount of coins the player has.
     */
    int Player::coins() const
    {
        return amount;
    }

    /**
     *  Checks if the player is under arrest restriction.
     * @return ---> True if arrested.
     */
    bool Player::ArrestStatus()
    {
        return arrestStatus;
    }

    /**
     *  Marks the player as arrested (prevents arresting others).
     */
    void Player::ActivateArrest()
    {
        arrestStatus = true;
    }

    /**
     * Resets the player's arrest status.
     */
    void Player::resetArrest()
    {
        arrestStatus = false;
    }

    /**
     *  Checks if the player is currently sanctioned.
     * @return ---> True if under economic restriction.
     */
    bool Player::SanctionStatus()
    {
        return sanctionStatus;
    }

    /**
     * Activates the bribe status for the player.
     * This function marks the player as having performed a bribe action.
     * Used to track whether a bribe has occurred, so that it can be blocked
     * by a Judge or to apply special rules (e.g., multiple turns).
     */

    void Player::ActivateBribeStatus()
    {
        bribeStatus = true;
    }

    /**
     *  Removes the sanction status from the player.
     */
    void Player::resetSanction()
    {
        sanctionTax = false;
        sanctionGather = false;
    }

    /**
     *  Checks if the player is blocked from arresting this turn only.
     */
    bool Player::blockarrestturnStatus()
    {
        return blockarrestturn;
    }

    /**
     *  Temporarily blocks arresting ability for this turn.
     */
    void Player::Activateblockarrestturn()
    {
        blockarrestturn = true;
    }
    /**
     *  Resets the one-turn arrest block.
     */
    void Player::resetblockarrestturn()
    {
        blockarrestturn = false;
    }

    /**
     *  Checks if the player is restricted from performing a bribe.
     */
    bool Player::bribeStatusStatus()
    {
        return bribeStatus;
    }

    /**
     * Activates a sanction on the player.
     * Applies a specific type of economic restriction to the player,  such as blocking the ability to perform a "tax" or "gather" action.
     * This is typically used as a consequence of another player's action.
     * @param type ---> A string indicating the type of action to block.
     *   Valid values are "tax" and "gather".
     */

    void Player::ActivateSanction(const std::string &type)
    {
        if (type == "tax")
            sanctionTax = true;
        else if (type == "gather")
            sanctionGather = true;
    }

    /**
     *  Clears the bribe restriction.
     */
    void Player::resetBribeStatus()
    {
        bribeStatus = false;
    }

    /**
     * Checks if the player is still active in the game.
     * @return ---> True if still alive.
     */
    bool Player::Getstillingame()
    {
        return stillingame;
    }

    /**
     * Eliminates the player from the game.
     */
    void Player::eliminated()
    {
        stillingame = false;
    }

    /**
     *  Returns the player back to the game (used in special cases).
     */
    void Player::returnToGame()
    {
        stillingame = true;
    }

    /**
     *  Adds the specified number of coins to the player.
     */
    void Player::AddCoins(int coins)
    {
        amount += coins;
    }

    /**
     * Deducts coins from the player.
     * @param coins ---> Amount to deduct.
     * @throws ---> std::invalid_argument If player doesn't have enough coins.
     */
    void Player::DecreaseCoins(int coins)
    {
        if (amount < coins)
            throw std::invalid_argument("amount bigger than coins");
        amount -= coins;
    }

    /**
     *  Ensures that players with 10 or more coins must perform a coup.
     * @throws ---> std::invalid_argument If player has 10+ coins but tries a different action.
     */
    void Player::checkCoupMandatory() const
    {
        if (coins() >= 10)
            throw std::invalid_argument("You must coup if you have 10 or more coins");
    }

    /**
     *  Performs the 'gather' action -> gain 1 coin.
     * @throws ---> std::invalid_argument If it's not the player's turn, or coup is required.
     */
    void Player::gather()
    {
        if (sanctionGather)
            throw std::invalid_argument("You are sanctioned from using gather");
        if (!game.isPlayerTurn(*this))
            throw std::invalid_argument("Not " + GetRole() + "'s turn");
        checkCoupMandatory();
        AddCoins(1);
        lastAction = "gather";
        game.advanceTurn();
    }

    /**
     * Performs the 'tax' action -> gain 2 coins (default).
     * @throws ---> std::invalid_argument If it's not the player's turn, or coup is required.
     */
    void Player::tax()
    {
        if (!game.isPlayerTurn(*this))
            throw std::invalid_argument("Not your turn");

        if (sanctionTax)
        {
            throw std::invalid_argument(GetName() + " is sanctioned and cannot use tax");
            return;
        }

        checkCoupMandatory();
        AddCoins(2);
        lastAction = "tax";
        game.advanceTurn();
    }
    /**
     * Performs the 'bribe' action -> pay 4 coins and activate bribe status.
     * @throws ---> std::invalid_argument If it's not the player's turn or coup is required.
     */
    void Player::bribe()
    {
        if (!game.isPlayerTurn(*this))
            throw std::invalid_argument("Not your turn");

        checkCoupMandatory();
        DecreaseCoins(4);
        ActivateBribeStatus();
        game.addExtraTurns(this->GetName(), 1);
        lastAction = "bribe";
    }

    /**
     * Performs an arrest on the target player.
     * Cannot arrest the same player twice in a row.
     * @param target ---> The player to arrest.
     * @throws ---> std::invalid_argument If not your turn, coup is required, or arresting same player twice.
     */

    void Player::arrest(Player &target)
    {
        if (!game.isPlayerTurn(*this))
            throw std::invalid_argument("Not " + GetName() + "'s turn");

        if (blockarrestturn)
            throw std::invalid_argument(GetName() + " is temporarily blocked from arresting this turn");

        checkCoupMandatory();

        if (game.getLastArrestedVictim() == &target)
            throw std::invalid_argument("This player was just arrested. You can't arrest the same target twice in a row.");

        if (target.GetRole() == "Merchant")
        {
            target.DecreaseCoins(2);
            std::cout << target.GetName() << " is a Merchant and pays 2 coins (no reward).\n";
        }
        else if (target.GetRole() == "General")
        {
            target.DecreaseCoins(1);
            AddCoins(1);
            target.AddCoins(1);
        }
        else
        {
            target.DecreaseCoins(1);
            AddCoins(1);
        }

        lastArrestedTarget = &target;
        game.setLastArrestedVictim(&target);
        lastAction = "arrest";

        game.advanceTurn();
    }

    /**
     *  Applies a sanction to the target player (blocks gather/tax).
     * Costs 3 coins.
     * Calls target's `onSanction()` hook for role-specific behavior.
     * @param target ---> The player to sanction.
     * @throws---> std::invalid_argument If not your turn or coup is required.
     */
    void Player::sanction(Player &target, const std::string &type)
    {
        if (!game.isPlayerTurn(*this))
            throw std::invalid_argument("Not your turn");

        checkCoupMandatory();
        DecreaseCoins(3);
        target.ActivateSanction(type);
        target.onSanction();
        if (target.GetRole() == "Judge")
        {
            std::cout << GetName() << " sanctioned a Judge and loses 1 extra coin!\n";
            DecreaseCoins(1);
        }

        lastAction = "sanction";
        game.advanceTurn();
    }
    /**
     *  Performs a coup on the target player (eliminates them).
     * Costs 7 coins.
     * @param target ---> The player to eliminate.
     * @throws ---> std::invalid_argument If not your turn or you don't have enough coins.
     */
    void Player::coup(Player &target)
    {
        if (!game.isPlayerTurn(*this))
            throw std::invalid_argument("Not your turn");

        if (coins() < 7)
            throw std::invalid_argument("You can't coup the player");

        DecreaseCoins(7);
        target.eliminated();
        lastAction = "coup";

        game.advanceTurn();

        if (!target.Getstillingame())
        {
            std::cout << name << " made a coup on " << target.GetName() << std::endl;
        }
    }

    /**
     * @Sets the name of the last action the player performed.
     * Used for block/challenge logic.
     * @param action ---> The action name.
     */
    void Player::SetLastAction(const std::string &action)
    {
        lastAction = action;
    }

    /**
     *  Retrieves the last action performed by the player.
     * @return --->  A string indicating the action.
     */
    std::string Player::GetLastAction() const
    {
        return lastAction;
    }

    /**
     *  called when the player is sanctioned.
     * Default is empty,  override in roles to implement custom logic.
     */
    void Player::onSanction()
    {
    }

    /**
     * Resets the internal state specific to the player's role.
     * This base implementation does nothing.
     * Derived classes can override this method to reset role-specific .
     */
    void Player::resetRoleState()
    {
    }

}
