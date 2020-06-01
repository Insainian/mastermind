#ifndef __MASTERMIND_H__
#define __MASTERMIND_H__

#include <iostream>
using namespace std;

class Mastermind
{
public:
    struct Pegs
    {
        int black = 0;
        int white = 0;

        bool isValid()
        {
            int total = black + white;
            return (total >= 0 && total <= 4);
        }

        bool isSolution()
        {
            return (black == 4 && white == 0);
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

    static Pegs calculatePegs(char solution[], char guess[]);

    Mastermind();
    ~Mastermind();

private:
    struct PossibleSolution
    {
        char solution[4];
        bool isPossible;
    };

    struct GameRecordEntry
    {
        Pegs pegs;
        char guess[4];
        void print()
        {
            printSolution(guess, true);
            pegs.print();
            cout << endl;
        }

        bool validatePegsWithSolution(char solution[4])
        {
            Pegs expectedPegs = calculatePegs(solution, guess);
            return (pegs == expectedPegs);
        }
    };

    struct GameRecord
    {
        GameRecordEntry entries[10];
        int length = 0;
        void print()
        {
            for (int i = 0; i < length; i++)
            {
                cout << "Guess #" << i + 1 << ": ";
                entries[i].print();
            }
        }

        void validateGame(char solution[4])
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

    static void printSolution(char solution[4], bool printEndLine = true);

    int getPossibleSolution();

    void initializeAllPossibleSolutions();

    void giveUp(GameRecord gameRecord);

    Pegs getUserPegs();

    PossibleSolution possibleSolutions[1296];
    int possibleSolutionCount;

    GameRecord gameRecord;

    static char colors[6];
};

#endif // __MASTERMIND_H__
