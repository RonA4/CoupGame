#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <set>
#include "../game/Game.hpp"
#include "../roles/Governor.hpp"
#include "../roles/Spy.hpp"
#include "../roles/Baron.hpp"
#include "../roles/General.hpp"
#include "../roles/Judge.hpp"
#include "Merchant.hpp"
/**
 *  Graphical User Interface for the Coup strategy game.
 * This file implements the main graphical interface of the game using the SFML library.
 * It presents a window with interactive elements (buttons, player boxes, and status text),
 * allowing players to perform actions such as gather, tax, bribe, arrest, coup, etc.,
 * using a point-and-click GUI rather than the console.
 * The interface supports visual feedback for:
 * - Player turns and actions
 * - Role-based abilities and interactions
 * - Real-time game state visualization
 * - Rule enforcement (e.g. blocking, sanctions, extra turns)
 * It also includes logic for handling user input (mouse clicks), displaying win popups,
 * and coordinating GUI elements with the underlying game logic.
 *A player's choice of whether to block any action is made through the terminal.
 */

using namespace coup;
using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 900;

Player *lastArrestedBy = nullptr;
Player *lastArrestedTarget = nullptr;

string lastPlayerWatchedCoins = "";
set<string> arrestedBanSet;

string lastTurnPlayer = "";
int turnCounter = 0;
map<string, pair<string, int>> blockedActions; // player -> (action, unblock turn)

struct GUIPlayer
{
    Player *logic;
    sf::RectangleShape box;
    sf::Text nameText, roleText, coinText;
    bool showCoins = false;
};

GUIPlayer createGUIPlayer(Player *player, sf::Font &font, float x, float y, const sf::Color &color)
{
    GUIPlayer gui;
    gui.logic = player;
    gui.box.setSize(sf::Vector2f(200, 90));
    gui.box.setPosition(x, y);
    gui.box.setFillColor(color);
    gui.box.setOutlineColor(sf::Color::Black);
    gui.box.setOutlineThickness(2);
    gui.nameText.setFont(font);
    gui.nameText.setCharacterSize(18);
    gui.nameText.setFillColor(sf::Color::Black);
    gui.nameText.setString(player->GetName());
    gui.nameText.setPosition(x + 10, y + 10);
    gui.roleText.setFont(font);
    gui.roleText.setCharacterSize(16);
    gui.roleText.setFillColor(sf::Color(70, 70, 200));
    gui.roleText.setString("Role: " + player->GetRole());
    gui.roleText.setPosition(x + 10, y + 35);
    gui.coinText.setFont(font);
    gui.coinText.setCharacterSize(16);
    gui.coinText.setFillColor(sf::Color::Green);
    gui.coinText.setPosition(x + 10, y + 60);
    return gui;
}

sf::RectangleShape createButton(float x, float y, const string &label, sf::Font &font, sf::Text &outText)
{
    sf::RectangleShape button(sf::Vector2f(140, 40));
    button.setPosition(x, y);
    button.setFillColor(sf::Color(215, 235, 255));
    button.setOutlineColor(sf::Color(160, 160, 160));
    button.setOutlineThickness(1);
    outText.setFont(font);
    outText.setString(label);
    outText.setCharacterSize(18);
    outText.setFillColor(sf::Color::Black);
    outText.setPosition(x + 10, y + 8);
    return button;
}

void printGameState(const vector<Player *> &players)
{
    cout << "====================\n";
    for (const auto &p : players)
    {
        if (!p->Getstillingame())
            continue;
        cout << p->GetName() << " (" << p->GetRole() << ") - Coins: " << p->coins() << endl;
    }
    cout << "====================\n"
         << endl;
}

