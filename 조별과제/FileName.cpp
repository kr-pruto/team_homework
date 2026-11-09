#include <iostream>
#include <string>
using namespace std;

// ==========================================
// 1. Pokemon 기본 클래스
// ==========================================
class Pokemon {
protected:
    string name;
    string type;
    int hp, maxHp, attack;

public:
    static int totalCount;

    Pokemon(string n, string t, int h, int a)
        : name(n), type(t), hp(h), maxHp(h), attack(a) {
        totalCount++;
    }

    virtual ~Pokemon() {
        totalCount--;
    }

    virtual void attackPokemon(Pokemon& target) {
        target.takeDamage(attack);
    }

    // 특수 기술을 사용할 수 없는 포켓몬의 기본 동작
    virtual void useSpecial(Pokemon& target) {
        cout << name << " cannot use a special skill!\n";
    }

    void takeDamage(int damage) {
        hp -= damage;

        if (hp < 0)
            hp = 0;

        cout << name << " takes " << damage
            << " damage! HP: " << hp << "/" << maxHp << '\n';
    }

    bool isAlive() const {
        return hp > 0;
    }

    string getName() const {
        return name;
    }

    string getType() const {
        return type;
    }

    void showInfo() const {
        cout << name << " [" << type << "] "
            << "HP: " << hp << "/" << maxHp
            << " ATK: " << attack << '\n';
    }

    // 연산자 오버로딩: 체력 비교
    bool operator==(const Pokemon& other) const {
        return hp == other.hp;
    }

    // 연산자 오버로딩: 공격력에 보너스 추가
    int operator+(int bonus) const {
        return attack + bonus;
    }

    friend int calculateDamage(
        const Pokemon& attacker,
        const Pokemon& defender
    );
};

int Pokemon::totalCount = 0;


// ==========================================
// 2. 타입 상성 데미지 계산
// ==========================================
int calculateDamage(
    const Pokemon& attacker,
    const Pokemon& defender
) {
    int damage = attacker.attack;

    if (attacker.type == "Fire" &&
        defender.type == "Grass")
        damage *= 2;

    else if (attacker.type == "Water" &&
        defender.type == "Fire")
        damage *= 2;

    else if (attacker.type == "Grass" &&
        defender.type == "Water")
        damage *= 2;

    else if (attacker.type == "Fire" &&
        defender.type == "Water")
        damage /= 2;

    else if (attacker.type == "Water" &&
        defender.type == "Grass")
        damage /= 2;

    else if (attacker.type == "Grass" &&
        defender.type == "Fire")
        damage /= 2;

    return damage;
}


// ==========================================
// 3. 타입별 포켓몬
// ==========================================
class FirePokemon : public Pokemon {
public:
    FirePokemon(string n, int h, int a)
        : Pokemon(n, "Fire", h, a) {
    }

    void attackPokemon(Pokemon& target) override {
        cout << name << " uses Flame Attack!\n";

        target.takeDamage(
            calculateDamage(*this, target)
        );
    }
};

class WaterPokemon : public Pokemon {
public:
    WaterPokemon(string n, int h, int a)
        : Pokemon(n, "Water", h, a) {
    }

    void attackPokemon(Pokemon& target) override {
        cout << name << " uses Water Shot!\n";

        target.takeDamage(
            calculateDamage(*this, target)
        );
    }
};

class GrassPokemon : public Pokemon {
public:
    GrassPokemon(string n, int h, int a)
        : Pokemon(n, "Grass", h, a) {
    }

    void attackPokemon(Pokemon& target) override {
        cout << name << " uses Leaf Blade!\n";

        target.takeDamage(
            calculateDamage(*this, target)
        );
    }
};


// ==========================================
// 4. 다중 상속 및 특수 기술
// ==========================================
class BattleSkill {
public:
    virtual void useSkill(Pokemon& target) = 0;

    virtual ~BattleSkill() {}
};

class SpecialPokemon : public Pokemon,
    public BattleSkill {
public:
    SpecialPokemon(string n, string t, int h, int a)
        : Pokemon(n, t, h, a) {
    }

    void attackPokemon(Pokemon& target) override {
        cout << name << " uses Normal Attack!\n";

        target.takeDamage(
            calculateDamage(*this, target)
        );
    }

    // BattleSkill의 순수 가상 함수 구현
    void useSkill(Pokemon& target) override {
        cout << name << " uses Special Blast!\n";

        target.takeDamage(
            calculateDamage(*this, target) + 10
        );
    }

    // Pokemon의 가상 함수 재정의
    void useSpecial(Pokemon& target) override {
        useSkill(target);
    }
};


// ==========================================
// 5. Trainer 클래스
// ==========================================
class Trainer {
private:
    static const int MAX_TEAM_SIZE = 3;

    string name;
    Pokemon** team;
    int teamSize;

public:
    Trainer(string n) {
        name = n;
        teamSize = 0;

        team = new Pokemon * [MAX_TEAM_SIZE];

        for (int i = 0; i < MAX_TEAM_SIZE; i++)
            team[i] = nullptr;
    }

