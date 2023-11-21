//================================================================================
// WORD GAMES - BOARD MAKER
// 
// GROUP 14
// MEMBERS:
//  Gabriele Rocha de Carvalho (up202302557)
//  Hannia Valentina Espinoza Reyes (up202302569)
//================================================================================ 



#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

//-------------------------------------------------------------------------------- 
using namespace std;

//================================================================================ 
// COLOR CODES / ANSI ESCAPE SEQUENCES
//================================================================================ 
// TEXT COLOR
#define NO_COLOR      "\033[0m"
#define BLACK         "\033[0;30m"
#define RED           "\033[0;31m"
#define GREEN         "\033[0;32m"
#define BROWN         "\033[0;33m"
#define BLUE          "\033[0;34m"
#define MAGENTA       "\033[0;35m"
#define CYAN          "\033[0;36m"
#define LIGHTGRAY     "\033[0;37m"
#define DARKGRAY      "\033[1;30m"
#define LIGHTRED      "\033[1;31m"
#define LIGHTGREEN    "\033[1;32m"
#define YELLOW        "\033[1;33m"
#define LIGHTBLUE     "\033[1;34m"
#define LIGHTMAGENTA  "\033[1;35m"
#define LIGHTCYAN     "\033[1;36m"
#define WHITE         "\033[1;37m"

// BACKGROUND COLOR
#define BLACK_B       "\033[0;40m"
#define RED_B         "\033[0;41m"
#define GREEN_B       "\033[0;42m"
#define YELLOW_B      "\033[0;43m"
#define BLUE_B        "\033[0;44m"
#define MAGENTA_B     "\033[0;45m"
#define CYAN_B        "\033[0;46m"
#define WHITE_B       "\033[1;47m"



//================================================================================ 
// USER DEFINED TYPES
//================================================================================ 

// Represents the position of a word on the board: line, column, and direction (H or V)
typedef struct {
    char lin;
    char col;
    char dir;
} WordPosition;

//-------------------------------------------------------------------------------- 

// Represents a word on the board, including its position and the word itself
typedef struct {
    WordPosition pos;
    string word;
} WordOnBoard;

//-------------------------------------------------------------------------------- 

// Represents the game board, including the cells, dimensions, and words placed on it
typedef struct {
    vector<vector<char>> boardCells; // 2D vector representing the cells of the board
    int numLins;                     // Number of lines in the board
    int numCols;                     // Number of columns in the board
    vector<WordOnBoard> wordsOnBoard; // List of words placed on the board
} Board;

//--------------------------------------------------------------------------------

// Represents a list of words
typedef vector<string> WordList;


//================================================================================
// HELP FUNCTIONS 
// 
//================================================================================
// Converts all characters of ‘s’ to lowercase 
void tolowerStr(string& s) {
    for (auto& c : s)
        c = tolower(c);
}


//================================================================================
// Converts all characters of ‘s’ to uppercase 
void toupperStr(string& s) 
{
    for (auto& c : s) 
        c = toupper(c);
}


//================================================================================
// Initializes the board, all cells with ‘.’ 
void initBoard(Board& board, int numLins, int numCols) 
{
    board.boardCells = vector<vector<char>>(numLins, vector<char>(numCols, '.'));
    board.numLins = numLins;
    board.numCols = numCols;
    board.wordsOnBoard.resize(0);
}


//================================================================================
// Shows the board ‘b’ on the screen
// The line and column identifying letters are colored in red 
void showBoard(const Board& b) 
{
    cout << "\n\n";
    cout << RED << "  ";
    for (size_t j = 0; j < b.boardCells.at(0).size(); j++)
        cout << (char)('a' + j) << ' ';
    cout << endl;
    cout << NO_COLOR;
    for (size_t i = 0; i < b.boardCells.size(); i++) 
    {
        cout << RED << (char)('A' + i) << ' ' << NO_COLOR;
        for (size_t j = 0; j < b.boardCells.at(i).size(); j++)
            cout << b.boardCells.at(i).at(j) << ' ';
        cout << endl;
    }
    cout << endl;
}


