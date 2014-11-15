	// Code for Project 1
	// Determine gas mileage

	#include <iostream>
	using namespace std;       // see pp. 36-37 in Savitch book

	int main()
	{
	    double milesTraveled;
	    double gasConsumed;

	    cout << "How many miles have you driven? ";
	    cin >> milesTraveled;
	    cout << "How many gallons of gas did that take? ";
	    cin >> gasConsumed;

	    double mpg = milesTraveled / gasConsumed;

	    cout.setf(ios::fixed);       // see pp. 31-32 in Savitch book
	    cout.precision(1);

	    cout << "Your vehicle gets " << mpg << " miles per gallon." << endl;

	    if (mpg > 40)
	        cout << "How green you are!" << endl;
	    else if (mpg < 20)
	        cout << "What a gas hog!" << endl;
	}