    // 복사로 인한 메모리 중복 해제 방지
    Trainer(const Trainer& other) = delete;
    Trainer& operator=(const Trainer& other) = delete;

    ~Trainer() {
        for (int i = 0; i < teamSize; i++)
            delete team[i];

        delete[] team;
    }

    void addPokemon(Pokemon* p) {
        if (teamSize >= MAX_TEAM_SIZE) {
            cout << "Team is full!\n";
            delete p;
            return;
        }

        team[teamSize] = p;
        teamSize++;
    }

    Pokemon* getPokemon(int index) {
        if (index < 0 || index >= teamSize)
            return nullptr;

        return team[index];
    }

    int getTeamSize() const {
        return teamSize;
    }

    string getName() const {
        return name;
    }

    // 선택한 포켓몬을 팀의 첫 번째 위치로 이동
    void selectPokemon(int index) {
        for (int i = index; i > 0; i--) {
            Pokemon* temp = team[i];
            team[i] = team[i - 1];
            team[i - 1] = temp;
        }
    }

    void showTeam() const {
        cout << "\n--- " << name << "'s Team ---\n";

        for (int i = 0; i < teamSize; i++) {
            cout << i + 1 << ". ";
            team[i]->showInfo();
        }
    }

    bool hasAlivePokemon() const {
        for (int i = 0; i < teamSize; i++) {
            if (team[i]->isAlive())
                return true;
        }

        return false;
    }

    int getNextAlive(int start) const {
        for (int i = start; i < teamSize; i++) {
            if (team[i]->isAlive())
                return i;
        }

        return -1;
    }
};


// ==========================================
// 6. 배틀 시스템
// ==========================================
void battle(Trainer& player, Trainer& rival) {
    int playerIndex = 0;
    int rivalIndex = 0;

    Pokemon* playerPokemon = player.getPokemon(playerIndex);
    Pokemon* rivalPokemon = rival.getPokemon(rivalIndex);

    cout << "\n========== BATTLE START ==========\n";

    while (player.hasAlivePokemon() &&
        rival.hasAlivePokemon()) {

        cout << "\n----------------------------------\n";

        cout << "[Player]\n";
        playerPokemon->showInfo();

        cout << "\n[Rival]\n";
        rivalPokemon->showInfo();

        // 플레이어 턴
        cout << "\n1. Normal Attack\n";
        cout << "2. Special Skill\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            playerPokemon->attackPokemon(*rivalPokemon);
        }

        else if (choice == 2) {
            playerPokemon->useSpecial(*rivalPokemon);
        }

        else {
            cout << "Invalid choice!\n";
            continue;
        }

        // 상대 포켓몬이 쓰러졌는지 확인
        if (!rivalPokemon->isAlive()) {
            cout << rivalPokemon->getName()
                << " fainted!\n";

            rivalIndex = rival.getNextAlive(rivalIndex + 1);

            if (rivalIndex == -1)
                break;

            rivalPokemon = rival.getPokemon(rivalIndex);

            cout << "Rival sends out "
                << rivalPokemon->getName() << "!\n";
        }

        // 상대 턴
        cout << "\nRival's turn!\n";

        rivalPokemon->attackPokemon(*playerPokemon);

        // 플레이어 포켓몬이 쓰러졌는지 확인
        if (!playerPokemon->isAlive()) {
            cout << playerPokemon->getName()
                << " fainted!\n";

            playerIndex = player.getNextAlive(playerIndex + 1);

            if (playerIndex == -1)
                break;

            playerPokemon = player.getPokemon(playerIndex);

            cout << "You send out "
                << playerPokemon->getName() << "!\n";
        }
    }

    cout << "\n========== BATTLE RESULT ==========\n";

    if (player.hasAlivePokemon())
        cout << player.getName() << " WINS!\n";

    else
        cout << player.getName() << " LOSES!\n";
}


// ==========================================
// 7. Main
// ==========================================
int main() {
    Trainer player("Player");
    Trainer rival("Rival");

    // 플레이어 포켓몬
    player.addPokemon(
        new FirePokemon("Charmander", 100, 20)
    );

    player.addPokemon(
        new WaterPokemon("Squirtle", 110, 18)
    );

    player.addPokemon(
        new SpecialPokemon("Pikachu", "Electric", 90, 22)
    );

    // 상대 포켓몬
    rival.addPokemon(
        new GrassPokemon("Bulbasaur", 105, 19)
    );

    rival.addPokemon(
        new FirePokemon("Vulpix", 95, 21)
    );

    rival.addPokemon(
        new SpecialPokemon("Eevee", "Normal", 100, 17)
    );

    cout << "Total Pokemon: "
        << Pokemon::totalCount << '\n';

    player.showTeam();

    // 출전 포켓몬 선택
    int choice;

    cout << "\nChoose your first Pokemon: ";
    cin >> choice;

    if (choice < 1 || choice > player.getTeamSize()) {
        cout << "Invalid choice!\n";
        return 0;
    }

    player.selectPokemon(choice - 1);

    battle(player, rival);

    cout << "\nTotal Pokemon: "
        << Pokemon::totalCount << '\n';

    return 0;
}