//================================================================================
// Gets a copy of the contents of a line, ‘lin’, of board ‘board’ 
// and returns it through parameter ‘linContents’ 
// The return value of the function is ‘true’ if ‘lin’ is valid, ‘false’ otherwise
bool getBoardLin(const Board& board, char lin, string& linContents) 
{
    int linInt = lin - 'A';
    if (linInt < 0 || linInt >= board.numLins)
        return false;
    linContents = "";
    for (size_t colInt = 0; colInt < board.numCols; colInt++)
        linContents += board.boardCells.at(linInt).at(colInt);
    return true;
}

//================================================================================
// Verify if the word is in the list 
bool isWordInList(const string word, WordList availableWords)
{
    for (const string validWord : availableWords)
    {
        if (word == validWord)
        {
            return true;
        }
    }
    return false;
}

//================================================================================
// Check if it is possible to insert the word in the chosen position
bool isInsertionPossible(WordPosition& position, Board& board, const string& word) {
    int linInt = position.lin - 'A';
    int colInt = position.col - 'a';

    if (position.dir == 'H') {
        // Check if the word fits horizontally within the board boundaries
        if (colInt + word.size() > board.numCols) {
            return false;
        }

        // Check for conflicts with existing letters horizontally
        for (int i = 0; i < word.size(); i++) {
            char boardCell = board.boardCells[linInt][colInt + i];

            if (boardCell != '.' && boardCell != toupper(static_cast<char>(word[i]))) {
                return false;
            }
        }
    }
    else if (position.dir == 'V') {
        // Check if the word fits vertically within the board boundaries
        if (linInt + word.size() > board.numLins) {
            return false;
        }

        // Check for conflicts with existing letters vertically
        for (int i = 0; i < word.size(); i++) {
            char boardCell = board.boardCells[linInt + i][colInt];

            if (boardCell != '.' && boardCell != toupper(static_cast<char>(word[i]))) {
                return false;
            }
        }
    }
    return true;
}

//================================================================================
void insertWords(Board& board, WordList& availableWords)
{
    WordPosition position;
    WordOnBoard word;
    string inserting; //Variable to STOP

    cout << "\nINSERTING WORDS ...\n\n";
    showBoard(board);

    while (true) 
    {
        cout << "\nPosition(LcD / STOP) ? ";
        cin >> inserting;

        if (inserting == "STOP") 
        {
            break;
        }

        bool isValid = true;  // Variable to check the validity of the position

        // Check if the line is within limits
        if (inserting[0] < 'A' || inserting[0] >= ('A' + board.numLins) || inserting[0] == tolower(inserting[0]))
        {
            cout << RED << "Invalid line!\n" << NO_COLOR;
            isValid = false;
        }

        // Check if the column is within limits
        if (inserting[1] < 'a' || inserting[1] >= ('a' + board.numCols) || inserting[1] == toupper(inserting[1]))
        {
            cout << RED << "Invalid column!\n" << NO_COLOR;
            isValid = false;
        }

        // Check if the direction is 'H' or 'V'
        if (inserting[2] != 'H' && inserting[2] != 'V' || inserting[2] == tolower(inserting[2]))
        {
            cout << RED << "Invalid direction!\n" << NO_COLOR;
            isValid = false;
        }

        if (!isValid) 
        {
            continue;  // Return to beginning of loop for new valid input
        }

        //Convert string to WordPosition
        position.lin = inserting[0];
        position.col = inserting[1];
        position.dir = inserting[2];

        cout << "Word ? ";
        cin >> word.word;
        
        // Convert the word to lower
        tolowerStr(word.word);
       
        // Check if the word is in the list and if it is possible to insert the word in the chosen position
        if (!isWordInList(word.word, availableWords) || !isInsertionPossible(position, board, word.word))
        {
            cout << RED << "Could not put word on board !" << NO_COLOR;
            continue;
        }

        // Convert the word to upper
        toupperStr(word.word);

        // Add the word to the board
        word.pos = position;
        board.wordsOnBoard.push_back(word);

        // Update the board cells with the entered word
        int linInt = position.lin - 'A';
        int colInt = position.col - 'a';
        if (position.dir == 'H') 
        {
            for (char c : word.word) 
            {
                board.boardCells[linInt][colInt] = c;
                colInt++;
            }
        }
        else if (position.dir == 'V') 
        {
            for (char c : word.word) 
            {
                board.boardCells[linInt][colInt] = c;
                linInt++;
            }
        }

        showBoard(board);
    }
}

