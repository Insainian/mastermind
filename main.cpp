#include "Mastermind.h"

#include <iostream>
using namespace std;

void menu();
void howToPlay();

int main()
{
  Mastermind mastermind;
  int choice;

  cout << "Welcome to Mastermind!" << endl;
  do
  {
    cout << endl;
    menu();
    cin >> choice;

    cout << endl;
    switch (choice)
    {
    case 1:
      howToPlay();
      break;
    case 2:
      mastermind.codeBreaker();
      break;
    case 3:
      mastermind.codeMaker();
      break;
    case 9:
      break;
    default:
      cout << "Please only enter a number from the choices" << endl;
      break;
    }
  } while (choice != 9);
  cout << "Thanks for playing!" << endl << endl;
  return 0;
}

void menu()
{
  cout << "Menu: " << endl;
  cout << "1. How to play." << endl;
  cout << "2. Computer guesses your code." << endl;
  cout << "3. You guess the computer's code." << endl;
  cout << "9. Quit." << endl;
  cout << "Choose an option: ";
}

void howToPlay()
{
  cout << "Mastermind is a game that invovles making or guessing a secret code consisting" << endl;
  cout << "of a series of 4 \"code pegs\" among the following colors: R, G, B, Y, W, K." << endl
       << endl;
  cout << "The code-maker provides \"key pegs\" to show the accuracy of the code-breaker's guess." << endl
       << endl;
  cout << "The black \"key peg\" means that a guessed \"code peg\" is the same color and in the same position as" << endl;
  cout << "a solution \"code peg\". White \"key pegs\" mean that a guessed \"code peg\" is the same color as" << endl;
  cout << "a solution \"code peg\", but not in the same position." << endl;
}