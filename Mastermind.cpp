#include "Mastermind.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

char Mastermind::colors[MAX_COLORS] = {'B', 'G', 'Y', 'R', 'W', 'K'};

Mastermind::Mastermind()
{
  initializeAllPossibleSolutions();
}

Mastermind::~Mastermind()
{
}

void Mastermind::printSolution(char solution[CODE_LENGTH], bool printEndLine)
{
  for (int i = 0; i < CODE_LENGTH; i++)
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
  for (int guessNum = 0; guessNum < GAME_LENGTH; guessNum++)
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
    for (int i = 0; i < CODE_LENGTH; i++)
    {
      gameRecord.entries[guessNum].guess[i] = possibleSolutions[possibleSolutionIndex].solution[i];
    }

    // Ask for number of blacks or whites
    gameRecord.entries[guessNum].pegs = getUserPegs();

    gameRecord.length++;

    // If the code has been properly guessed,
    if (gameRecord.entries[guessNum].pegs.black == MAX_PEGS)
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
  char guess[CODE_LENGTH];
  char solution[CODE_LENGTH];
  char errorColor;
  Pegs pegs;
  // Randomly generate solution
  int randSolutionIndex = getRandomSolutionIndex();

  for (int i = 0; i < CODE_LENGTH; i++)
  {
    solution[i] = possibleSolutions[randSolutionIndex].solution[i];
  }

  // Start loop (only 10 guesses from user)
  for (int guessNum = 0; guessNum < GAME_LENGTH; guessNum++)
  {
    // Get the user's next guess
    do
    {
      cout << "Enter guess #" << guessNum + 1 << ": ";
      for (int i = 0; i < CODE_LENGTH; i++)
      {
        cin >> guess[i];
      }

      errorColor = validateCode(guess);

      if (errorColor != ' ')
      {
        cout << endl;
        cout << "You entered a wrong color: " << errorColor << endl;
        cout << "Please only enter one of the following colors: B, G, Y, R, W, K" << endl
             << endl;
      }

    } while (errorColor != ' ');

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
    cout << endl;
  }
  // Display losing message
  cout << "Computer wins! Better luck next time!" << endl;
  cout << "Computer's code: ";
  printSolution(solution, true);
}

void Mastermind::giveUp(GameRecord gameRecord)
{
  Pegs pegs;
  char solution[CODE_LENGTH];

  // Get the user's solution
  cout << "I give up, what is your code: ";
  for (int i = 0; i < CODE_LENGTH; i++)
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

void Mastermind::eliminateImpossibleSolutions(char guess[CODE_LENGTH], Pegs pegs)
{
  for (int i = 0; i < MAX_SOLUTIONS; i++)
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
  const int MAX_VALUE = MAX_SOLUTIONS - 1;

  int solutionIndex;

  // Get system time
  unsigned seed = time(0);

  // Seed the random number generator
  srand(seed);

  solutionIndex = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;

  return solutionIndex;
}

char Mastermind::validateCode(char code[CODE_LENGTH])
{
  bool found = false;
  for (int c = 0; c < CODE_LENGTH; c++)
  {
    found = false;
    for (int s = 0; s < MAX_COLORS; s++)
    {
      if (code[c] == colors[s])
      {
        found = true;
      }
    }
    if (!found)
    {
      return code[c];
    }
  }
  return ' ';
}

int Mastermind::getPossibleSolution()
{
  for (int i = 0; i < MAX_SOLUTIONS; i++)
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

  for (int a = 0; a < MAX_COLORS; a++)
  {
    for (int b = 0; b < MAX_COLORS; b++)
    {
      for (int c = 0; c < MAX_COLORS; c++)
      {
        for (int d = 0; d < MAX_COLORS; d++)
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

Mastermind::Pegs Mastermind::calculatePegs(char solution[CODE_LENGTH], char guess[CODE_LENGTH])
{
  Pegs pegs;

  bool isSolutionUsed[CODE_LENGTH] = {false, false, false, false};

  // Find black-pegs first
  for (int g = 0; g < CODE_LENGTH; g++)
  {
    if (guess[g] == solution[g])
    {
      pegs.black++;
      isSolutionUsed[g] = true;
    }
  }

  // Find white-pegs next
  for (int g = 0; g < CODE_LENGTH; g++)
  {
    // Skip if it is a black-peg
    if (guess[g] == solution[g])
    {
      continue;
    }

    for (int s = 0; s < CODE_LENGTH; s++)
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
        break;
      }
    }
  }
  return pegs;
}