//================================================================================
// Read the word list and return the number of words
int readWordList(WordList& availableWords)
{
    string fileName;
    cout << "Please input the file name (including the extension):" << endl;
    cin >> fileName;

    ifstream inStream(fileName); // Open a file stream to read from the specified file

    // Check if the file stream is in a fail state, indicating an error opening the file
    if (inStream.fail())
    {
        cerr << "Error opening " << fileName << "\n"; // Display an error message
        return 0; // Return 0 to indicate failure (no words read)
    }

    string word;

    // Read words from the file stream until the end of the file is reached
    while (inStream >> word)
    {
        availableWords.push_back(word); // Add each word to the 'availableWords' vector
    }

    return availableWords.size(); // Return the number of words read from the file
}


//================================================================================
// Read the board size and return if it is in the range 
bool readBoardSize(int& numLins, int& numCols)
{
    // Check if both the number of lines and columns are within the valid range (7 - 15)
    if (numLins > 6 && numLins < 16 && numCols > 6 && numCols < 16)
    {
        return true; // Size is valid, return true
    }
    else
    {
        cout << RED << "Invalid size! Choose a size in the 7 - 15 range " << NO_COLOR;
        // Display an error message for an invalid size
        return false; // Return false to indicate an invalid size
    }
}

//================================================================================
// Read option of the user
char readOption() 
{
    char option;
    cout << "\n\nH - Help \nI - Insert words \nR - Remove words \nE - Exit \nOption? ";
    cin >> option;
    return option;
}

//================================================================================
void showHelp() 
{
    cout << BLUE << "\nHELP \n" << NO_COLOR;
    cout << BLUE << "To choose the position you need to indicate as following (LcD): \n" << NO_COLOR;
    cout << BLUE << "- Line: with an uppercase letter \n- Column: with a lowercase letter\n- H for horizontal or V for vertical" << NO_COLOR;
}

//================================================================================
// Removes a word from the board based on user input.
void removeWords(Board& board)
{
    string wordToRemove;

    cout << "\nEnter the word you want to remove: ";
    cin >> wordToRemove;

    toupperStr(wordToRemove); // Convert the input word to uppercase for case-insensitive comparison

    // Iterate through the words on the board to find the word to remove
    for (auto it = board.wordsOnBoard.begin(); it != board.wordsOnBoard.end(); ++it) {
        // Check if the current word on the board matches the word to remove
        if (it->word == wordToRemove) {
            int linInt = it->pos.lin - 'A';
            int colInt = it->pos.col - 'a';

            // Iterate through each letter of the word to update the board
            for (int i = 0; i < it->word.size(); i++) {
                // Determine the board cell corresponding to the current letter of the word
                char& boardCell = it->pos.dir == 'H' ? board.boardCells[linInt][colInt + i] : board.boardCells[linInt + i][colInt];

                // Check if there are other words sharing the same letter at the current position
                bool sharedLetter = false;
                for (const auto& otherWord : board.wordsOnBoard) {
                    if (&otherWord != &(*it) && i < otherWord.word.size()) {
                        // Determine the board cell of the other word at the same position
                        char otherCell = otherWord.pos.dir == 'H' ? board.boardCells[otherWord.pos.lin - 'A'][otherWord.pos.col - 'a' + i] : board.boardCells[otherWord.pos.lin - 'A' + i][otherWord.pos.col - 'a'];

                        // Check if the other word shares the same letter at the current position
                        if (otherCell == it->word[i]) {
                            sharedLetter = true;
                            break;
                        }
                    }
                }

                // Remove the letter only if it does not belong to other words
                if (!sharedLetter) {
                    boardCell = '.';
                }
            }

            // Remove the word from the list of words on the board
            board.wordsOnBoard.erase(it);

            cout << GREEN << "Word '" << wordToRemove << "' removed from the board." << NO_COLOR << endl;
            return; // Exit the function after successfully removing the word
        }
    }

    cout << "Word '" << wordToRemove << "' not found on the board." << endl;
    // Display a message if the specified word is not found on the board
}


