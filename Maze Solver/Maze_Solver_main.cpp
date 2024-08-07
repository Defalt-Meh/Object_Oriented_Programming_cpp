#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> // for std::remove

using namespace std;

struct MazeNode {
    char cellContent;   // Content of the cell (wall, item, player, '.')
    MazeNode *right, *left, *up, *down; // Pointers to neighboring nodes

    MazeNode(char content) : cellContent(content), right(nullptr), left(nullptr), up(nullptr), down(nullptr) {}
};

// Function prototypes
MazeNode*** create2DLinkedList(size_t rows, size_t cols);
void loadMazeFromFile(const string &filename, MazeNode*** &maze, int &startRow, int &startCol, int &exitRow, int &exitCol, int &rows, int &cols);
void printMaze(MazeNode*** maze, int rows, int cols, int playerRow, int playerCol, int score);
bool movePlayer(MazeNode*** maze, int &playerRow, int &playerCol, char direction, int rows, int cols, int &score, bool &itemFound);
void collectItem(MazeNode*** maze, int playerRow, int playerCol, int &score);
void deallocateMaze(MazeNode*** maze, int rows, int cols);
void playMazeGame(const string &filename);

int main() {
    string filename;
    cout << "Enter the maze file name:" << endl;
    cin >> filename;
    playMazeGame(filename);
    return 0;
}

//Main Game loop
void playMazeGame(const string &filename) {
    int rows, cols;
    int startRow, startCol, exitRow, exitCol;
    MazeNode*** maze = nullptr;

    // Load maze from file
    loadMazeFromFile(filename, maze, startRow, startCol, exitRow, exitCol, rows, cols);

    int playerRow = startRow;
    int playerCol = startCol;
    int score = 0;
    char command;
    bool itemFound = false;

    cout << "Maze loaded. Start exploring!" << endl;

    // Game loop
    while (true) {
        cout << "Enter input (R/L/U/D/P/C/Q):" << endl;
        cin >> command;

        switch (command) {
            case 'P'://Print maze
                printMaze(maze, rows, cols, playerRow, playerCol, score);
                break;
            case 'R'://Move right
            case 'L'://Move left
            case 'U'://Move up
            case 'D'://Move down
                if (movePlayer(maze, playerRow, playerCol, command, rows, cols, score, itemFound)) {
                    if (playerRow == exitRow && playerCol == exitCol) {
                        cout << "Congratulations! You reached the exit!" << endl;
                        cout << "Final score: " << score << endl;
                        deallocateMaze(maze, rows, cols);
                        return;
                    }
                }
                break;
            case 'C'://Collect item
                if (itemFound) {
                    collectItem(maze, playerRow, playerCol, score);
                    itemFound = false; // Reset itemFound after collection
                } else {
                    cout << "No item to collect here." << endl;
                }
                break;
            case 'Q'://Quit game
                cout << "Game ended by the player." << endl;
                cout << "Final score: " << score << endl;
                deallocateMaze(maze, rows, cols);
                return;
            default:
                cout << "Invalid input. Please try again." << endl;
                break;
        }
    }
}

// Creates a 2D linked list representation of the maze
MazeNode*** create2DLinkedList(int rows, int cols) {
    MazeNode*** maze = new MazeNode**[rows];
    for (int i = 0; i < rows; ++i) {
        maze[i] = new MazeNode*[cols];
        for (int j = 0; j < cols; ++j) {
            maze[i][j] = new MazeNode('.');
        }
    }

    // Set neighbors for each node
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (j > 0) maze[i][j]->left = maze[i][j - 1];
            if (j < cols - 1) maze[i][j]->right = maze[i][j + 1];
            if (i > 0) maze[i][j]->up = maze[i - 1][j];
            if (i < rows - 1) maze[i][j]->down = maze[i + 1][j];
        }
    }
    return maze;
}

