
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <random>

std::queue<char> pieceQueue;

char getNextPiece() {
    if (pieceQueue.empty()) {
        std::vector<char> bag = { 'I', 'O', 'T', 'S', 'Z', 'L', 'J' };

        static std::random_device rd;
        static std::mt19937 g(rd());
        std::shuffle(bag.begin(), bag.end(), g);

        for (char piece : bag) {
            pieceQueue.push(piece);
        }
    }

    char next = pieceQueue.front();
    pieceQueue.pop();
    return next;
}