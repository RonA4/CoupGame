// ronamsalem4@gmail.com
#include "Game.hpp"
#include "Player.hpp"

namespace coup
{
    /**
     * Constructs a new Game object.
     * Initializes turn index to 0 and sets the game as not started.
     */
    Game::Game() : index(0), startGame(false) {}

    /**
     * Adds a player to the game before it starts.
     * If the number of players reaches 2 or more, the game is marked as started.
     * @param player ---> Pointer to the Player to add.
     * @throws ---> invalid_argument if there are already 6 players.
     */
    void Game::addPlayer(Player *player)
    {
        if (list_players.size() >= 6)
            throw invalid_argument("Maximum 6 players allowed.");
        list_players.push_back(player);
        if (list_players.size() >= 2)
            startGame = true;
    }

    /**
     * Eliminates a player from the game.
     * Marks the player as inactive. Eliminated players are skipped during turns.
     * @param player  ---> Pointer to the player to eliminate.
     * @throws ---> invalid_argument if the player is already eliminated.
     */
    void Game::eliminatePlayer(Player *player)
    {
        if (!player->Getstillingame())
            throw invalid_argument("The player is already eliminated!");
        player->eliminated();
    }

    /**
     * Returns a previously eliminated player to the game.
     * Useful for undo logic or testing.
     * @param player ---> Pointer to the player to return.
     * @throws ---> invalid_argument if the player is already active.
     */
    void Game::returnPlayer(Player *player)
    {
        if (player->Getstillingame())
        {
            throw invalid_argument("The player is still in the game!");
        }

        player->returnToGame();
        size_t restoredIndex = 0;
        for (size_t i = 0; i < list_players.size(); ++i)
        {
            if (list_players[i] == player)
            {
                restoredIndex = i;
                break;
            }
        }

        index = (restoredIndex + list_players.size() - 1) % list_players.size();
    }

    /**
     * Checks if it's currently the given player's turn.
     * @param player ---> Reference to the player to check.
     * @return ---> true if it's their turn, false otherwise.
     */
    bool Game::isPlayerTurn(const Player &player) const
    {
        return list_players.at(index)->GetName() == player.GetName();
    }

    /**
     * Advances the turn to the next active player.
     * Skips eliminated players. Resets temporary status effects from the previous turn.
     */
    void Game::advanceTurn()
    {
        if (!hasEnoughPlayers())
            throw invalid_argument("At least 2 players required.");
        Player *current = list_players.at(index);

        // reset
        current->resetBribeStatus();
        current->resetSanction();
        current->resetblockarrestturn();
        current->resetRoleState();

        if (current->GetLastAction() != "arrest")
        {
            resetLastArrestedVictim();
        }

        if (hasExtraTurn(current->GetName()))
        {
            useExtraTurn(current->GetName());
            return;
        }

        size_t originalIndex = index;
        do
        {
            index = (index + 1) % list_players.size();
        } while (!list_players.at(index)->Getstillingame() && index != originalIndex);
    }

    /**
     * Returns a list of names of all players still in the game.
     * @return ---> Vector of strings with active player names.
     */
    vector<string> Game::players() const
    {
        vector<string> names;
        for (Player *p : list_players)
            if (p->Getstillingame())
                names.push_back(p->GetName());
        return names;
    }

    /**
     * @return ---> The name of the current player.
     */
    string Game::turn() const
    {
        return list_players.at(index)->GetName();
    }

    /**
     * Returns the name of the winner, if only one player remains.
     * @return ---> The winner's name.
     * @throws ---> invalid_argument if there is no winner yet or game hasn't started.
     */
    string Game::winner() const
    {
        if (!hasEnoughPlayers())
            throw invalid_argument("At least 2 players required.");
        int aliveCount = 0;
        Player *potentialWinner = nullptr;
        for (Player *p : list_players)
        {
            if (p->Getstillingame())
            {
                aliveCount++;
                potentialWinner = p;
            }
        }
        if (aliveCount == 1 && startGame)
        {
            return potentialWinner->GetName();
        }
        throw invalid_argument("No winner yet!");
    }

    /**
     * Adds extra turns to the given player.
     * Increments the number of extra turns the specified player can take.
     * Typically used after actions like bribe.
     * @param playerName ---> The name of the player.
     * @param count ---> Number of extra turns to add.
     */
    void Game::addExtraTurns(const std::string &playerName, int count)
    {
        extra_turns[playerName] += count;
    }

    bool Game::hasExtraTurn(const std::string &playerName) const
    {
        auto it = extra_turns.find(playerName);
        return it != extra_turns.end() && it->second > 0;
    }

    /**
     * Checks if the player has any extra turns left.
     *
     * @param playerName ---> The name of the player.
     * @return ---> true if the player has at least one extra turn, false otherwise.
     */
    void Game::useExtraTurn(const std::string &playerName)
    {
        if (hasExtraTurn(playerName))
        {
            extra_turns.at(playerName)--;
            if (extra_turns.at(playerName) == 0)
            {
                extra_turns.erase(playerName);
            }
        }
    }

    /**
     * Consumes one of the player's extra turns, if any.
     * If the player has extra turns, one will be removed.
     * If no extra turns remain, the entry is erased.
     * @param playerName --->  The name of the player.
     */
    void Game::removeExtraTurns(const std::string &playerName)
    {
        extra_turns.erase(playerName);
    }

    /**
     * Sets the last player who was arrested.
     * Used for logic that prevents immediate re-arresting.
     * @param player ---> Pointer to the arrested player.
     */
    void Game::setLastArrestedVictim(Player *player)
    {
        lastArrestedVictim = player;
    }

    /**
     *  Retrieves the last player who was arrested.
     * @return ---> Pointer to the arrested player, or nullptr if none.
     */
    Player *Game::getLastArrestedVictim() const
    {
        return lastArrestedVictim;
    }

    /**
     * Clears the record of the last arrested player.
     * Typically called after the arrest lock period ends.
     */
    void Game::resetLastArrestedVictim()
    {
        lastArrestedVictim = nullptr;
    }

    /**
     * Checks if the game has at least two players.
     * Ensures the game can legally start or continue.
     * @return ---> true if player count >= 2, false otherwise.
     */
    bool Game::hasEnoughPlayers() const
    {
        return list_players.size() >= 2;
    }
}
