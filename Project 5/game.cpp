#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <cstdlib> // for std::srand and std::rand
#include <ctime>   // for std::time

using namespace std;

//Written by Nathan Tung

const int MIN_WORD_LENGTH=4; //Minimum length for mystery word
const int MAX_WORD_LENGTH=6; //Maximum length for mystery word
const int MAX_WORDS=10000; //Maximum amount of words to be chosen from

int manageOneRound(char words[][MAX_WORD_LENGTH+1], int num, int wordnum); //Responsible for the output of one round
int charsInCommon(char guessWord[], char mysteryWord[]); //Determines number of characters in common between two C strings
bool invalidWord(char word[]); //Determines if the word is invalid (i.e., not all lower case alpha between 4-6 chars long)
int loadWords(char words[][MAX_WORD_LENGTH+1], int maxWords); //Responsible for loading words from text file to array

int main()
{
	char wordList[MAX_WORDS][MAX_WORD_LENGTH+1]; //Initializes array from which mystery word is chosen
	//int maxWords=10000; //Set number of total words to allow

	int numWordsLoaded = loadWords(wordList, MAX_WORDS); //Load words and find number of words loaded

	if(numWordsLoaded<1)
	{
		cout << "No words were loaded, so I can\'t play the game!" << endl; //Check to make sure at least one word is loaded
		return -1;
	}

	int numRounds; 
	cout << "How many rounds do you want to play? ";
	cin >> numRounds; //Prompt user for number of rounds to play
	
	cin.ignore(1, '\n'); //Ignore the line entered after numRounds to avoid pre-satisfying the upcoming trial word

	if(numRounds<=0)
	{
		cout << "The number of rounds must be positive" << endl; //Check to make sure at least one round is to be played
		return -1;
	}

	int minTries=0, maxTries=0; //Initialize to an impossible value; thus we can determine whether to update in a round
	double avgTries=0.00; //Initialize average number of tries to 0.00

	std::srand(std::time(0)); //Seeds std::rand with the current time to assert "randomness"

	int numTries=0; //Initialize number of tries per round; reset per round
	int collectiveNumTries=0; //Initialize total number of tries; accumulates per round to aid in calculating average

	for(int k=1; k<=numRounds; k++) //Play user-specified number of rounds by looping manageOneRound
	{
		int mysteryIndex=rand()%numWordsLoaded; //Choose a random integer, using the modulus operator to guarantee size boundaries in index
		
		//cerr << "//The mystery word is " << wordList[mysteryIndex] << endl; //CERR: Tell programmer what the mystery word is

		cout << endl << "Round " << k << endl;
		cout << "The mystery word is " << strlen(wordList[mysteryIndex]) << " letters long" << endl;

		numTries=manageOneRound(wordList, numWordsLoaded, mysteryIndex); //Reset numTries to number of tries used in last round only

		cout << "You got it in " << numTries << " tries" << endl;

		if(minTries==0 && maxTries==0) //If this is the first round played, set both minTries and maxTries to the number of tries in first round
		{
			minTries=numTries;
			maxTries=numTries;
		}

		if(numTries>maxTries)
			maxTries=numTries; //Update maxTries if the latest numTries is greater

		if(numTries<minTries)
			minTries=numTries; //Update minTries if the latest numTries is lesser

		collectiveNumTries+=numTries; //Add numTries of every round to collectiveNumTries

		avgTries=static_cast<double>(collectiveNumTries)/k; //Find average, dividing collectiveNumTries by number of rounds played thus far

		cout.setf(ios::fixed);
	    cout.precision(2);

		cout << "Average: " << avgTries << ", minimum: " << minTries << ", maximum: " << maxTries << endl;
	}
}

