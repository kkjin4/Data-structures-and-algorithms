/*
 * This function takes a Grid board of string and return the maximum score a player can get.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This helper function takes a string input and calculates its score then return.
 *
 */

int points(string str) {
    if(str.size() >= 4){
        return str.size() - 3;
    }
    return 0;
}

/*
 * This helper function takes a GridLocation of current location and a Grid as input
 * and calculates next possible steps from that location, it then returns all of the steps in a Set.
 *
 */
Set<GridLocation> generateNextLetter(GridLocation currentLoc, Grid<char>& board){
    Set<GridLocation> potentialLoc;
    for(int i = -1; i<=1; i++){
         for(int j = -1; j <= 1; j++){
             int newRow = currentLoc.row + i;
             int newCol = currentLoc.col + j;
             /*if(newRow >= 0 && newCol >= 0 && newRow < board.numRows() && newCol < board.numCols())*/
             if(board.inBounds(newRow,newCol)){
                 GridLocation nextLetter = {newRow, newCol};
                 if(nextLetter != currentLoc){
                     potentialLoc.add(nextLetter);
                 }
             }
         }
    }
    return potentialLoc;
}

/*
 * Note: At every steps, the helper function check the possible
 * moves from the current coordinates, then calls itself
 * for each of options.
 */

void findWord(string sofar, GridLocation currentLoc, Grid<char>& board, Lexicon& Lex, Set<string>& FindNow, Set<GridLocation>& visitedLoc, int& score){
    if(!Lex.containsPrefix(sofar)){
        return;
    }
    else if(Lex.contains(sofar) && sofar.size() >=4 && !FindNow.contains(sofar) ){
        FindNow.add(sofar);
        score += points(sofar);
    }
    Set<GridLocation> possibleLoc = generateNextLetter(currentLoc, board);
    possibleLoc -= visitedLoc;
      for(GridLocation nextLoc : possibleLoc){
          visitedLoc.add(nextLoc);
          char nextLetter = board[nextLoc];
          findWord(sofar+nextLetter, nextLoc, board, Lex, FindNow, visitedLoc, score);
          visitedLoc.remove(nextLoc);
      }

}

/*
 * This function takes a Grid board and Lexicon lex as input. It then iterates through each location of the Grid and runs
 * `findWord` to calculate the maximum possible score one can get in a boggle game.
 *
 */

int scoreBoard(Grid<char>& board, Lexicon& lex) {
    Set<string> sofarWord;
    int score = 0;
    for(int i = 0; i< board.numRows(); i++){
         for(int j = 0; j < board.numCols(); j++){
                 char currentLetter = board[i][j];
                 string currentStr = charToString(currentLetter);
                 GridLocation currentLoc = {i,j};
                 Set<GridLocation> visitedLoc = {{currentLoc}};
                 findWord(currentStr, currentLoc, board, lex,sofarWord, visitedLoc, score);
         }
    }
    return score;
   }



/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("Test point scoring") {
    EXPECT_EQUAL(points("a"), 0);
    EXPECT_EQUAL(points("hello"), 2);
    EXPECT_EQUAL(points("good"), 1);
    EXPECT_EQUAL(points("everyday"), 5);
}


