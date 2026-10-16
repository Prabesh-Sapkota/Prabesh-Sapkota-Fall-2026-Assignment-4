/*
 * Program: EECS 348 Assignment 4
 * Description: Reads and solves five Sudoku puzzles using recursive backtracking.
 * Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt, puzzle4.txt, puzzle5.txt.
 * Output: Each original puzzle, every solution found, or an appropriate error message.
 * Collaborators: None.
 * Other sources: ChatGPT (OpenAI).
 * Author: Prabesh Sapkota
 * Creation Date: October 15, 2026
 */

#include <iostream> // Provides console input and output.
#include <fstream> // Provides file-reading tools.
#include <string> // Provides the string type.
#include <vector> // Stores and validates the puzzle's 81 input cells.
#include <cctype> // Provides the isspace function.
using namespace std; // Allows standard-library names without the std:: prefix.

class SudokuSolver { // Defines a class that stores and solves one Sudoku puzzle.
private: // Begins the private section of the class.
    int board[9][9]; // Stores the Sudoku grid; zero represents an empty cell.
    int solutionCount; // Counts how many complete solutions have been found.

    bool isSafe(int row, int col, int digit) { // Checks whether digit can go in the specified cell.
        for (int j = 0; j < 9; j++) { // Checks all cells in the selected row.
            if (board[row][j] == digit) { // Detects the same digit in that row.
                return false; // Rejects a digit that already appears in the row.
            } // Ends the row-conflict condition.
        } // Ends the row scan.

        for (int i = 0; i < 9; i++) { // Checks all cells in the selected column.
            if (board[i][col] == digit) { // Detects the same digit in that column.
                return false; // Rejects a digit that already appears in the column.
            } // Ends the column-conflict condition.
        } // Ends the column scan.

        int startRow = (row / 3) * 3; // Finds the first row of the cell's 3-by-3 block.
        int startCol = (col / 3) * 3; // Finds the first column of the cell's 3-by-3 block.

        for (int i = startRow; i < startRow + 3; i++) { // Visits the three rows of the block.
            for (int j = startCol; j < startCol + 3; j++) { // Visits the three columns of the block.
                if (board[i][j] == digit) { // Detects the same digit inside the block.
                    return false; // Rejects a digit that already appears in the block.
                } // Ends the block-conflict condition.
            } // Ends the inner block scan.
        } // Ends the outer block scan.

        return true; // Confirms that the digit follows all Sudoku placement rules.
    } // Ends isSafe.

    void printBoard() const { // Prints the current Sudoku grid without modifying it.
        for (int i = 0; i < 9; i++) { // Visits each row of the board.
            if (i % 3 == 0) { // Checks whether a new group of three rows begins.
                cout << "+-------+-------+-------+" << endl; // Prints the horizontal grid border.
            } // Ends the row-group condition.

            for (int j = 0; j < 9; j++) { // Visits each cell in the current row.
                if (j % 3 == 0) { // Checks whether a new group of three columns begins.
                    cout << "| "; // Prints the left boundary of a 3-column group.
                } // Ends the column-group condition.

                if (board[i][j] == 0) { // Checks whether the current cell is empty.
                    cout << "_ "; // Displays an underscore for an empty cell.
                } else { // Handles a cell containing a digit.
                    cout << board[i][j] << " "; // Displays the digit stored in the cell.
                } // Ends the empty/non-empty condition.
            } // Ends the scan of the current row.

            cout << "|" << endl; // Prints the right boundary and starts a new output line.
        } // Ends the scan of all rows.

        cout << "+-------+-------+-------+" << endl; // Prints the bottom border of the grid.
    } // Ends printBoard.

