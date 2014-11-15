#include <iostream>
#include <string>
using namespace std;

/*
	Coded by Nathan Tung

	Example Output:
	Minutes used: 385
	Text messages: 423
	Customer name: Alex Bell
	Month number (1=Jan, 2=Feb, etc.): 11
	---
	The bill for Alex Bell is $48.46
*/

int main()
{
	const double MINUTES_CHARGE = 0.40;
	const double TEXTS_OVER_250_CHARGE = 0.02;
	const double TEXTS_OVER_250_CHARGE_DISCOUNT = 0.01;
	const double TEXTS_OVER_500_CHARGE = 0.11;


	int minutes;
	cout << "Minutes used: ";
	cin >> minutes; //Prompt user for amount of minutes used
	

	int texts;
	cout << "Text messages: ";
	cin >> texts; //Prompt user for amount of texts used
	cin.ignore(10000, '\n'); //Prevent string errors in which getline is prematurely satisfied
	

	string name;
	cout << "Customer name: ";
	getline(cin, name); //Prompt user for his/her name
	

	int month;
	cout << "Month number (1=Jan, 2=Feb, etc.): ";
	cin >> month; //Prompt user for month of bill
	

	cout << "---" << endl; //Dividing line between input and output

	//Start error check; if 1 is returned, program has failed due to wrong user input
	if(minutes<0) //Minutes must be at least 0
	{
		cout << "The number of minutes used must be nonnegative." << endl;
		return 1;
	}
	if(texts<0) //Texts must be at least 0
	{
		cout << "The number of text messages must be nonnegative."<< endl;
		return 1;
	}
	if(name=="") //Name cannot be left blank
	{
		cout << "You must enter a customer name." << endl;
		return 1;
	}
	if(month<1 || month>12) //Months must be between 1 and 12 (inclusive)
	{
		cout << "The month number must be in the range 1 through 12." << endl;
		return 1;
	}
	//End of specific error checking


	double calculatedCost=45.0; //Base bill amount
	double extraMinutesCost=0.0;
	double extraTextsCost=0.0;


	if(minutes>400) //Calculate cost of any extra minutes over the given 400
	{
		extraMinutesCost = (minutes-400)*MINUTES_CHARGE;
	}

	if(texts>500) //Calculate cost of any extra texts over the given base 250 (over 500 means a higher charge/rate)
	{
		extraTextsCost = (texts-500)*TEXTS_OVER_500_CHARGE;
		texts=500; //If texts was originally greater than 500, then there are still 500 texts that must be accounted for

	}
	if (texts>250) //Calculate cost of extra texts between 250 and up to 500
	{
		if(6 <= month && month <= 9)
			extraTextsCost += (texts-250)*TEXTS_OVER_250_CHARGE_DISCOUNT; //With summer discount
		else
			extraTextsCost +=(texts-250)*TEXTS_OVER_250_CHARGE; //Without summer discount
	}


	calculatedCost += extraMinutesCost + extraTextsCost; //Add together the total cost

	cout.setf(ios::fixed);
	cout.precision(2); //Fix the output charge amount to two decimals

	cout << "The bill for " << name << " is $" << calculatedCost << endl;

	return 0; //The program has completed successfully

}