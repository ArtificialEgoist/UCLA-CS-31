#include <iostream>
#include <string>
#include <cctype>
#include <assert.h>
using namespace std;

/*

Coded by Nathan Tung.	//Use this code for testing outputs: cerr << "TEST";

Example Input/Output:
Enter the subject line of the email: MAKE MONEY FAST XTFWAQQQ
Enter the body of the email.  Press Enter on an empty line to finish.
THIS IS YOUR CHANCE TO MAKE lots of moolah!
CLICK ON www.lotsofmoolah.com FOR MORE INFORMATION!
	← the user just hit the enter key on this line
This email is classified as spam, because its spam score is 115.
Would you like to classify another email (y or n)? Quit
Please enter y or n.
Would you like to classify another email (y or n)? Y
Please enter y or n.
Would you like to classify another email (y or n)? y
Enter the subject line of the email: hi mom
Enter the body of the email.  Press Enter on an empty line to finish.
Hi mom,
I hope you're doing OK.  My CS class is great.  The instructor is GREAT!
I love you!
	← the user just hit the enter key on this line
This email is classified as legitimate, because its spam score is 0.
Would you like to classify another email (y or n)? n

Number of spam messages: 1
Number of legitimate messages: 1

*/

string getFirstWord(string text);
string getLastWord(string text);
string extractWord(string& text);
bool isUppercase(string text);
string makeUppercase(string text);
bool hasMultipleExclamations(string text);
bool isGibberishWord(string text);
bool isVowel(char c);
bool isSpecialWord(string text);

void testAll();