//================================================================================
// Saves ‘board’ into a text file 
void saveBoard(const Board& board)
{
    // Read the current board number from the auxiliary file
    int boardNumber;
    ifstream boardNumFile("board_num.txt");
    if (boardNumFile.is_open()) {
        boardNumFile >> boardNumber;
        boardNumFile.close();
    }
    else {
        cerr << "Error opening board_num.txt for reading.\n";
        return;
    }

    // Increment the board number for the next board
    boardNumber++;

    // Save the updated board number back to the auxiliary file
    ofstream boardNumFileOut("board_num.txt");
    if (boardNumFileOut.is_open()) {
        boardNumFileOut << boardNumber;
        boardNumFileOut.close();
    }
    else {
        cerr << RED << "Error opening board_num.txt for writing.\n" << NO_COLOR;
        return;
    }

    // Create the filename for the current board
    string fileName = "b";
    if (boardNumber < 10) {
        fileName += "00";
    }
    else if (boardNumber < 100) {
        fileName += "0";
    }
    fileName += to_string(boardNumber) + ".txt";

    // Open a file for writing
    ofstream outFile(fileName);

    if (outFile.is_open()) // Check if the file is successfully opened
    {
        // Write board size to the file
        outFile << board.numLins << " " << board.numCols << "\n";

        // Write board cells to the file
        for (size_t i = 0; i < board.boardCells.size(); i++)
        {
            for (size_t j = 0; j < board.boardCells.at(i).size(); j++)
                outFile << board.boardCells.at(i).at(j) << ' ';
            outFile << "\n";
        }

        // Write words on the board to the file
        for (const auto& wordOnBoard : board.wordsOnBoard)
        {
            outFile << wordOnBoard.pos.lin << wordOnBoard.pos.col << wordOnBoard.pos.dir << " " << wordOnBoard.word << "\n";
        }

        outFile.close(); // Close the file
        cout << GREEN << "Board saved to " << fileName << NO_COLOR << endl;
    }
    else
    {
        cerr << RED << "Error opening file for writing: " << fileName << "\n" << NO_COLOR;
    }
}

//================================================================================
//================================================================================
//================================================================================
int main() 
{
    cout << "WORD GAMES - BOARD MAKER\n\n";

    WordList availableWords;

    unsigned numWords = readWordList(availableWords); 

    if (numWords > 0) 
    {
        int numLins, numCols;
        cout << "Board size (lines columns) ? ";
        cin >> numLins >> numCols;
        bool validBoardSize = readBoardSize(numLins, numCols); 

        if (validBoardSize) 
        {
            bool endOfOperations = false;
            Board board;
            initBoard(board, numLins, numCols);

            do 
            {
                char option = readOption(); 
                switch (option) 
                {
                case 'H':
                    showHelp();
                    break;
                case 'I':
                    insertWords(board, availableWords);
                    break;
                case 'R':
                    removeWords(board); 
                    break;
                case 'E':
                    endOfOperations = true;
                    break;
                }
            } while (!endOfOperations);

            saveBoard(board);
        }
    }

    return 0;
}
