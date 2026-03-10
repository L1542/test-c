// เกม เป่า ยิง ฉุบ | Project 01418113
// ไฟล์: users.txt  รูปแบบ → username score

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <string>
#include <windows.h>
using namespace std;

// ── CLASS 1 : Player (Abstract Base Class) ──────────
class Player {
protected:
    string username;
    int    score;
    int    choice;  // 1=ค้อน  2=กรรไกร  3=กระดาษ
public:
    Player(string u, int s);
    virtual void makeChoice() = 0;  // pure virtual
    string getUsername();
    int    getScore();
    void   addScore(int points);
    int    getChoice();
    string getChoiceName();
};

Player::Player(string u, int s) : username(u), score(s), choice(0) {}
string Player::getUsername()        { return username; }
int    Player::getScore()           { return score; }
void   Player::addScore(int points) { score += points; }
int    Player::getChoice()          { return choice; }
string Player::getChoiceName() {
    if (choice == 1) return "ค้อน    (Rock)";
    if (choice == 2) return "กรรไกร  (Scissors)";
    if (choice == 3) return "กระดาษ  (Paper)";
    return "ไม่ทราบ";
}

// ── CLASS 2 : HumanPlayer (Inheritance #1) ──────────
class HumanPlayer : public Player {
public:
    HumanPlayer(string u, int s);
    void makeChoice();
};

HumanPlayer::HumanPlayer(string u, int s) : Player(u, s) {}
void HumanPlayer::makeChoice() {
    cout << "\nเลือก  1=ค้อน  2=กรรไกร  3=กระดาษ : ";
    while (true) {
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "กรุณาพิมพ์ 1, 2 หรือ 3 : ";
            continue;
        }
        if (choice >= 1 && choice <= 3) break;
        cout << "กรุณาพิมพ์ 1, 2 หรือ 3 : ";
    }
}

// ── CLASS 3 : ComputerPlayer (Inheritance #2) ───────
class ComputerPlayer : public Player {
public:
    ComputerPlayer();
    void makeChoice();
};

ComputerPlayer::ComputerPlayer() : Player("Computer", 0) {}
void ComputerPlayer::makeChoice() {
    choice = rand() % 3 + 1;
}

// ── CLASS 4 : FileManager ───────────────────────────
// อ่าน/เขียน users.txt  รูปแบบ → username score
class FileManager {
public:
    int  loadScore(string username);              // โหลดคะแนนเดิม (0 ถ้าไม่มี)
    void save(string username, int score);        // Overloading #1 : บันทึก user เดียว
    void save(vector<pair<string,int>> ranking);  // Overloading #2 : บันทึก ranking
    vector<pair<string,int>> loadRanking();       // โหลด ranking เรียงมากไปน้อย
};

int FileManager::loadScore(string username) {
    ifstream file("users.txt");
    string u; int s;
    while (file >> u >> s)
        if (u == username) { file.close(); return s; }
    file.close();
    return 0;  // ไม่พบ → คะแนนเริ่มต้น 0
}

// Overloading save #1 : อัปเดตคะแนนของ user คนเดียว
void FileManager::save(string username, int score) {
    // อ่านทุก user เข้า memory
    vector<pair<string,int>> all;
    ifstream fileIn("users.txt");
    string u; int s;
    bool found = false;
    while (fileIn >> u >> s) {
        if (u == username) { all.push_back(make_pair(u, score)); found = true; }
        else                 all.push_back(make_pair(u, s));
    }
    fileIn.close();
    if (!found) all.push_back(make_pair(username, score));  // user ใหม่ → เพิ่มเข้าไป

    // เขียนทับกลับลงไฟล์
    ofstream fileOut("users.txt");
    for (auto &r : all)
        fileOut << r.first << " " << r.second << "\n";
    fileOut.close();
}

