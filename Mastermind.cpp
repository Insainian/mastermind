#include "Mastermind.h"

#include <iostream>
using namespace std;

char Mastermind::colors[6] = {'B', 'G', 'Y', 'R', 'W', 'K'};

Mastermind::Mastermind()
{
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

  // Ask user to write their code
  cout << "Write down a secret code consisting of a series of 4 colors." << endl;
  cout << "You can use the following colors: R, G, B, Y, W, K." << endl;
  cout << "Please put a white peg if the computer guesses a color right" << endl;
  cout << "and a black if it guesses a color and its position right" << endl
       << endl;

  initializeAllPossibleSolutions();

  // Start loop (only 10 guesses)
  for (int numGuess = 0; numGuess < 10; numGuess++)
  {
    // Get the next guess
    possibleSolutionIndex = getPossibleSolution();

    // If there is no next guess
    if (possibleSolutionIndex == -1)
    {
      // fail and then break
      giveUp(gameRecord);
      return;
    }

    // Show the new guess
    cout << "Guess #" << numGuess + 1 << ": ";
    printSolution(possibleSolutions[possibleSolutionIndex].solution);

    // Record the guess
    for (int i = 0; i < 4; i++)
    {
      gameRecord.entries[numGuess].guess[i] = possibleSolutions[possibleSolutionIndex].solution[i];
    }

    // Ask for number of blacks or whites
    gameRecord.entries[numGuess].pegs = getUserPegs();

    gameRecord.length++;

    // If the code has been properly guessed,
    if (gameRecord.entries[numGuess].pegs.black == 4)
    {
      // Display the record of the game
      cout << "Computer wins! Better luck next time!" << endl;
      return;
    }

    // Eliminate impossible solutions
    eliminateImpossibleSolutions(gameRecord.entries[numGuess].guess, gameRecord.entries[numGuess].pegs);
  } // End loop
  giveUp(gameRecord);
  // fail
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
    cout << "Please enter the number of black pegs: ";
    cin >> pegs.black;

    cout << "Please enter the number of white pegs: ";
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
      possibleSolutions[i].isPossible = possiblePegs == pegs;
    }
  }
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
