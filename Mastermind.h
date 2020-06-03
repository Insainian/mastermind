#ifndef __MASTERMIND_H__
#define __MASTERMIND_H__

#include <iostream>
using namespace std;

class Mastermind
{
public:
    static const int MAX_SOLUTIONS = 1296;
    static const int CODE_LENGTH = 4;
    static const int GAME_LENGTH = 10;
    static const int MAX_PEGS = CODE_LENGTH;
    static const int MIN_PEGS = 0;
    static const int MAX_COLORS = 6;

    struct Pegs
    {
        int black = 0;
        int white = 0;

        bool isValid()
        {
            int total = black + white;
            return (total >= MIN_PEGS && total <= MAX_PEGS);
        }

        bool isSolution()
        {
            return (black == MAX_PEGS && white == MIN_PEGS);
        }

        bool operator==(const Pegs &that)
        {
            if (this->black != that.black)
            {
                return false;
            }
            if (this->white != that.white)
            {
                return false;
            }

            return true;
        }

        void print()
        {
            cout << "Black Pegs: " << black << endl;
            cout << "White Pegs: " << white << endl;
        }
    };

    void codeBreaker();

    void codeMaker();

    int numPossibleSolutions = MAX_SOLUTIONS;

    static Pegs calculatePegs(char solution[], char guess[]);

    Mastermind();
    ~Mastermind();

private:
    struct PossibleSolution
    {
        char solution[CODE_LENGTH];
        bool isPossible;
    };

    struct GameRecordEntry
    {
        Pegs pegs;
        char guess[CODE_LENGTH];
        void print()
        {
            printSolution(guess, true);
            pegs.print();
            cout << endl;
        }

        bool validatePegsWithSolution(char solution[CODE_LENGTH])
        {
            Pegs expectedPegs = calculatePegs(solution, guess);
            return (pegs == expectedPegs);
        }
    };

    struct GameRecord
    {
        GameRecordEntry entries[GAME_LENGTH];
        int length = 0;
        void print()
        {
            for (int i = 0; i < length; i++)
            {
                cout << "Guess #" << i + 1 << ": ";
                entries[i].print();
            }
        }

        void validateGame(char solution[CODE_LENGTH])
        {
            for (int i = 0; i < length; i++)
            {
                if (!entries[i].validatePegsWithSolution(solution))
                {
                    cout << "You made a mistake on guess #" << i + 1 << ":" << endl;
                    cout << "You entered the following pegs: " << endl;
                    entries[i].print();
                    cout << "It should have been: " << endl;
                    calculatePegs(solution, entries[i].guess).print();
                    return;
                }
            }
            cout << "Congrats, you beat the code breaker!" << endl;
        }
    };

    static void printSolution(char solution[CODE_LENGTH], bool printEndLine = true);

    int getPossibleSolution();

    void initializeAllPossibleSolutions();

    void giveUp(GameRecord gameRecord);

    Pegs getUserPegs();

    void eliminateImpossibleSolutions(char guess[CODE_LENGTH], Pegs pegs);

    PossibleSolution possibleSolutions[MAX_SOLUTIONS];
    int possibleSolutionCount;

    GameRecord gameRecord;

    int getRandomSolutionIndex();

    static char colors[MAX_COLORS];

    char validateCode(char code[CODE_LENGTH]);
};

#endif // __MASTERMIND_H__
