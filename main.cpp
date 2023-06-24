#include <iostream>
#include <fstream>
#include <string>
#include <vector>  // Include vector library
#include <climits> // Include climits library for INT_MAX constant

using namespace std;

// Define a structure for the weapons
struct Weapon {
    string name;
    int powerRating;
    float powerConsumption;
};

// Define a structure for the ships
struct Ship {
    string name;
    string classType;
    short length;
    int shieldCapacity;
    float maxWarpSpeed;
    vector<Weapon> weapons; // A ship has a vector of weapons
};

// Create a vector of ships to hold all ships in the game
vector<Ship> ships;

// Function to load file
void loadFile(const string& filename) {
    ifstream file(filename, ios::binary);

    int numShips;
    file.read((char*)&numShips, sizeof(int));

    for (int i = 0; i < numShips; i++) {
        Ship ship;
        int length;

        // Read the length of the name
        file.read((char*)&length, sizeof(int));
        ship.name.resize(length - 1);  // -1 to exclude the null terminator
        file.read(&ship.name[0], length);

        // Repeat the same steps for classType
        file.read((char*)&length, sizeof(int));
        ship.classType.resize(length - 1);  // -1 to exclude the null terminator
        file.read(&ship.classType[0], length);

        file.read((char*)&ship.length, sizeof(short));
        file.read((char*)&ship.shieldCapacity, sizeof(int));
        file.read((char*)&ship.maxWarpSpeed, sizeof(float));

        int numWeapons;
        file.read((char*)&numWeapons, sizeof(int));

        for (int j = 0; j < numWeapons; j++) {
            Weapon weapon;

            file.read((char*)&length, sizeof(int));
            weapon.name.resize(length - 1);  // -1 to exclude the null terminator
            file.read(&weapon.name[0], length);

            file.read((char*)&weapon.powerRating, sizeof(int));
            file.read((char*)&weapon.powerConsumption, sizeof(float));

            ship.weapons.push_back(weapon);
        }

        ships.push_back(ship);
    }
}

// Function to print a single ship
void printShip(const Ship& ship) {
    int totalFirepower = 0; // Initialize total firepower to 0

    cout << "Name: " << ship.name << endl;
    cout << "Class: " << ship.classType << endl;
    cout << "Length: " << ship.length << endl;
    cout << "Shield capacity: " << ship.shieldCapacity << endl;
    cout << "Maximum Warp: " << ship.maxWarpSpeed << endl;
    cout << "Armaments: " << endl;
    if (ship.weapons.empty()) {
        cout << "Unarmed" << endl; // If the ship has no weapons, print "Unarmed"
    } else {
        for (const Weapon& weapon : ship.weapons) {
            // Removed the dash and extra spaces at the start of the line
            cout << weapon.name << ", " << weapon.powerRating << ", " << weapon.powerConsumption << endl;
            totalFirepower += weapon.powerRating; // Add the power rating of the current weapon to total firepower
        }
        cout << "Total firepower: " << totalFirepower << endl; // Print the total firepower
    }
}


// Function to print all ships
void printAllShips(const vector<Ship>& ships) {
    for (const Ship& ship : ships) {
        printShip(ship);
        cout << endl;
    }
}

// Function to find the ship with the most powerful weapon
const Ship* findShipWithMostPowerfulWeapon(const vector<Ship>& ships) {
    const Ship* maxShip = nullptr;
    int maxPower = -1;
    for (const Ship& ship : ships) {
        for (const Weapon& weapon : ship.weapons) {
            if (weapon.powerRating > maxPower) {
                maxPower = weapon.powerRating;
                maxShip = &ship;
            }
        }
    }
    return maxShip;
}

// Function to find the most powerful ship overall
const Ship* findMostPowerfulShip(const vector<Ship>& ships) {
    const Ship* maxShip = nullptr;
    int maxPower = -1;
    for (const Ship& ship : ships) {
        int totalPower = 0;
        for (const Weapon& weapon : ship.weapons) {
            totalPower += weapon.powerRating;
        }
        if (totalPower > maxPower) {
            maxPower = totalPower;
            maxShip = &ship;
        }
    }
    return maxShip;
}

// Function to find the weakest ship (ignoring unarmed)
const Ship* findWeakestShip(const vector<Ship>& ships) {
    const Ship* minShip = nullptr;
    int minPower = INT_MAX;
    for (const Ship& ship : ships) {
        if (ship.weapons.empty()) continue;
        int totalPower = 0;
        for (const Weapon& weapon : ship.weapons) {
            totalPower += weapon.powerRating;
        }
        if (totalPower < minPower) {
            minPower = totalPower;
            minShip = &ship;
        }
    }
    return minShip;
}

// Function to find unarmed ships
vector<const Ship*> findUnarmedShips(const vector<Ship>& ships) {
    vector<const Ship*> result;
    for (const Ship& ship : ships) {
        if (ship.weapons.empty()) {
            result.push_back(&ship);
        }
    }
    return result;
}

int main()
{
    cout << "Which file(s) to open?" << endl;
    cout << "1. friendlyships.shp" << endl;
    cout << "2. enemyships.shp" << endl;
    cout << "3. Both files" << endl;
    int option;
    cin >> option;

    /* Load files here */
    if (option == 1) {
        loadFile("friendlyships.shp");
    } else if (option == 2) {
        loadFile("enemyships.shp");
    } else if (option == 3) {
        loadFile("friendlyships.shp");
        loadFile("enemyships.shp");
    }

    cout << "1. Print all ships" << endl;
    cout << "2. Starship with the strongest weapon" << endl;
    cout << "3. Strongest starship overall" << endl;
    cout << "4. Weakest ship (ignoring unarmed)" << endl;
    cout << "5. Unarmed ships" << endl;

    cin >> option;

    /* Work your magic here */
    if (option == 1) {
        printAllShips(ships);
    } else if (option == 2) {
        const Ship* ship = findShipWithMostPowerfulWeapon(ships);
        if (ship) {
            printShip(*ship);
        }
    } else if (option == 3) {
        const Ship* ship = findMostPowerfulShip(ships);
        if (ship) {
            printShip(*ship);
        }
    } else if (option == 4) {
        const Ship* ship = findWeakestShip(ships);
        if (ship) {
            printShip(*ship);
        }
    } else if (option == 5) {
        vector<const Ship*> unarmedShips = findUnarmedShips(ships);
        for (const Ship* ship : unarmedShips) {
            printShip(*ship);
        }
    }

    return 0;
}