void showWinnerPopup(sf::RenderWindow &window, const std::string &winner, sf::Font &font)
{
    sf::RectangleShape popup(sf::Vector2f(500, 200));
    popup.setFillColor(sf::Color(255, 255, 255));
    popup.setOutlineColor(sf::Color::Red);
    popup.setOutlineThickness(5);
    popup.setPosition(WIDTH / 2 - 250, HEIGHT / 2 - 100);

    sf::Text title("\xF0\x9F\x8E\x89 WINNER \xF0\x9F\x8E\x89", font, 36);
    title.setFillColor(sf::Color::Red);
    title.setStyle(sf::Text::Bold);
    title.setPosition(WIDTH / 2 - 110, HEIGHT / 2 - 90);

    sf::Text name(winner + " wins the game!", font, 26);
    name.setFillColor(sf::Color::Black);
    name.setPosition(WIDTH / 2 - 160, HEIGHT / 2 - 20);

    sf::Text hint("Click anywhere to close", font, 18);
    hint.setFillColor(sf::Color(100, 100, 100));
    hint.setPosition(WIDTH / 2 - 100, HEIGHT / 2 + 60);

    window.draw(popup);
    window.draw(title);
    window.draw(name);
    window.draw(hint);
}

bool askGeneralToBlockCoup(Player *general, const string &targetName)
{
    if (general->coins() < 5)
    {
        return false;
    }
    cout << "General (" << general->GetName() << ") – do you want to block the coup on " << targetName << "? (y/n): ";
    string response;
    cin >> response;
    return (response == "y" || response == "Y");
}

bool askGovernorToBlockTax(Player *governor, const string &playerName)
{
    cout << "Governor (" << governor->GetName() << ") – do you want to block the tax from " << playerName << "? (y/n): ";
    string response;
    cin >> response;
    return (response == "y" || response == "Y");
}

bool askJudgeToBlockBribe(Player *judge, const string &briberName)
{
    cout << "Judge (" << judge->GetName() << ") – do you want to block the bribe from " << briberName << "? (y/n): ";
    string response;
    cin >> response;
    return (response == "y" || response == "Y");
}

void handleMerchantArrested(Player *arrestedPlayer)
{
    if (arrestedPlayer->GetRole() == "Merchant")
    {
        Merchant *merchant = dynamic_cast<Merchant *>(arrestedPlayer);
        if (merchant != nullptr)
        {
            merchant->getArrested();
        }
    }
}

void advanceTurn(Game &game)
{
    ++turnCounter;
    lastArrestedTarget = nullptr; // reset arrested target
    for (auto it = blockedActions.begin(); it != blockedActions.end();)
    {
        if (it->second.second <= turnCounter)
        {
            cout << "[DEBUG] Unblocking " << it->first << " from action '" << it->second.first << "' at turn " << turnCounter << endl;
            it = blockedActions.erase(it);
        }
        else
        {
            ++it;
        }
    }
    game.advanceTurn();
}

