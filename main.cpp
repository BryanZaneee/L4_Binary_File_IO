#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>

using namespace std;

// Define a structure for the weapons
struct Weapon {
    string name; // Name of the weapon
    int powerRating; // Power rating of the weapon
    float powerConsumption; // Power consumption of the weapon
};

// Define a structure for the ships
struct Ship {
    string name; // Name of the ship
    string classType; // Class of the ship
    short length; // Length of the ship
    int shieldCapacity; // Shield capacity of the ship
    float maxWarpSpeed; // Maximum warp speed of the ship
    vector<Weapon> weapons; // Vector containing all weapons of the ship
};

// Vector to store all ships
vector<Ship> ships;

// Function to load file
void loadFile(const string& filename) {
    ifstream file(filename, ios::binary); // Open the file in binary mode

    int numShips; // Variable to hold the number of ships
    file.read((char*)&numShips, sizeof(int)); // Read the number of ships

    // Loop through each ship
    for (int i = 0; i < numShips; i++) {
        Ship ship; // Create a new ship object
        int length; // Variable to hold the length of the string

        // Read the length of the name
        file.read((char*)&length, sizeof(int));
        // Resize the string to the correct length (subtract 1 to exclude null terminator)
        ship.name.resize(length - 1);
        // Read the name directly into the string's buffer
        file.read(&ship.name[0], length);

        // Repeat the same steps for classType
        file.read((char*)&length, sizeof(int));
        ship.classType.resize(length - 1);
        file.read(&ship.classType[0], length);

        // Read the length, shield capacity, and max warp speed of the ship
        file.read((char*)&ship.length, sizeof(short));
        file.read((char*)&ship.shieldCapacity, sizeof(int));
        file.read((char*)&ship.maxWarpSpeed, sizeof(float));

        int numWeapons; // Variable to hold the number of weapons
        file.read((char*)&numWeapons, sizeof(int)); // Read the number of weapons

        // Loop through each weapon
        for (int j = 0; j < numWeapons; j++) {
            Weapon weapon; // Create a new weapon object

            // Read the length of the weapon's name
            file.read((char*)&length, sizeof(int));
            // Resize the string to the correct length (subtract 1 to exclude null terminator)
            weapon.name.resize(length - 1);
            // Read the name directly into the string's buffer
            file.read(&weapon.name[0], length);

            // Read the power rating and power consumption of the weapon
            file.read((char*)&weapon.powerRating, sizeof(int));
            file.read((char*)&weapon.powerConsumption, sizeof(float));

            // Add the weapon to the ship's weapon vector
            ship.weapons.push_back(weapon);
        }

        // Add the ship to the ships vector
        ships.push_back(ship);
    }
}

// Function to print a single ship
void printShip(const Ship& ship) {
    int totalFirepower = 0; // Initialize total firepower to 0

    // Print the ship's information
    cout << "Name: " << ship.name << endl;
    cout << "Class: " << ship.classType << endl;
    cout << "Length: " << ship.length << endl;
    cout << "Shield capacity: " << ship.shieldCapacity << endl;
    cout << "Maximum Warp: " << ship.maxWarpSpeed << endl;
    cout << "Armaments: " << endl;

    // Check if the ship has weapons
    if (ship.weapons.empty()) {
        cout << "Unarmed" << endl;
    } else {
        // Loop through each weapon
        for (const Weapon& weapon : ship.weapons) {
            // Print the weapon's information
            cout << weapon.name << ", " << weapon.powerRating << ", " << weapon.powerConsumption << endl;
            // Add the power rating of the current weapon to total firepower
            totalFirepower += weapon.powerRating;
        }

        // Print the total firepower
        cout << "Total firepower: " << totalFirepower << endl;
    }
}


// Function to print all ships
void printAllShips(const vector<Ship>& ships) {
    for (const Ship& ship : ships) { // Iterate through each ship in the vector
        printShip(ship); // Print each ship
        cout << endl; // Insert a newline for readability
    }
}