// Overloading save #2 : เขียน ranking ทั้งหมดทับ users.txt
void FileManager::save(vector<pair<string,int>> ranking) {
    ofstream file("users.txt");
    for (auto &r : ranking)
        file << r.first << " " << r.second << "\n";
    file.close();
}

vector<pair<string,int>> FileManager::loadRanking() {
    vector<pair<string,int>> ranking;
    ifstream file("users.txt");
    string u; int s;
    while (file >> u >> s)
        ranking.push_back(make_pair(u, s));
    file.close();
    sort(ranking.begin(), ranking.end(),
        [](const pair<string,int> &a, const pair<string,int> &b) {
            return a.second > b.second;
        });
    return ranking;
}

// ── CLASS 5 : Game ──────────────────────────────────
class Game {
private:
    Player *player;
    Player *com;
    void    printResult();
public:
    Game(Player *p, Player *c);
    void play();            // Overloading #1 : 1 รอบ
    void play(int rounds);  // Overloading #2 : n รอบ
};

Game::Game(Player *p, Player *c) : player(p), com(c) {}
void Game::printResult() {
    int p = player->getChoice();
    int c = com->getChoice();
    cout << "---------------------------------------\n";
    cout << player->getUsername() << " : " << player->getChoiceName() << "\n";
    cout << "Computer       : " << com->getChoiceName() << "\n";
    if (p == c)
        cout << ">> เสมอ!\n";
    else if ((p==1&&c==2)||(p==2&&c==3)||(p==3&&c==1)) {
        cout << ">> " << player->getUsername() << " ชนะ! +1 คะแนน\n";
        player->addScore(1);
    } else
        cout << ">> Computer ชนะ!\n";
    cout << "คะแนนสะสม : " << player->getScore() << "\n";
}

void Game::play() {
    player->makeChoice();
    com->makeChoice();
    printResult();
}

void Game::play(int rounds) {
    cout << "\n=== เริ่มเกม " << rounds << " รอบ ===\n";
    for (int i = 1; i <= rounds; i++) {
        cout << "\n[ รอบที่ " << i << " ]\n";
        play();
    }
}

// ── MAIN ────────────────────────────────────────────
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand((unsigned)time(nullptr));

    FileManager fm;
    string username;

    cout << "========================================\n";
    cout << "   เกม เป่า ยิง ฉุบ  (Rock Paper Scissors)\n";
    cout << "========================================\n";
    cout << "กรอกชื่อของคุณ : ";
    cin >> username;

    // โหลดคะแนนสะสมเดิม (ถ้ามี)
    int savedScore = fm.loadScore(username);
    cout << "[+] สวัสดี " << username << "!  คะแนนสะสม : " << savedScore << " คะแนน\n";

    HumanPlayer    human(username, savedScore);
    ComputerPlayer computer;
    Game game(&human, &computer);

    // วน loop จนกว่าจะกด 2 ออก
    while (true) {
        game.play(3);

        int finalScore = human.getScore();
        cout << "\n========================================\n";
        cout << "  คะแนนที่ได้   : +" << (finalScore - savedScore) << " คะแนน\n";
        cout << "  คะแนนสะสม    : "   << finalScore << " คะแนน\n";
        cout << "========================================\n";

        fm.save(username, finalScore);  // save #1

        auto ranking = fm.loadRanking();
        cout << "\n========== RANKING ==========\n";
        for (int i = 0; i < (int)ranking.size(); i++)
            cout << i+1 << ". " << ranking[i].first
                 << "  :  " << ranking[i].second << " คะแนน\n";
        fm.save(ranking);  // save #2

        savedScore = finalScore;  // อัปเดตสำหรับรอบถัดไป

        int menuChoice = 0;
        cout << "\n  1. เล่นอีกครั้ง\n";
        cout << "  2. ออก\n";
        cout << "เลือก : ";
        if (!(cin >> menuChoice) || menuChoice == 2) break;
    }

    cout << "\nขอบคุณที่เล่น! Goodbye!\n";
    return 0;
}