int main()
{
	//testAll();

	int numSpamMsgs=0; //Total number of spam messages
	int numLegitimateMsgs=0; //Total number of legitimate messages

	string classifyAnother; //Used later to determine whether to continue the do-while loop

	do
	{
		int spamScore=0; //Initiate the spam score to 0 for each upcoming email (resets for each iteration)
		
		string subject;
		cout << "Enter the subject line of the email: "; //Prompt user for subject line
		getline(cin, subject); //Take in subject line
	
		//cerr<<subject<<"\n";
	
		string body;
		cout << "Enter the body of the email.  Press Enter on an empty line to finish." << endl; //Prompt user for email body
		
		string temp;
		do
		{
			temp="";
			getline(cin, temp); //Take in next line of email body
			body += temp + " "; //Append that line to running total of email body
		}
		while(temp!=""); //Continue to take in lines of email body until one line is completely empty
	
		//cerr<<body;

		//Start testing for spam

		string subjectCopy1 = subject; //SPAM TEST 1
		if(getFirstWord(subjectCopy1)!="")
		{
			int numWordsCap=0;
			int numWordsTotal=0;
			string temp = extractWord(subjectCopy1); //As long as there is a word, find that word (and remove it from the rest)

			while(temp!="")
			{
				if(temp==makeUppercase(temp)) //Check if that word is all uppercase
				{
					numWordsCap++;
				}
				numWordsTotal++;

				temp = extractWord(subjectCopy1);
			}

			//cerr << numWordsCap << " / " << numWordsTotal << endl;

			if(static_cast<double>(numWordsCap)/numWordsTotal > 0.9) //Determine the ratio between all uppercase words and total words
				spamScore += 30; //Add to score if the ratio is greater than 0.9
		}

		string subjectCopy2 = subject; //SPAM TEST 2
		if(isGibberishWord(getLastWord(subjectCopy2))) //Check if the last word is gibberish (has more than 3 consecutive consonants)
			spamScore += 40;

		string subjectCopy3 = subject; //SPAM TEST 3
		if(hasMultipleExclamations(subjectCopy3)) //Check if the subject has 3 or more consecutive exclamations
			spamScore += 20;

		string bodyCopy1 = body; //SPAM TEST 4
		if(getFirstWord(bodyCopy1)!="")
		{
			int numWordsCap=0;
			int numWordsTotal=0;
			string temp = extractWord(bodyCopy1); //As long as there is a word, find that word (and remove it from the rest)

			while(temp!="")
			{
				if(temp==makeUppercase(temp)) //Check if that word is all uppercase
				{
					numWordsCap++;
				}
				numWordsTotal++;

				temp = extractWord(bodyCopy1);
			}

			//cerr << numWordsCap << " / " << numWordsTotal << endl;

			if(static_cast<double>(numWordsCap)/numWordsTotal > 0.5) //Determine the ratio between all uppercase words and total words
				spamScore += 40; //Add to score if the ratio is greater than 0.5
		}

		string bodyCopy2 = body; //SPAM TEST 5
		if(getFirstWord(bodyCopy2)!="")
		{
			string temp = extractWord(bodyCopy2); //Find the first word if there is one (and remove it from the rest)
			while(temp!="")
			{
				if(isSpecialWord(temp)) //If that word is a special word (which identifies spam emails), add to spam score
					spamScore += 5;
				temp = extractWord(bodyCopy2); //Continue to find following words (and removing them from the rest)
			}
		}

		//End testing for spam
	
		if(spamScore>100)
		{
			cout << "This email is classified as spam, because its spam score is " << spamScore << "." << endl;
			numSpamMsgs++;
		}
		else
		{
			cout << "This email is classified as legitimate, because its spam score is " << spamScore << "." << endl;
			numLegitimateMsgs++;
		}
	
		cout << "Would you like to classify another email (y or n)? "; 
		getline(cin, classifyAnother);
		while(classifyAnother!="y" && classifyAnother!="n") //User must input "y" or "n" for the program to exit this loop
		{
			cout << "Please enter y or n." << endl; //If user inputs something else, they will continuing to be prompted for either "y" or "n"
			cout << "Would you like to classify another email (y or n)? "; 
			getline(cin, classifyAnother);
		}
	}
	while(classifyAnother=="y"); //The entire spam check process occurs all over again
		
	cout << "\nNumber of spam messages: " << numSpamMsgs << endl; //After user is done with spam checking (by inputting "n"), total counts are displayed
	cout << "Number of legitimate messages: " << numLegitimateMsgs << endl;

}
	
	
string getFirstWord(string text)
{
	string first="";
	int ind=0; //This is going to be index of the first character of a word
	while(ind<text.size() && !isalpha(text[ind])) //As long as the index does not go out of bounds, set ind to the position of the first letter in the string
	{
		ind++;
	}
	if(ind==text.size()) //If no such letters are found, return a blank string
		return first;
	int start=ind;
	int count=0;
	while(ind<text.size() && isalpha(text[ind])) //Otherwise, count consecutive letters within bounds starting from ind (counting forwards)
	{
		count++;
		ind++;
	}
	first=text.substr(start, count); //Take the substring starting from the first letter and going for count characters
	return first;
}

string getLastWord(string text)
{
	string first="";
	int ind=text.size()-1; //This is going to be index of the last character of a word
	while(ind>=0 && !isalpha(text[ind])) //As long as the index does not go out of bounds, set ind to the position of the last letter in the string
	{
		ind--;
	}
	if(ind==text.size()) //If no such letters are found, return a blank string
		return first;
	int end=ind;
	int count=0;
	while(ind>=0 && isalpha(text[ind])) //Otherwise, count consecutive letters within bounds starting from ind (counting backwards)
	{
		count++;
		ind--;
	}
	first=text.substr(end-count+1, count); //Take the substring starting from the first letter and going for count characters
	return first;
}

string extractWord(string& text)
{
	string temp = getFirstWord(text); //Determine the first word
	
	if(temp=="") //If there is no first word, return an empty string
		return temp;

	int ind=text.find(temp); //Find the index where the first word starts
	text=text.substr(ind+temp.size()); //Set text to the portion of the string right AFTER the first word ends
	return temp;
}

