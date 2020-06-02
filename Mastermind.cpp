#include "Mastermind.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

char Mastermind::colors[6] = {'B', 'G', 'Y', 'R', 'W', 'K'};

Mastermind::Mastermind()
{
  initializeAllPossibleSolutions();
}

Mastermind::~Mastermind()
{
}

void Mastermind::printSolution(char solution[4], bool printEndLine)
{
  for (int i = 0; i < 4; i++)
  {
    cout << solution[i] << " ";
  }

  if (printEndLine)
    cout << endl;
}

void Mastermind::codeBreaker()
{
  int possibleSolutionIndex;

  // Start loop (only 10 guesses)
  for (int guessNum = 0; guessNum < 10; guessNum++)
  {
    // Get the next guess
    possibleSolutionIndex = guessNum == 0 ? getRandomSolutionIndex() : getPossibleSolution();

    // If there is no next guess
    if (possibleSolutionIndex == -1)
    {
      // fail and then break
      giveUp(gameRecord);
      return;
    }

    // Show the new guess
    cout << "Possible solutions: " << numPossibleSolutions << endl;
    cout << "Guess #" << guessNum + 1 << ": ";
    printSolution(possibleSolutions[possibleSolutionIndex].solution);

    // Record the guess
    for (int i = 0; i < 4; i++)
    {
      gameRecord.entries[guessNum].guess[i] = possibleSolutions[possibleSolutionIndex].solution[i];
    }

    // Ask for number of blacks or whites
    gameRecord.entries[guessNum].pegs = getUserPegs();

    gameRecord.length++;

    // If the code has been properly guessed,
    if (gameRecord.entries[guessNum].pegs.black == 4)
    {
      cout << "Computer wins! Better luck next time!" << endl;
      return;
    }

    // Eliminate impossible solutions
    eliminateImpossibleSolutions(gameRecord.entries[guessNum].guess, gameRecord.entries[guessNum].pegs);
  } // End loop
  giveUp(gameRecord);
  // fail
}

void Mastermind::codeMaker()
{
  char guess[4];
  char solution[4];
  Pegs pegs;
  // Randomly generate solution
  int randSolutionIndex = getRandomSolutionIndex();

  for (int i = 0; i < 4; i++)
  {
    solution[i] = possibleSolutions[randSolutionIndex].solution[i];
  }

  // Start loop (only 10 guesses from user)
  for (int guessNum = 0; guessNum < 10; guessNum++)
  {
    // Get the user's next guess
    cout << "Enter guess #" << guessNum + 1 << ": ";
    for (int i = 0; i < 4; i++)
    {
      cin >> guess[i];
    }

    // Calculate the blacks and whites
    pegs = calculatePegs(solution, guess);

    // If the code has been properly guessed
    if (pegs.isSolution())
    {
      cout << "Congrats, you beat the computer!" << endl;
      return;
    }
    // Display the blacks and whites
    pegs.print();
  }
  // Display losing message
  cout << "Computer's code: ";
  printSolution(solution, true);
  cout << "Computer wins! Better luck next time!" << endl;
}

void Mastermind::giveUp(GameRecord gameRecord)
{
  Pegs pegs;
  char solution[4];

  // Get the user's solution
  cout << "I give up, what is your code: ";
  for (int i = 0; i < 4; i++)
  {
    cin >> solution[i];
  }
  gameRecord.validateGame(solution);
}

Mastermind::Pegs Mastermind::getUserPegs()
{
  Pegs pegs;

  while (true)
  {
    cout << "Number of black pegs? ";
    cin >> pegs.black;

    cout << "Number of white pegs? ";
    cin >> pegs.white;
    cout << endl;

    if (pegs.isValid())
    {
      break;
    }

    cout << "You entered an incorrect number of pegs, please try again." << endl
         << endl;
  }

  return pegs;
}

void Mastermind::eliminateImpossibleSolutions(char guess[4], Pegs pegs)
{
  for (int i = 0; i < 1296; i++)
  {
    if (possibleSolutions[i].isPossible == true)
    {
      Pegs possiblePegs = calculatePegs(possibleSolutions[i].solution, guess);
      bool possible = possiblePegs == pegs;
      possibleSolutions[i].isPossible = possible;
      if (!possible)
      {
        numPossibleSolutions--;
      }
    }
  }
}

int Mastermind::getRandomSolutionIndex()
{
  const int MIN_VALUE = 0;
  const int MAX_VALUE = 1296;

  int solutionIndex;

  // Get system time
  unsigned seed = time(0);

  // Seed the random number generator
  srand(seed);

  solutionIndex = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;

  return solutionIndex;
}

int Mastermind::getPossibleSolution()
{
  for (int i = 0; i < 1296; i++)
  {
    if (possibleSolutions[i].isPossible == true)
    {
      return i;
    }
  }
  return -1;
}

void Mastermind::initializeAllPossibleSolutions()
{
  int solutionIndex = 0;

  for (int a = 0; a < 6; a++)
  {
    for (int b = 0; b < 6; b++)
    {
      for (int c = 0; c < 6; c++)
      {
        for (int d = 0; d < 6; d++)
        {
          possibleSolutions[solutionIndex].isPossible = true;
          possibleSolutions[solutionIndex].solution[0] = colors[a];
          possibleSolutions[solutionIndex].solution[1] = colors[b];
          possibleSolutions[solutionIndex].solution[2] = colors[c];
          possibleSolutions[solutionIndex].solution[3] = colors[d];
          solutionIndex++;
        }
      }
    }
  }

  possibleSolutionCount = solutionIndex;
}

Mastermind::Pegs Mastermind::calculatePegs(char solution[4], char guess[4])
{
  Pegs pegs;

  bool isSolutionUsed[4] = {false, false, false, false};

  // Find black-pegs first
  for (int g = 0; g < 4; g++)
  {
    if (guess[g] == solution[g])
    {
      pegs.black++;
      isSolutionUsed[g] = true;
    }
  }

  // Find white-pegs next
  for (int g = 0; g < 4; g++)
  {
    // Skip if it is a black-peg
    if (guess[g] == solution[g])
    {
      continue;
    }

    for (int s = 0; s < 4; s++)
    {
      // A white-peg cannot be the same index, so skip if it is the same index
      if (s == g)
      {
        continue;
      }

      if (guess[g] == solution[s] && !isSolutionUsed[s])
      {
        pegs.white++;
        isSolutionUsed[s] = true;
      }
    }
  }
  return pegs;
}
