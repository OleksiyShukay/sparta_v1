

#include <iostream>
#include <string>
#include <cstring>//for rand() and srand()
#include <ctime> //дл€ time()

//random number generate
int getRandomNumber(int min, int max) {
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand()*fraction*(max - min + 1) + min);
}

class Creature {
protected:
	std::string m_name;
	char m_symbol;
	int m_health;
	int m_damage;
	int m_gold;

public:
	Creature(std::string name, char symbol, int health, int damage, int gold)
		:m_name(name), m_symbol(symbol), m_health(health), m_damage(damage), m_gold(gold) {}

	const std::string &getName() {
		return m_name;
	}
	char getSymbol() {
		return m_symbol;
	}
	int getHealth() {
		return m_health;
	}
	int getDamage() {
		return m_damage;
	}
	int getGold() {
		return m_gold;
	}

	void reduceHealth(int health) { 
		m_health -= health;
	}
	bool isDead() {
		return m_health <= 0;
	}
	void addGold(int gold) {
		m_gold += gold;
	}
};

class Player : public Creature {
	int m_level = 1;

public:
	Player(std::string name) :Creature(name, '@', 10, 1, 0) {}

	void levelUp() {
		++m_level;
		++m_damage;
	}

	int getLevel() {
		return m_level;
	}

	bool hasWon() {
		return m_level >= 20;
	}
};

class Monster : public Creature {
public:
	enum Type {
		DRAGON,
		ORC,
		SLIME,
		MAX_TYPES
	};

	struct MonsterData {
		const char *name;
		char symbol;
		int health;
		int damage;
		int gold;
	};

	//array monsterData of type MonsterData
	static MonsterData monsterData[MAX_TYPES];

	//creat Monster
	Monster(Type type)
		:Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold) {}


	//generate random monster
	static Monster getRandomMonster() {
		int num = getRandomNumber(0, MAX_TYPES - 1);
		return Monster(static_cast<Type>(num));
	}
};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]{
  {"dragon", 'D', 20, 4, 100},
  {"orc", 'o', 4, 2, 25},
  {"slime", 's', 1, 1, 10}
};


void attackMonster(Player &p, Monster &m) {
  
  if (p.isDead())
    return;

  std::cout << "You hit the " << m.getName() << " for " << p.getDamage() << " damage.\n ";

   m.reduceHealth(p.getDamage());

  if (m.isDead()) {
	std::cout << "You killed the " << m.getName() << ".\n";
	p.levelUp();
	std::cout << "You are now level" << p.getLevel() << " .\n";
	std::cout << "You found " << m.getGold() << " gold.\n";
  }
}


void attackPlayer(Monster &m, Player &p) {
  //≈сли монстр мертв, то он не может атаковать игрока
  if (m.isDead())
   return;
  //«доровье игрока уменьшаетьс€ от урона монстра
  p.reduceHealth(m.getDamage());
  std::cout << "The " << m.getName() << " hit you for " << m.getDamage() << " damage.\n";
}


void fightMonster(Player &p){
  
  Monster m = Monster::getRandomMonster();
  std::cout << " You have encountered a " << m.getName() << " (" << m.getSymbol() << ").\n";

  while (!m.isDead() && !p.isDead()) {
	std::cout << "(R)un or (F)ight:";
	char input;
	std::cin >> input;
	if (input == 'R' || input == 'r') {
	  //50x50 
	  if (getRandomNumber(1, 2) == 1) {
		std::cout << "You successfully fled.\n";
		return;
	  } else {
	    std::cout << "You failed to flee.\n";
		attackMonster(p, m);
		continue;
	  }
	}
	if (input == 'F' || input == 'f') {
      //player attack first
	  attackMonster(p, m);
	  attackPlayer(m, p);
	}
  }
}


int main() {
  srand(static_cast<unsigned int>(time(0)));//system time - start time
  rand();//reset time

  std::cout << "Enter your name: ";
  std::string playName;
  std::cin >> playName;

  Player p(playName);
  std::cout << " Welcome, " << p.getName() << '\n';

  //≈сли игрок не мертв и еще не победил, то игра продолжаетьс€
  while (!p.isDead() && !p.hasWon())
	fightMonster(p);
  
  //  этому моменту игрок либо мертв, либо победил
  if (p.isDead()) {
	std::cout << "You died at level" << p.getLevel() << " and with " << p.getGold() << " gpld.\n";
	std::cout << "Too bad you can not take it eith you!\n";
  } else {
	std::cout << " You won the game with " << p.getGold() << " gold!\n";
  }
  return 0;
}
