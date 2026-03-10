#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct RankEntry {
    string name;
    int score;
};

class Player {
protected:
    string name;
public:
    Player(string n);
    virtual int choose();
    int choose(int c);
    string getName() { return name; }
};

Player::Player(string n) {
    name = n;
}

int Player::choose() {
    char c;
    while (true) {
        cout << "Choose: 1 = Rock, 2 = Scissors, 3 = Paper (0 = Exit) : ";
        cin >> c;

        // Block if more than 1 character was typed
        if (cin.peek() != '\n') {
            cout << "Error: Please enter only ONE character\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (c == '0') return 0;
        if (c == '1') return 1;
        if (c == '2') return 2;
        if (c == '3') return 3;

        cout << "Error: Number must be 0, 1, 2, or 3\n";
    }
}

int Player::choose(int c) {
    return c;
}

class Bot : public Player {
public:
    Bot(string n) : Player(n) {}
    int choose();
};

int Bot::choose() {
    return rand() % 3 + 1;
}

class Rank {
private:
    vector<RankEntry> rankList;
    string filename = "rank.txt";
public:
    void loadRank();
    void saveRank();
    void addScore(string name, int score);
    void showRank();
};

void Rank::loadRank() {
    ifstream file(filename);
    if (!file.is_open()) return;
    RankEntry entry;
    while (file >> entry.name >> entry.score) {
        rankList.push_back(entry);
    }
    file.close();
}

void Rank::saveRank() {
    ofstream file(filename);
    for (auto& e : rankList) {
        file << e.name << " " << e.score << "\n";
    }
    file.close();
}

void Rank::addScore(string name, int score) {
    for (auto& e : rankList) {
        if (e.name == name) {
            if (score > e.score) e.score = score;
            return;
        }
    }
    rankList.push_back({name, score});
}

void Rank::showRank() {
    sort(rankList.begin(), rankList.end(), [](RankEntry a, RankEntry b) {
        return a.score > b.score;
    });
    cout << "\n===== RANKING =====\n";
    int place = 1;
    for (auto& e : rankList) {
        cout << place++ << ". " << e.name << " - " << e.score << " pts\n";
    }
    if (rankList.empty()) cout << "No records yet.\n";
    cout << "===================\n";
}

class Game {
private:
    Player *player;
    Bot *bot;
    int scorePlayer = 0;
    int scoreBot = 0;
    bool exited = false;
public:
    Game(Player *p, Bot *b);
    void playRound(int round);
    void showResult(Rank &rank);
    int checkWin(int p, int b);
    bool isExited() { return exited; }
};

Game::Game(Player *p, Bot *b) {
    player = p;
    bot = b;
}

string convertChoice(int c) {
    if (c == 1) return "Rock";
    if (c == 2) return "Scissors";
    if (c == 3) return "Paper";
    return "";
}

void Game::playRound(int round) {
    cout << "\n----- Round " << round << " -----\n";
    int p = player->choose();
    if (p == 0) {
        cout << "You exited the game.\n";
        exited = true;
        return;
    }
    int b = bot->choose();
    cout << "You chose: " << convertChoice(p) << endl;
    cout << "Bot chose: " << convertChoice(b) << endl;
    int result = checkWin(p, b);
    if (result == 1) {
        cout << "You win this round!\n";
        scorePlayer++;
    } else if (result == 2) {
        cout << "Bot wins this round!\n";
        scoreBot++;
    } else {
        cout << "Draw!\n";
    }
}

int Game::checkWin(int p, int b) {
    if (p == b) return 0;
    if ((p == 1 && b == 2) ||
        (p == 2 && b == 3) ||
        (p == 3 && b == 1))
        return 1;
    return 2;
}

void Game::showResult(Rank &rank) {
    cout << "\n===== FINAL RESULT =====\n";
    cout << "Player Score: " << scorePlayer << endl;
    cout << "Bot Score: " << scoreBot << endl;

    if (scorePlayer > scoreBot)
        cout << "You win the game!\n";
    else if (scoreBot > scorePlayer)
        cout << "Bot wins the game!\n";
    else
        cout << "Game Draw!\n";

    rank.addScore(player->getName(), scorePlayer);
    rank.saveRank();
    rank.showRank();
}

int main() {
    srand(time(0));

    Rank rank;
    rank.loadRank();

    while (true) {
        cout << "\n===== MENU =====\n";
        cout << "1. Play Game\n";
        cout << "2. Show Ranking\n";
        cout << "3. Exit\n";
        cout << "Choose: ";

        char menu;
        cin >> menu;

        // Block multi-character menu input too
        if (cin.peek() != '\n') {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: Please enter only ONE character\n";
            continue;
        }

        if (menu == '3') {
            cout << "Goodbye!\n";
            break;
        } else if (menu == '2') {
            rank.showRank();
        } else if (menu == '1') {
            string playerName;
            cout << "Enter your name: ";
            cin >> playerName;
            cin.ignore(1000, '\n');

            Player p(playerName);
            Bot b("Bot");
            Game game(&p, &b);

            for (int i = 1; i <= 3; i++) {
                game.playRound(i);
                if (game.isExited()) break;
            }

            if (!game.isExited()) {
                game.showResult(rank);
            }
        }
    }

    return 0;
}