int main()
{
    Game game;
    Governor governor(game, "Moshe");
    Spy spy(game, "Yossi");
    Baron baron(game, "Meirav");
    General general(game, "Reut");
    Judge judge(game, "Gilad");
    Merchant merchant(game, "Dana");

    vector<Player *> players = {&governor, &spy, &baron, &general, &judge, &merchant};

    vector<sf::Color> pastelColors = {
        sf::Color(255, 204, 204),
        sf::Color(204, 255, 229),
        sf::Color(255, 255, 204),
        sf::Color(204, 229, 255),
        sf::Color(229, 204, 255),
        sf::Color(255, 229, 204)};

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Coup - Game View");
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"))
    {
        cerr << "Font load error\n";
        return 1;
    }

    vector<GUIPlayer> guiPlayers;
    float baseX = 80, baseY = 200;
    float spacing = 230;
    for (size_t i = 0; i < players.size(); ++i)
    {
        float rowX = baseX + (i % 3) * spacing;
        float rowY = baseY + (i / 3) * 150;
        guiPlayers.push_back(createGUIPlayer(players[i], font, rowX, rowY, pastelColors[i % pastelColors.size()]));
    }

    sf::Text turnText, logText;
    turnText.setFont(font);
    turnText.setCharacterSize(28);
    turnText.setFillColor(sf::Color(120, 80, 160));
    turnText.setPosition(50, 30);

    logText.setFont(font);
    logText.setCharacterSize(20);
    logText.setFillColor(sf::Color(80, 80, 100));
    logText.setPosition(50, HEIGHT - 40);

    vector<string> actions = {"gather", "tax", "bribe", "arrest", "sanction", "coup", "watch", "invest"};
    vector<sf::RectangleShape> buttons;
    vector<sf::Text> buttonLabels;

    for (size_t i = 0; i < actions.size(); ++i)
    {
        sf::Text lbl;
        sf::RectangleShape btn = createButton(WIDTH - 220, 100 + i * 50, actions[i], font, lbl);
        buttons.push_back(btn);
        buttonLabels.push_back(lbl);
    }

    string log = "";
    Player *pendingTargetAction = nullptr;
    string pendingAction = "";
    string winnerName = "";
    bool showPopup = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (showPopup)
                {
                    window.close();
                    continue;
                }

                sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

                if (!pendingAction.empty())
                {
                    for (auto &gp : guiPlayers)
                    {
                        if (!gp.logic->Getstillingame())
                            continue;
                        if (gp.box.getGlobalBounds().contains(mouse) && gp.logic->GetName() != game.turn())
                        {
                            try
                            {
                                Player *current = nullptr;
                                for (auto *p : players)
                                    if (p->GetName() == game.turn())
                                        current = p;

                                if (pendingAction == "coup")
                                {
                                    if (current->coins() < 7)
                                    {
                                        log = current->GetName() + " has less than 7 coins and cannot coup.";
                                        cout << log << endl;
                                        logText.setString(log);
                                    }
                                    else if (askGeneralToBlockCoup(&general, gp.logic->GetName()))
                                    {
                                        if (general.coins() >= 5)
                                        {

                                            general.DecreaseCoins(5);
                                            current->DecreaseCoins(7);
                                            log = "General blocked the coup on " + gp.logic->GetName();
                                            logText.setString(log);
                                            cout << log << endl;
                                            advanceTurn(game);
                                        }
                                        else
                                        {
                                            log = "General tried to block but doesn't have enough coins. Skipping block.";
                                            logText.setString(log);
                                            cout << log << endl;
                                            current->coup(*gp.logic);
                                            game.advanceTurn();
                                        }
                                    }
                                    else
                                    {
                                        current->coup(*gp.logic);
                                        log = current->GetName() + " used coup on " + gp.logic->GetName();
                                        logText.setString(log);
                                        cout << log << endl;
                                    }
                                }
                                else if (pendingAction == "tax")
                                {
                                    Player *governorPtr = nullptr;
                                    for (auto *p : players)
                                    {
                                        if (p->GetRole() == "Governor" && p->Getstillingame() && p != current)
                                        {
                                            governorPtr = p;
                                            break;
                                        }
                                    }
                                    if (governorPtr != nullptr)
                                    {
                                        if (askGovernorToBlockTax(governorPtr, current->GetName()))
                                        {
                                            log = "Governor blocked tax by " + current->GetName();
                                            logText.setString(log);
                                            cout << log << endl;
                                            game.advanceTurn();
                                            pendingAction = ""; // קודם מאפסים
                                            game.advanceTurn(); // ואז עוברים תור
                                            break;              // מסיימים את הטיפול הנוכחי
                                        }
                                        else
                                        {
                                            current->tax();
                                            log = current->GetName() + " used tax";
                                            logText.setString(log);
                                            cout << log << endl;
                                            pendingAction = ""; // במקרה שהטקס מצליח, גם כן לא לשכוח
                                        }
                                    }

                                    else
                                    {
                                        current->tax();
                                        log = current->GetName() + " used tax";
                                        logText.setString(log);
                                        cout << log << endl;
                                    }
                                }
                                else if (pendingAction == "watch")
                                {
                                    if (current->GetRole() == "Spy")
                                    {
                                        log = gp.logic->GetName() + " has " + to_string(gp.logic->coins()) + " coins";
                                        for (auto &g : guiPlayers)
                                            if (g.logic == gp.logic)
                                                g.showCoins = true;

                                        // הוספה לסט איסור arrest לתור הבא
                                        arrestedBanSet.insert(gp.logic->GetName());

                                        lastPlayerWatchedCoins = gp.logic->GetName();
                                    }
                                    else
                                    {
                                        log = current->GetName() + " cannot perform watch: not a Spy";
                                    }
                                    logText.setString(log);
                                    cout << log << endl;
                                }
                                else if (pendingAction == "arrest")
                                {
                                    // בדיקה אם השחקן כבר נעצר לאחרונה
                                    if (lastArrestedTarget && gp.logic->GetName() == lastArrestedTarget->GetName())
                                    {
                                        log = gp.logic->GetName() + " was recently arrested and cannot be arrested again immediately.";
                                        logText.setString(log);
                                        cout << log << endl;
                                        pendingAction = ""; // ← זה השורה החשובה!
                                        break;
                                    }
                                    if (arrestedBanSet.find(current->GetName()) != arrestedBanSet.end())
                                    {
                                        log = current->GetName() + " cannot perform arrest this turn because someone watched their coins last turn.";
                                        cout << log << endl;
                                        logText.setString(log);
                                        cout << log << endl;
                                        // הסרת האיסור כדי שיפוג בתור הבא
                                        arrestedBanSet.erase(current->GetName());
                                        break;
                                    }
                                    else
                                    {
                                        current->arrest(*gp.logic);
                                        lastArrestedBy = current;
                                        lastArrestedTarget = gp.logic;
                                        handleMerchantArrested(gp.logic);
                                        if (gp.logic->GetRole() == "General")
                                        {
                                            General *gen = dynamic_cast<General *>(gp.logic);
                                            if (gen != nullptr)
                                            {
                                                gen->Gotarrested();
                                                log += " | General received 1 coin back due to arrest.";
                                                logText.setString(log);
                                            }
                                        }
                                    }
                                }
                                else if (pendingAction == "sanction")
                                {
                                    log = "Choose which action to block for " + gp.logic->GetName() + " (gather/tax): ";
                                    logText.setString(log);
                                    cout << log;
                                    string chosenBlock;
                                    cin >> chosenBlock;
                                    if (chosenBlock == "gather" || chosenBlock == "tax")
                                    {
                                        current->DecreaseCoins(3);
                                        blockedActions[gp.logic->GetName()] = {chosenBlock, turnCounter + 1};

                                        // Baron compensation
                                        if (gp.logic->GetRole() == "Baron")
                                        {
                                            Baron *baronTarget = dynamic_cast<Baron *>(gp.logic);
                                            if (baronTarget)
                                            {
                                                baronTarget->onSanction();
                                                log += " | Baron received 1 coin compensation.";
                                            }
                                        }

                                        // Judge penalty
                                        if (gp.logic->GetRole() == "Judge")
                                        {
                                            Judge *judgeTarget = dynamic_cast<Judge *>(gp.logic);
                                            if (judgeTarget)
                                            {
                                                judgeTarget->gotSanctioned(*current);
                                                log += " | Judge triggered extra penalty: attacker pays 1 coin to the bank.";
                                            }
                                        }

                                        log = current->GetName() + " used sanction on " + gp.logic->GetName() + ", blocking " + chosenBlock + " until the end of their next turn";
                                        logText.setString(log);
                                        cout << log << endl;
                                    }
                                    else
                                    {
                                        log = "Invalid action to block. Sanction cancelled.";
                                        logText.setString(log);
                                        cout << log << endl;
                                    }
                                    pendingAction = "";
                                    game.advanceTurn();
                                    if (gp.logic->GetRole() == "Judge")
                                    {
                                        Judge *judge = dynamic_cast<Judge *>(gp.logic);
                                        if (judge != nullptr)
                                        {
                                            log += " | Judge triggered extra penalty: attacker pays 1 coin to the bank.";
                                            logText.setString(log);
                                            log += " | Judge triggered extra penalty: attacker pays 1 coin to the bank.";
                                            logText.setString(log);
                                        }
                                    }
                                }
                                printGameState(players);
                            }
                            catch (const exception &e)
                            {
                                log = string("[Error] ") + e.what();
                                logText.setString(log);
                                cout << log << endl;
                            }
                            pendingAction = "";
                        }
                    }
                }
                else
                {
                    for (size_t i = 0; i < buttons.size(); ++i)
                    {
                        if (buttons[i].getGlobalBounds().contains(mouse))
                        {
                            string action = actions[i];
                            Player *current = nullptr;
                            for (auto *p : players)
                                if (p->GetName() == game.turn())
                                    current = p;

                            try
                            {
                                // --- איפוס איסור arrest בתחילת תור חדש ---
                                string currentTurn = game.turn();
                                if (currentTurn != lastTurnPlayer)
                                {
                                    lastTurnPlayer = currentTurn;
                                    for (auto it = arrestedBanSet.begin(); it != arrestedBanSet.end();)
                                    {
                                        if (*it == currentTurn)
                                        {
                                            ++it; // נשאר בתור
                                        }
                                        else
                                        {
                                            it = arrestedBanSet.erase(it); // מוחקים אחרים
                                        }
                                    }
                                    // לא מוחקים ישירות את השם כי רק מי שצפו עליו יישאר בסט
                                    // מוחקים רק את השמות של כל השחקנים שלא תורם כעת
                                    for (auto it = arrestedBanSet.begin(); it != arrestedBanSet.end();)
                                    {
                                        if (*it == currentTurn)
                                        {
                                            ++it; // נשאר בתור
                                        }
                                        else
                                        {
                                            it = arrestedBanSet.erase(it); // מוחקים אחרים
                                        }
                                    }
                                    lastTurnPlayer = currentTurn;
                                }

                                if (current->GetName() == lastPlayerWatchedCoins && action == "arrest")
                                {
                                    log = current->GetName() + " cannot perform arrest this turn because someone watched their coins last turn.";
                                    cout << log << endl;
                                    logText.setString(log);
                                    lastPlayerWatchedCoins = ""; // שחרור חסימה לאחר תור אחד
                                    break;
                                }
                                if (current->coins() >= 10 && action != "coup")
                                {
                                    log = current->GetName() + " has less than 10 coins and cannot coup.";
                                    cout << log << endl;
                                    logText.setString(log);
                                    cout << log << endl;
                                    break;
                                }

                                if (action == "gather")
                                {
                                    if (blockedActions.count(current->GetName()) && blockedActions[current->GetName()].first == "gather")
                                    {
                                        log = current->GetName() + " is blocked from performing gather due to sanction.";
                                        logText.setString(log);
                                        cout << log << endl;
                                        // Remove one-time block
                                        blockedActions.erase(current->GetName());
                                        break;
                                    }
                                    current->gather();
                                    log = current->GetName() + " used gather";
                                    logText.setString(log);
                                    cout << log << endl;
                                }
                                else if (action == "tax")
                                {
                                    if (blockedActions.count(current->GetName()) && blockedActions[current->GetName()].first == "tax")
                                    {
                                        log = current->GetName() + " is blocked from performing tax due to sanction.";
                                        logText.setString(log);
                                        cout << log << endl;
                                        // Remove one-time block
                                        blockedActions.erase(current->GetName());
                                        break;
                                    }
                                    Player *governorPtr = nullptr;
                                    for (auto *p : players)
                                    {
                                        if (p->GetRole() == "Governor" && p->Getstillingame() && p != current)
                                        {
                                            governorPtr = p;
                                            break;
                                        }
                                    }
                                    if (governorPtr != nullptr)
                                    {
                                        if (askGovernorToBlockTax(governorPtr, current->GetName()))
                                        {
                                            log = "Governor blocked tax by " + current->GetName();
                                            logText.setString(log);
                                            cout << log << endl;
                                            game.advanceTurn();
                                        }
                                        else
                                        {
                                            current->tax();
                                            log = current->GetName() + " used tax";
                                            logText.setString(log);
                                            cout << log << endl;
                                        }
                                    }
                                    else
                                    {
                                        current->tax();
                                        log = current->GetName() + " used tax";
                                        logText.setString(log);
                                        cout << log << endl;
                                    }
                                }
                                else if (action == "bribe")
                                {
                                    bool blocked = false;
                                    for (auto *p : players)
                                    {
                                        if (p->GetRole() == "Judge" && p->Getstillingame())
                                        {
                                            Judge *judge = dynamic_cast<Judge *>(p);
                                            if (judge && askJudgeToBlockBribe(judge, current->GetName()))
                                            {
                                                log = "Judge blocked the bribe from " + current->GetName();
                                                logText.setString(log);
                                                cout << log << endl;
                                                current->DecreaseCoins(4);
                                                game.advanceTurn();
                                                pendingAction = "";
                                                blocked = true;
                                                break;
                                            }
                                        }
                                    }
                                    if (!blocked)
                                    {
                                        try
                                        {
                                            current->bribe();
                                            if (current->GetRole() == "Merchant")
                                            {
                                                log += " | Merchant received 1 bonus coin.";
                                            }
                                            log = current->GetName() + " used bribe and gets another turn.";
                                            logText.setString(log);
                                            cout << log << endl;
                                            printGameState(players);
                                        }
                                        catch (const std::exception &e)
                                        {
                                            log = string("[Error] ") + e.what();
                                            logText.setString(log);
                                            cout << log << endl;
                                        }
                                        pendingAction = "";
                                    }
                                }
                                else if (action == "invest")
                                {
                                    Baron *baron = dynamic_cast<Baron *>(current);
                                    if (baron != nullptr)
                                    {
                                        try
                                        {
                                            baron->invest();
                                            log = baron->GetName() + " used invest";
                                            logText.setString(log);
                                            cout << log << endl;
                                        }
                                        catch (const std::exception &e)
                                        {
                                            log = string("[Error] ") + e.what();
                                            logText.setString(log);
                                            cout << log << endl;
                                        }
                                    }
                                    else
                                    {
                                        log = current->GetName() + " cannot invest: not a Baron";
                                        logText.setString(log);
                                        cout << log << endl;
                                    }
                                }
                                else
                                {
                                    pendingAction = action;
                                    log = "Select a player for action: " + action;
                                    logText.setString(log);
                                    cout << log << endl;
                                }

                                if (action != "watch")
                                {
                                    for (auto &g : guiPlayers)
                                        g.showCoins = false;
                                }

                                if (action == "gather")
                                {
                                    log = current->GetName() + " used " + action;
                                    logText.setString(log);
                                    cout << log << endl;
                                }

                                printGameState(players);
                            }
                            catch (const exception &e)
                            {
                                log = string("[Error] ") + e.what();
                                logText.setString(log);
                                cout << log << endl;
                            }
                        }
                    }
                }
            }
        }

        for (auto *p : players)
        {
            Baron *baron = dynamic_cast<Baron *>(p);
            if (baron)
            {
                baron->resetInvestFlag();
            }
        }

        try
        {
            winnerName = game.winner();
            showPopup = true;
        }
        catch (...)
        {
        }

        turnText.setString("Current Player: " + game.turn());

        window.clear(sf::Color(255, 239, 239));
        window.draw(turnText);

        for (auto &gp : guiPlayers)
        {
            if (!gp.logic->Getstillingame())
                continue;
            gp.roleText.setString("Role: " + gp.logic->GetRole());
            if (gp.showCoins)
                gp.coinText.setString("Coins: " + to_string(gp.logic->coins()));
            else
                gp.coinText.setString("");

            window.draw(gp.box);
            window.draw(gp.nameText);
            window.draw(gp.roleText);
            if (gp.showCoins)
                window.draw(gp.coinText);
        }

        for (size_t i = 0; i < buttons.size(); ++i)
        {
            window.draw(buttons[i]);
            window.draw(buttonLabels[i]);
        }

        window.draw(logText);

        if (showPopup && !winnerName.empty())
        {
            showWinnerPopup(window, winnerName, font);
        }

        window.display();
    }

    return 0;
}
