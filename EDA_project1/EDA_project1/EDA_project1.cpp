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
#include <algorithm> //if(binary_search(word.begin(),wordList.end(),Word)) {cout<<"Found\n"}; else {cout<<"Not found\n"};

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
typedef struct {
    char lin;
    char col;
    char dir;
} WordPosition;
//-------------------------------------------------------------------------------- 
typedef struct {
    WordPosition pos;
    string word;
} WordOnBoard;
//-------------------------------------------------------------------------------- 
typedef struct {
    vector<vector<char>> boardCells;
    int numLins;
    int numCols;
    vector<WordOnBoard> wordsOnBoard;
} Board;
//--------------------------------------------------------------------------------
typedef vector<string> WordList;



//================================================================================
// HELP FUNCTIONS 
//================================================================================
// Converts all characters of �s� to lowercase 
void tolowerStr(string& s) {
    for (auto& c : s)
        c = tolower(c);
}


//================================================================================
// Converts all characters of �s� to uppercase 
void toupperStr(string& s) 
{
    for (auto& c : s) c = toupper(c);
}


//================================================================================
// Initializes the board, all cells with �.� 
void initBoard(Board& board, int numLins, int numCols) 
{
    board.boardCells = vector<vector<char>>(numLins, vector<char>(numCols, '.'));
    board.numLins = numLins;
    board.numCols = numCols;
    board.wordsOnBoard.resize(0);
}


//================================================================================
// Shows the board �b� on the screen
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
// Gets a copy of the contents of a line, �lin�, of board �board� 
// and returns it through parameter �linContents� 
// The return value of the function is �true� if �lin� is valid, �false� otherwise
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

    if (position.dir == 'H') 
    {
        // Check if the word fits horizontally within the board boundaries
        if (colInt + word.size() > board.numCols) 
        {
            return false; 
        }

        // Check if there are any conflicting words in the chosen position
        for (int i = 0; i < word.size(); i++) 
        {
            char boardCell = board.boardCells[linInt + i][colInt];
            char closeCell = board.boardCells[linInt - 1 + i][colInt - 1];

            if (boardCell != '.' && boardCell != toupper(static_cast<char>(word[i])) && closeCell != '.')
            {
                return false;
            }
        }
    }
    else if (position.dir == 'V') 
    {
        // Check if the word fits vertically within the board boundaries
        if (linInt + word.size() > board.numLins) 
        {
            return false;
        }

        // Check if there are any conflicting words in the chosen position
        for (int i = 0; i < word.size(); i++) 
        {
            char boardCell = board.boardCells[linInt + i][colInt];
            char closeCell = board.boardCells[linInt - 1 + i ][colInt - 1];

            if (boardCell != '.' && boardCell != toupper(static_cast<char>(word[i])) && closeCell != '.')
            {
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
// Saves �board� into a text file 
void saveBoard(const Board& board) 
{
    // To do
}

//================================================================================
// Read the word list and return the number of words
int readWordList(WordList& availableWords)
{
    string fileName;
    cout << "Please input the file name (including the extension):" << endl;
    cin >> fileName;

    ifstream inStream(fileName);

    if (inStream.fail())
    {
        cerr << "Error opening " << fileName << "\n";
        return 0;
    }

    string word;
    while (inStream >> word)
    {
        availableWords.push_back(word);
    }

    return availableWords.size();
}

//================================================================================
// Read the board size and return if it is in the range 
bool readBoardSize(int& numLins, int& numCols) 
{
    if (numLins > 6 && numLins < 16 && numCols > 6 && numCols < 16) 
    {
        return true;
    }
    else 
    {
        cout << RED << "Invalid size! Choose a size in the 7 - 15 range " << NO_COLOR;
        return false;
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
    cout << "HELP \n";
    cout << "To choose the position you need to indicate as following (LcD): \n";
    cout << "- Line: with an uppercase letter \n - Column: with a lowercase letter\n - H for horizontal or V for vertical";
}

//================================================================================
void removeWords(Board& board) 
{
    // To do
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
                    showHelp(); //Create function to show help
                    break;
                case 'I':
                    insertWords(board, availableWords);
                    break;
                case 'R':
                    removeWords(board); // possible improvement (Create function to remove words)
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
