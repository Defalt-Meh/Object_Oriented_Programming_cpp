#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Directions: right, left, down, up, and diagonals
const int dx[8] = {1, -1, 0, 0, 1, -1, 1, -1};
const int dy[8] = {0, 0, 1, -1, 1, -1, -1, 1};

// Check if the word exists in the given direction
bool isWordHere(const vector<vector<char>> &matrix, size_t startX, size_t startY, const string &word, int dirX, int dirY) {
    size_t wordLen = word.length(), rows = matrix.size(), cols = matrix[0].size();
    size_t endX = startX + (wordLen - 1) * dirX, endY = startY + (wordLen - 1) * dirY;

    // Check if the word is within bounds
    if (endX < 0 || endY < 0 || endX >= rows || endY >= cols) return false;

    // Traverse in the specified direction
    for (size_t i = 0; i < wordLen; i++) {
        size_t newX = startX + i * dirX, newY = startY + i * dirY;
        if (matrix[newX][newY] != word[i]) return false;
    }
    return true;
}

// Find the word in the matrix and update the score
bool findTheElusiveWord(const vector<vector<char>> &matrix, const string &word, int &score) {
    size_t rows = matrix.size(), cols = matrix[0].size();
    for (size_t x = 0; x < rows; x++) {
        for (size_t y = 0; y < cols; y++) {
            if (matrix[x][y] == word[0]) { // Check for the first letter
                for (int dir = 0; dir < 8; dir++) {
                    if (isWordHere(matrix, x, y, word, dx[dir], dy[dir])) {
                        score += word.length() * (dir < 4 ? 1 : 2); // Score based on direction: straight or diagonal
                        if (word.length() > 5) score += 2; // Bonus points for longer words
                        return true;
                    }
                }
            }
        }
    }
    score -= 5; // Penalty for not finding the word
    return false;
}

// Read the matrix from input
vector<vector<char>> grabTheMatrix(size_t rows) {
    vector<vector<char>> matrix(rows);
    for (size_t i = 0; i < rows; i++) {
        string row;
        cin >> row;
        matrix[i] = vector<char>(row.begin(), row.end()); // Convert row string to vector of characters
    }
    return matrix;
}

// Read words from input
vector<string> grabTheWords(size_t wordsCount) {
    vector<string> words(wordsCount);
    for (size_t i = 0; i < wordsCount; i++) {
        cin >> words[i];
    }
    return words;
}

int main() {
    size_t rows;//size_t avoids issues related to negative values or type mismatches.
    cin >> rows; // Number of rows in the matrix
    vector<vector<char>> matrix = grabTheMatrix(rows);

    size_t wordsCount;
    cin >> wordsCount; // Number of words to find
    vector<string> words = grabTheWords(wordsCount);

    int score = 0; // Initialize score
    for (const string &word : words) {
        findTheElusiveWord(matrix, word, score);
    }

    cout << score << endl; // Output the final score
    return 0;
}