bool isUppercase(string text)
{
	for(string::size_type k=0; k<text.size(); k++) //Cycle through all the characters in text
	{
		if(isalpha(text[k]) && islower(text[k])) //If any of the letters are lowercase, return false
			return false;
	}
	return true; //Return true after none of the letters are lowercase
}

string makeUppercase(string text)
{
	string newText="";
	for(string::size_type k=0; k<text.size(); k++) //Cycle through all the characters in text
	{
		if(isalpha(text[k])) //If a character is a letter, append the uppercase version of that character to the new string
			newText += toupper(text[k]);
		else
			newText += text[k]; //If the character is not a letter, simply append that character to the new string
	}
	return newText;
}

bool hasMultipleExclamations(string text)
{
	int exclamationCount=0;
	int currentCount=0;
	for(string::size_type k=0; k<text.size(); k++) //Cycle through all the characters in text
	{
		if(text[k]=='!') //If an exclamation point is found, increase the current count (which represents consecutive exclamations)
			currentCount++;
		else //If not, update exclamation count based on the current count, then reset the current count
		{
			if(currentCount>exclamationCount)
				exclamationCount=currentCount;
			currentCount=0;
		}	
	}
	if(currentCount>exclamationCount) //Do one last comparison to update exclamation count
		exclamationCount=currentCount;
	return (exclamationCount>=3);
}

bool isGibberishWord(string text)
{
	int consonantCount=0;
	int currentCount=0;
	for(string::size_type k=0; k<text.size(); k++) //Cycle through all the characters in text
	{
		if(isalpha(text[k]) && !isVowel(text[k])) //If a consonant is found, increase the current count (which represents consecutive consonants)
			currentCount++;
		else //If not, update consonant count based on the current count, then reset the current count
		{
			if(currentCount>consonantCount)
				consonantCount=currentCount;
			currentCount=0;
		}
	}
	if(currentCount>consonantCount) //Do one last comparison to update consonant count
		consonantCount=currentCount;
	return (consonantCount>3);
}

bool isVowel(char c)
{
	char x = tolower(c); //Make a lowercase copy of the parameter character to make the if-statement easier
	if(isalpha(c) && (x=='a' || x=='e' || x=='i' || x=='o' || x=='u')) //If the character is not a vowel, return true
		return true;
	return false;
}

bool isSpecialWord(string text)
{
	string temp = makeUppercase(text);

	if(temp=="BUY" || temp=="CHEAP" || temp=="CLICK" || temp=="DIPLOMA" || temp=="ENLARGE" || temp=="FREE" || temp=="LIMITED" || temp=="MONEY" || temp=="NOW" || temp=="OFFER" || temp=="ONLY" || temp=="PILLS" || temp=="SEX")
		return true;
	return false;
}

