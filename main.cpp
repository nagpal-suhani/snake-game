
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

void run();
void printMap();
void initMap();
void move(int dx, int dy);
void update();
void changeDirection(char key);
void clearScreen();
void generateFood();

char getMapValue(int value);

// Map dimensions
const int mapwidth = 20;
const int mapheight = 20;

const int size = mapwidth * mapheight;

// The tile values for the map
int map[size];

// Snake head details
int headxpos;
int headypos;
int direction = 1; // Default direction: right

// Amount of food the snake has (How long the body is)
int food = 4;

// Determine if game is running
bool running;

int main()
{
    run();
    return 0;
}

// Main game function
void run()
{
    // Seed random number generator
    srand(time(0));

    // Initialize the map
    initMap();
    running = true;

    while (running) {
        // If a key is pressed
        if (cin.peek() != EOF) {
            char key;
            cin >> key; // Get the key
            changeDirection(key);
        }

        // Update the map
        update();

        // Clear the screen
        clearScreen();

        // Print the map
        printMap();

        // Delay 0.4 seconds
        this_thread::sleep_for(chrono::milliseconds(400));
    }

    // Game Over Text
    cout << "\t\tGame Over!" << endl
         << "\t\tYour score is: " << food << endl;

    // Stop console from closing instantly
    cin.ignore();
    cin.get();
}

// Changes snake direction from input
void changeDirection(char key) {
    switch (key) {
    case 'w':
        if (direction != 2) direction = 0; // Up
        break;
    case 'd':
        if (direction != 3) direction = 1; // Right
        break;
    case 's':
        if (direction != 0) direction = 2; // Down
        break;
    case 'a':
        if (direction != 1) direction = 3; // Left
        break;
    }
}

// Moves snake head to new location
void move(int dx, int dy) {
    int newx = headxpos + dx;
    int newy = headypos + dy;

    // Check boundaries
    if (newx < 0 || newx >= mapwidth || newy < 0 || newy >= mapheight || map[newx + newy * mapwidth] == -1) {
        running = false;
        return;
    }

    // Check if there is food at the location
    if (map[newx + newy * mapwidth] == -2) {
        food++;
        generateFood();
    } else if (map[newx + newy * mapwidth] != 0) {
        // If colliding with self
        running = false;
        return;
    }

    // Move head to new location
    headxpos = newx;
    headypos = newy;
    map[headxpos + headypos * mapwidth] = food + 1;
}

// Clears the screen
void clearScreen() {
    system("clear"); // Use "cls" for Windows, "clear" for Linux/Mac
}

// Generates new food on map
void generateFood() {
    int x, y;
    do {
        x = rand() % mapwidth;
        y = rand() % mapheight;
    } while (map[x + y * mapwidth] != 0);

    map[x + y * mapwidth] = -2;
}

// Updates the map
void update() {
    switch (direction) {
    case 0: move(0, -1); break; // Up
    case 1: move(1, 0); break;  // Right
    case 2: move(0, 1); break;  // Down
    case 3: move(-1, 0); break; // Left
    }

    // Reduce snake body values on map
    for (int i = 0; i < size; i++) {
        if (map[i] > 0) map[i]--;
    }
}

// Initializes map
void initMap() {
    headxpos = mapwidth / 2;
    headypos = mapheight / 2;
    map[headxpos + headypos * mapwidth] = food;

    // Create walls
    for (int x = 0; x < mapwidth; ++x) {
        map[x] = -1;
        map[x + (mapheight - 1) * mapwidth] = -1;
    }
    for (int y = 0; y < mapheight; ++y) {
        map[y * mapwidth] = -1;
        map[(mapwidth - 1) + y * mapwidth] = -1;
    }

    generateFood();
}

// Prints the map
void printMap() {
    for (int y = 0; y < mapheight; ++y) {
        for (int x = 0; x < mapwidth; ++x) {
            cout << getMapValue(map[x + y * mapwidth]);
        }
        cout << endl;
    }
}

// Returns graphical character for display
char getMapValue(int value) {
    if (value > 0) return 'o'; // Snake body
    if (value == -1) return 'X'; // Wall
    if (value == -2) return 'O'; // Food
    return ' '; // Empty space
}