    bool findBestEmptyCell(int& bestRow, int& bestCol) { // Finds an empty cell with the fewest legal candidates.
        int fewestCandidates = 10; // Starts above the maximum possible candidate count of nine.
        bestRow = -1; // Indicates that no empty cell has been selected yet.
        bestCol = -1; // Indicates that no empty cell has been selected yet.

        for (int row = 0; row < 9; row++) { // Searches each row of the board.
            for (int col = 0; col < 9; col++) { // Searches each column of the current row.
                if (board[row][col] != 0) { // Skips cells that already contain a digit.
                    continue; // Moves on to the next cell.
                } // Ends the non-empty-cell condition.

                int candidateCount = 0; // Counts legal digits for this empty cell.

                for (int digit = 1; digit <= 9; digit++) { // Tries each Sudoku digit as a candidate.
                    if (isSafe(row, col, digit)) { // Checks whether the candidate obeys Sudoku rules.
                        candidateCount++; // Counts this legal candidate.
                    } // Ends the candidate-validity condition.
                } // Ends the candidate-counting loop.

                if (candidateCount == 0) { // Detects an empty cell with no legal candidate.
                    bestRow = row; // Saves the contradictory cell's row.
                    bestCol = col; // Saves the contradictory cell's column.
                    return true; // Signals that an empty cell exists, but it has no possible value.
                } // Ends the no-candidate condition.

                if (candidateCount < fewestCandidates) { // Checks whether this cell has fewer candidates than the best so far.
                    fewestCandidates = candidateCount; // Records the new lowest candidate count.
                    bestRow = row; // Records the row of the best candidate cell.
                    bestCol = col; // Records the column of the best candidate cell.

                    if (fewestCandidates == 1) { // Checks whether this cell has only one possible digit.
                        return true; // A single-candidate cell cannot be improved upon.
                    } // Ends the single-candidate condition.
                } // Ends the best-cell update condition.
            } // Ends the scan of the current row.
        } // Ends the scan of the board.

        return bestRow != -1; // Returns true if an empty cell was found, otherwise the board is complete.
    } // Ends findBestEmptyCell.

    void solveRecursive() { // Searches recursively for all possible Sudoku solutions.
        int emptyRow = -1; // Holds the row of the selected empty cell.
        int emptyCol = -1; // Holds the column of the selected empty cell.

        if (!findBestEmptyCell(emptyRow, emptyCol)) { // Checks whether there are no empty cells left.
            solutionCount++; // Counts the completed valid board as a solution.
            cout << "\nSolution " << solutionCount << ":" << endl; // Prints the solution number.
            printBoard(); // Displays the completed Sudoku solution.
            return; // Backtracks to search for any additional solutions.
        } // Ends the completed-board condition.

        if (emptyRow == -1 || emptyCol == -1) { // Checks for an invalid selection state defensively.
            return; // Stops this branch if no valid cell coordinates were selected.
        } // Ends the defensive coordinate check.

        int candidateCount = 0; // Counts legal values for the selected cell.

        for (int digit = 1; digit <= 9; digit++) { // Tests each digit for the selected cell.
            if (isSafe(emptyRow, emptyCol, digit)) { // Checks whether the digit is legal in this cell.
                candidateCount++; // Records that at least one valid candidate exists.
                board[emptyRow][emptyCol] = digit; // Places the candidate digit in the board.
                solveRecursive(); // Solves the remaining empty cells using recursion.
                board[emptyRow][emptyCol] = 0; // Removes the candidate to try another possibility.
            } // Ends the valid-candidate condition.
        } // Ends the digit-trial loop.

        if (candidateCount == 0) { // Identifies a branch where no digit can fit the selected cell.
            return; // Abandons this branch so the previous recursive call can backtrack.
        } // Ends the dead-end condition.
    } // Ends solveRecursive.

public: // Begins the public section of the class.
    SudokuSolver() : solutionCount(0) { // Initializes the solution counter when an object is created.
        for (int row = 0; row < 9; row++) { // Visits each row during initialization.
            for (int col = 0; col < 9; col++) { // Visits each column during initialization.
                board[row][col] = 0; // Marks every cell as empty.
            } // Ends the initialization of one row.
        } // Ends board initialization.
    } // Ends the constructor.