void testAll()
{
	if (getFirstWord("hello there") == "hello")
		cerr << "Passed test 1: getFirstWord(\"hello there\") == \"hello\")" << endl;
	if (!isUppercase("WoW"))
		cerr << "Passed test 2: !isUppercase(\"WoW\")" << endl;
	if (isUppercase("WOW"))
		cerr << "Passed test 3: isUppercase(\"WOW\")" << endl;

	assert(isVowel('a'));
	assert(isVowel('e'));
	assert(isVowel('i'));
	assert(isVowel('o'));
	assert(isVowel('u'));
	assert(isVowel('A'));
	assert(isVowel('E'));
	assert(isVowel('I'));
	assert(isVowel('O'));
	assert(isVowel('U'));
	assert(!isVowel('B'));
	assert(!isVowel('z'));
	assert(!isVowel('!'));
	assert(!isVowel('5'));

	assert(makeUppercase("hello")=="HELLO");
	assert(makeUppercase("HellO")=="HELLO");
	assert(makeUppercase("hElLo")=="HELLO");
	assert(makeUppercase("z814F")=="Z814F");
	assert(makeUppercase("0!!!")=="0!!!");
	assert(makeUppercase("bigger")=="BIGGER");
	assert(makeUppercase("na~h")=="NA~H");
	assert(makeUppercase("")=="");

	assert(isUppercase("WOW!!") );
	assert(isUppercase("NOOOOOOOOOOO!!$%!^@&*#") );
	assert(isUppercase("W0WZERZ~~#084") );
	assert(!isUppercase("NOOOOOOOOOooOOOOo") );
	assert(!isUppercase("B000000`13`!~o") );
	assert(!isUppercase("WoW!!") );
	assert(isUppercase("") );
	assert(isUppercase("99") );
	assert(isUppercase("!!") );

	assert(hasMultipleExclamations("!!!"));
	assert(hasMultipleExclamations("afh!!akjg!!!jgu83tbi!!!!!!8bga"));
	assert(hasMultipleExclamations("something!!hereee!!!!!ee"));
	assert(hasMultipleExclamations("string text!!!!!!"));
	assert(!hasMultipleExclamations("!!"));
	assert(!hasMultipleExclamations("!!-!!-!!b8"));
	assert(!hasMultipleExclamations("!!1!946153!!487!~!!*!!"));
	assert(!hasMultipleExclamations("nothing here"));
	assert(!hasMultipleExclamations(""));

	assert(isGibberishWord("aaaaabnfjanuba"));
	assert(isGibberishWord("ngaonrgoangsfas"));
	assert(isGibberishWord("11876889~ffdf8*78~"));
	assert(isGibberishWord("fghj"));
	assert(!isGibberishWord("jkl"));
	assert(!isGibberishWord("ajkl~"));
	assert(!isGibberishWord("ccac"));
	assert(!isGibberishWord("sadde"));
	assert(!isGibberishWord("`123456789875432@#$%"));
	assert(!isGibberishWord("cool dude"));
	assert(!isGibberishWord("NASA"));
	assert(!isGibberishWord(""));

	assert(getFirstWord("hello there") == "hello");
	assert(getFirstWord("what is there to talk about") == "what");
	assert(getFirstWord("947`7~&*&$ ~(*$~") == "");
	assert(getFirstWord("") == "");
	assert(getFirstWord("              ") == "");
	assert(getFirstWord(" !!! hello !!!") == "hello");
	assert(getFirstWord("`123456789findme lol") == "findme");
	assert(getFirstWord("???pro???blem???") == "pro");
	assert(getFirstWord("`12345689`765432185@#$%^*(9 I wonder if this will work") == "I");
	assert(getFirstWord("1234NoThing is wrong with this!") == "NoThing");
	
	assert(getLastWord("hello there") == "there");
	assert(getLastWord("what is there to talk about") == "about");
	assert(getLastWord("947`7~&*&$ ~(*$~") == "");
	assert(getLastWord("") == "");
	assert(getLastWord("              ") == "");
	assert(getLastWord(" !!! hello !!!") == "hello");
	assert(getLastWord("`123456789findme lol") == "lol");
	assert(getLastWord("???pro???blem???") == "blem");
	assert(getLastWord("`12345689`765432185@#$%^*(9 I wonder if this will work") == "work");
	assert(getLastWord("1234NoThing is wrong with this!") == "this");
	
	string s = "***hello there";
		assert(extractWord(s) == "hello"  &&  s == " there");
		assert(extractWord(s) == "there"  &&  s == "");
		assert(extractWord(s) == ""  &&  s == "");
	string x="`123456789findme lol";
		assert(extractWord(x) == "findme"  &&  x == " lol");
		assert(extractWord(x) == "lol" && x == "");
	string y=" !!! hello !!!";
		assert(extractWord(y) == "hello"  &&  y == " !!!" );
		assert(extractWord(y) == "" && y == " !!!");
	string z="???pro???blem???";
		assert(extractWord(z) == "pro"  &&  z == "???blem???");
	string j="          ";
		assert(extractWord(j) == ""  &&  j == "          " );
	string k="";
		assert(extractWord(k) == ""  &&  k == "" );

	cerr << "All the tests have succeeded!" << endl;
}