// Loads maze data from a file into a 2D linked list
void loadMazeFromFile(const string &filename, MazeNode*** &maze, int &startRow, int &startCol, int &exitRow, int &exitCol, int &rows, int &cols) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file." << endl;
        cout << "Failed to load maze. Exiting..." << endl;
        exit(0);
    }

    string line;
    rows = 0;
    cols = 0;

    // Determine maze dimensions
    while (getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); // Remove '\r' if present
        if (cols == 0) {
            cols = line.length();
        }
        ++rows;
    }

    file.clear();
    file.seekg(0, ios::beg);

    maze = create2DLinkedList(rows, cols);

    int currentRow = 0;
    int currentCol = 0;

    // Load maze data into the linked list
    while (getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); // Remove '\r' if present
        currentCol = 0;
        for (char &c : line) {
            maze[currentRow][currentCol]->cellContent = c;
            if (c == 'X') {
                startRow = currentRow;
                startCol = currentCol;
            } else if (c == 'E') {
                exitRow = currentRow;
                exitCol = currentCol;
            }
            ++currentCol;
        }
        ++currentRow;
    }

    if (currentRow != rows) {
        cout << "Failed to load maze. Number of rows mismatch. Exiting..." << endl;
        exit(0);
    }

    file.close();
}

// Prints the current state of the maze
void printMaze(MazeNode*** maze, int rows, int cols, int playerRow, int playerCol, int score) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == playerRow && j == playerCol) {
                cout << 'X';//Player's position
            } else {
                cout << maze[i][j]->cellContent;//Cell content
            }
        }
        cout << endl; // Use endl for new lines
    }
    cout << "Current score: " << score << endl;
}

// Moves the player in the maze based on the direction
bool movePlayer(MazeNode*** maze, int &playerRow, int &playerCol, char direction, int rows, int cols, int &score, bool &itemFound) {
    MazeNode *currentNode = maze[playerRow][playerCol];
    MazeNode *nextNode = nullptr;
    string directionText;

    // Store the previous position and its content
    int prevRow = playerRow;
    int prevCol = playerCol;
    char prevCellContent = currentNode->cellContent;

    // Determine the next node based on the direction
    switch (direction) {
        case 'R':
            if (playerCol < cols - 1) {
                nextNode = currentNode->right;
                directionText = "right";
                playerCol++; // Move right
            } else {
                cout << "Cannot move: Out of the maze bound." << endl;
                return false;
            }
            break;
        case 'L':
            if (playerCol > 0) {
                nextNode = currentNode->left;
                directionText = "left";
                playerCol--; // Move left
            } else {
                cout << "Cannot move: Out of the maze bound." << endl;
                return false;
            }
            break;
        case 'U':
            if (playerRow > 0) {
                nextNode = currentNode->up;
                directionText = "up";
                playerRow--; // Move up
            } else {
                cout << "Cannot move: Out of the maze bound." << endl;
                return false;
            }
            break;
        case 'D':
            if (playerRow < rows - 1) {
                nextNode = currentNode->down;
                directionText = "down";
                playerRow++; // Move down
            } else {
                cout << "Cannot move: Out of the maze bound." << endl;
                return false;
            }
            break;
        default:
            cout << "Invalid move direction." << endl;
            return false;
    }

    // Check if the next node is a wall
    if (nextNode == nullptr || nextNode->cellContent == '#') {
        cout << "Cannot move: There is a wall in that direction." << endl;

        // Reset player position to the previous valid position
        playerRow = prevRow;
        playerCol = prevCol;
        return false;
    }

    // Notify the player of the move
    if (!directionText.empty()) {
        cout << "You moved " << directionText << "." << endl;
    }

    // Handle item discovery
    itemFound = (nextNode->cellContent == 'I');
    if (itemFound) {
        cout << "You found an item worth 204 points!" << endl;
    }

    // Reappearing item logic
    if (prevCellContent == 'I') {
        maze[prevRow][prevCol]->cellContent = 'I'; // Reappear the item at the previous position
    }
    if (prevCellContent == 'X') {
        maze[prevRow][prevCol]->cellContent = '.'; // Clear player position
    }

    return true;
}

// Collects an item at the player's current position
void collectItem(MazeNode*** maze, int playerRow, int playerCol, int &score) {
    MazeNode *currentNode = maze[playerRow][playerCol];
    if (currentNode->cellContent == 'I') {
        score += 204;//Score update
        currentNode->cellContent = '.'; // Remove the item from the maze
        cout << "Item collected! Your score is now " << score << "." << endl;
    } else {
        cout << "No item to collect here." << endl;
    }
}

// Deallocates memory used by the maze(As requested)
void deallocateMaze(MazeNode*** maze, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            delete maze[i][j];
        }
        delete[] maze[i];
    }
    delete[] maze;
}


//There are easier things to do in life... Like watching paint dry.