    bool loadPuzzle(const string& filename) { // Loads and validates the basic format of a puzzle file.
        ifstream inputFile(filename); // Opens the specified file for reading.

        if (!inputFile.is_open()) { // Checks whether the file opened successfully.
            cerr << "Error: Could not open " << filename << endl; // Reports a missing or inaccessible file.
            return false; // Stops loading when the file cannot be opened.
        } // Ends the file-open check.

        vector<char> cells; // Temporarily stores all 81 cell symbols before changing the board.
        char ch; // Stores each character read from the file.

        while (inputFile.get(ch)) { // Reads the file one character at a time.
            if ((ch >= '1' && ch <= '9') || ch == '_') { // Accepts digits one through nine and underscores.
                cells.push_back(ch); // Saves a valid Sudoku cell symbol.
            } else if (ch == '0') { // Allows zero as another way to mark an empty cell.
                cells.push_back('_'); // Normalizes zero to the underscore blank marker.
            } else if (!isspace(static_cast<unsigned char>(ch))) { // Rejects unexpected non-whitespace characters.
                cerr << "Error: Invalid character in " << filename << endl; // Reports an unexpected character.
                return false; // Stops loading a malformed puzzle file.
            } // Ends the character-validation branches.
        } // Ends the file-reading loop.

        if (cells.size() != 81) { // Requires exactly 81 cell symbols for a 9-by-9 puzzle.
            cerr << "Error: " << filename << " must contain exactly 81 Sudoku cells." << endl; // Reports the incorrect cell count.
            return false; // Rejects a puzzle with too few or too many cells.
        } // Ends the cell-count check.

        for (int row = 0; row < 9; row++) { // Copies the validated symbols into each board row.
            for (int col = 0; col < 9; col++) { // Copies the symbols into each board column.
                char cell = cells[row * 9 + col]; // Retrieves the symbol at the current linear position.

                if (cell == '_') { // Checks whether the symbol represents a blank cell.
                    board[row][col] = 0; // Stores blank cells internally as zero.
                } else { // Handles a symbol containing a digit.
                    board[row][col] = cell - '0'; // Converts the digit character into its integer value.
                } // Ends the cell-conversion condition.
            } // Ends copying one row.
        } // Ends copying all 81 cells.

        return true; // Confirms that the file was read and formatted correctly.
    } // Ends loadPuzzle.

    bool isInitialBoardValid() { // Checks that the puzzle's original digits do not conflict.
        for (int row = 0; row < 9; row++) { // Examines each row of the starting puzzle.
            for (int col = 0; col < 9; col++) { // Examines each cell in the current row.
                if (board[row][col] == 0) { // Checks whether the current cell is blank.
                    continue; // Skips blank cells because they cannot conflict with a given digit.
                } // Ends the blank-cell condition.

                int digit = board[row][col]; // Saves the digit currently being checked.
                board[row][col] = 0; // Temporarily clears the cell so it does not conflict with itself.
                bool valid = isSafe(row, col, digit); // Checks the saved digit against its row, column, and block.
                board[row][col] = digit; // Restores the original digit after checking it.

                if (!valid) { // Checks whether another given digit conflicts with this one.
                    return false; // Rejects the starting puzzle if a conflict is found.
                } // Ends the conflict condition.
            } // Ends the scan of the current row.
        } // Ends the scan of the starting board.

        return true; // Confirms that all given digits are mutually consistent.
    } // Ends isInitialBoardValid.

    void solve(const string& filename) { // Prints a puzzle and searches for all its solutions.
        cout << "\n========================================" << endl; // Prints a separator before the puzzle heading.
        cout << "Puzzle file: " << filename << endl; // Identifies the file being processed.
        cout << "========================================" << endl; // Prints a separator after the puzzle heading.

        cout << "\nOriginal puzzle:" << endl; // Labels the original, unsolved puzzle.
        printBoard(); // Displays the board as it was loaded from the file.

        if (!isInitialBoardValid()) { // Checks for conflicting given digits before searching.
            cout << "\nNo solution found: the original puzzle has conflicting digits." << endl; // Explains why solving was stopped.
            return; // Skips backtracking for an invalid starting board.
        } // Ends the original-board validation condition.

        solutionCount = 0; // Resets the solution count before this search begins.
        solveRecursive(); // Starts recursive backtracking with the selected-cell heuristic.

        if (solutionCount == 0) { // Checks whether the search found no complete solution.
            cout << "\nNo solution found." << endl; // Reports that no valid solution exists.
        } else { // Handles a puzzle for which one or more solutions were found.
            cout << "\nTotal solutions found: " << solutionCount << endl; // Displays the number of solutions found.
        } // Ends the solution-result condition.
    } // Ends solve.
}; // Ends the SudokuSolver class definition.

int main() { // Defines the program's starting point.
    const string filenames[] = {"puzzle1.txt", "puzzle2.txt", "puzzle3.txt", "puzzle4.txt", "puzzle5.txt"}; // Lists the five required puzzle files.

    for (const string& filename : filenames) { // Processes each required filename in sequence.
        SudokuSolver solver; // Creates a fresh solver so puzzles do not share board state.

        if (solver.loadPuzzle(filename)) { // Loads the current file and continues only if its format is valid.
            solver.solve(filename); // Validates and solves the loaded puzzle.
        } // Ends the load-success condition.
    } // Ends processing of the five puzzle files.

    return 0; // Reports successful completion of the program.
} // Ends main.