// Function to find the ship with the most powerful weapon
const Ship* findShipWithMostPowerfulWeapon(const vector<Ship>& ships) {
    const Ship* maxShip = nullptr; // Pointer to the ship with the most powerful weapon
    int maxPower = -1; // The power of the most powerful weapon

    // Iterate through each ship in the vector
    for (const Ship& ship : ships) {
        // Iterate through each weapon of the ship
        for (const Weapon& weapon : ship.weapons) {
            // If the current weapon is more powerful than the most powerful one found so far
            if (weapon.powerRating > maxPower) {
                maxPower = weapon.powerRating; // Update the power of the most powerful weapon
                maxShip = &ship; // Update the pointer to the ship with the most powerful weapon
            }
        }
    }

    return maxShip; // Return the pointer to the ship with the most powerful weapon
}

// Function to find the most powerful ship overall
const Ship* findMostPowerfulShip(const vector<Ship>& ships) {
    const Ship* maxShip = nullptr; // Pointer to the most powerful ship
    int maxPower = -1; // The total power of the most powerful ship

    // Iterate through each ship in the vector
    for (const Ship& ship : ships) {
        int totalPower = 0; // The total power of the current ship
        // Iterate through each weapon of the ship
        for (const Weapon& weapon : ship.weapons) {
            totalPower += weapon.powerRating; // Add the power of the weapon to the total power of the ship
        }
        // If the current ship is more powerful than the most powerful one found so far
        if (totalPower > maxPower) {
            maxPower = totalPower; // Update the total power of the most powerful ship
            maxShip = &ship; // Update the pointer to the most powerful ship
        }
    }

    return maxShip; // Return the pointer to the most powerful ship
}

// Function to find the weakest ship (ignoring unarmed)
const Ship* findWeakestShip(const vector<Ship>& ships) {
    const Ship* minShip = nullptr; // Pointer to the weakest ship
    int minPower = INT_MAX; // The total power of the weakest ship

    // Iterate through each ship in the vector
    for (const Ship& ship : ships) {
        if (ship.weapons.empty()) continue; // Ignore unarmed ships
        int totalPower = 0; // The total power of the current ship
        // Iterate through each weapon of the ship
        for (const Weapon& weapon : ship.weapons) {
            totalPower += weapon.powerRating; // Add the power of the weapon to the total power of the ship
        }
        // If the current ship is weaker than the weakest one found so far
        if (totalPower < minPower) {
            minPower = totalPower; // Update the total power of the weakest ship
            minShip = &ship; // Update the pointer to the weakest ship
        }
    }

    return minShip; // Return the pointer to the weakest ship
}

// Function to find unarmed ships
vector<const Ship*> findUnarmedShips(const vector<Ship>& ships) {
    vector<const Ship*> result; // Vector of pointers to unarmed ships

    // Iterate through each ship in the vector
    for (const Ship& ship : ships) {
        // If the ship has no weapons
        if (ship.weapons.empty()) {
            result.push_back(&ship); // Add the pointer to the ship to the result
        }
    }

    return result; // Return the vector of pointers to unarmed ships
}

int main() {
    // Provide options to the user
    cout << "Which file(s) to open?" << endl;
    cout << "1. friendlyships.shp" << endl;
    cout << "2. enemyships.shp" << endl;
    cout << "3. Both files" << endl;

    int option;
    cin >> option; // Get the user's choice

    // Load the chosen file(s)
    if (option == 1) {
        loadFile("friendlyships.shp");
    } else if (option == 2) {
        loadFile("enemyships.shp");
    } else if (option == 3) {
        loadFile("friendlyships.shp");
        loadFile("enemyships.shp");
    }

    // Provide options to the user
    cout << "1. Print all ships" << endl;
    cout << "2. Starship with the strongest weapon" << endl;
    cout << "3. Strongest starship overall" << endl;
    cout << "4. Weakest ship (ignoring unarmed)" << endl;
    cout << "5. Unarmed ships" << endl;

    cin >> option; // Get the user's choice

    // Execute the chosen action
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