int manageOneRound(char words[][MAX_WORD_LENGTH+1], int num, int wordnum)
{
	if(num<1 || wordnum<0 || wordnum>=num) //Check to make sure index of mystery word is within bound of word list
		return -1; //Error checking! Impossible values will terminate program

	int numTries=0; //Initialize number of tries for this round to zero
	char mysteryWord[MAX_WORD_LENGTH+1]; //Initialize mysteryWord C string
	strcpy(mysteryWord, words[wordnum]); //Make a copy of the mystery word, storing it in the mysteryWord C string

	char guessWord[100]; //Initialize C string to hold the trial words (up to 100 chars)
	
	do
	{
		cout << "Trial word: ";
		cin.getline(guessWord, 100); //Allow user to input a trial word up to 100 chars

		numTries++; //Increment number of tries, regardless of whether the word is valid or a right guess

		if(invalidWord(guessWord)) //Skip to next trial word if it is invalid
		{
			cout << "Your trial word must be a word of 4 to 6 lower case letters" << endl;
			continue;
		}
		
		bool found=false;
		for(int k=0; k<num; k++)
			if(strcmp(words[k], guessWord)==0) //See if any word in the array matches trial word
				found=true;
		if(!found) //Skip to next trial word if it is not "known" (i.e., not found in the array)
		{
			cout << "I don't know that word" << endl;
			continue;
		}

		if(strcmp(guessWord, mysteryWord)!=0) //If wrong guess, return number of unique chars matching between the trial and mystery words
			cout << charsInCommon(guessWord, mysteryWord) << endl; 

	}
	while(strcmp(guessWord, mysteryWord)!=0); //Keep repeating until the trial word matches the mystery word

	return numTries; //Return number of tries needed to guess the mystery word
}

int charsInCommon(char guessWord[], char mysteryWord[])
{
	char guessTemp[100];
	strcpy(guessTemp, guessWord); //Make a copy of the C string guessWord in guessTemp

	int count=0; //Initialize counter for the same of characters in common between trial word and mystery word

	for(int k=0; mysteryWord[k]!='\0'; k++) //Cycle through every combination of chars between mysteryWord and GuessTemp until one ends
	{
		for(int j=0; guessTemp[j]!='\0'; j++)
		{
			//cerr << "Testing: " << mysteryWord[k] << " - " << guessWord[j] << endl;
			
			if(mysteryWord[k]==guessTemp[j])
			{
				count++; //Increment count for one common character if they match
				guessTemp[j]='Z'; //Set that matching element of guessWord to 'Z' to indicate that it is used; prevents repeats by capitalization
				break; //Check the next character in mysteryWord since a match has already been found
			}
		}
	}

	return count; //Return number of common characters between the words
}

bool invalidWord(char word[])
{
	for(int k=0; word[k]!='\0'; k++) //Invalid if any characters are non-alpha or uppercase
		if(!isalpha(word[k]) || isupper(word[k]))
			return true;
	
	if(strlen(word)>6 || strlen(word)<4) //Invalid if the C string is not between 4 to 6 characters
		return true;

	/*
	for(int k=0; k<4; k++) //Invalid if zero-byte is found within the first four spots; it's less than 4 characters long!
		if(word[k]=='\0')
			return true;
	int index=0;
	while(word[index]!='\0' & index<7)
		index++;
	if(index==7) //Invalid if no zero-byte is found; it's more than 6 characters long!
		return true;
	*/

	return false; //Otherwise, it's valid (return false)
}

int loadWords(char words[][MAX_WORD_LENGTH+1], int maxWords) //Using the implementation given in Project 5 Specs
{
    //*********************
	//*** On a Windows system, if you replace the filename with the name
    //*** of some other file, use / in the string instead of \ (e.g.,
    //*** "C:/CS31/Project5/mywordfile.txt").
    //const char filename[] = "Z:/words.txt";
	const char filename[] = "C:/NathanCT/Homework/Freshman Year Q1/Computer Science 31/Project 5/words.txt";

    ifstream wordfile(filename);
    if ( ! wordfile)
    {
        cout << "Cannot open " << filename << endl;
        return -1;
    }
    const int LINELIMIT = 10000;
    char line[LINELIMIT];
    int numWords = 0;
    while (wordfile.getline(line, LINELIMIT))
    {
        if (numWords == maxWords)
        {
            cout << "Using only the first " << numWords
                 << " words from " << filename << endl;
            break;
        }
        int k;
        for (k = 0; islower(line[k]); k++)
            ;

          // The following two lines fix things for non-Windows users
        if (line[k] == '\r')
                line[k] = '\0';

        if (line[k] == '\0'  &&  k >= MIN_WORD_LENGTH  &&  k <= MAX_WORD_LENGTH)
        {
            strcpy(words[numWords], line);
            numWords++;
        }
    }
    return numWords;
}