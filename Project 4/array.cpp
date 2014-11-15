#include <iostream>
#include <string>
#include <cctype>
#include <cassert> //typically includes assert.h

//Coded by Nathan Tung

using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int rotateRight(string a[], int n, int pos);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int split(string a[], int n, string splitter);

void testPart1()
{
	string h[7] = { "peter", "lois", "meg", "chris", "", "stewie", "meg" };
	assert(lookup(h, 7, "stewie") == 5);
	assert(lookup(h, 7, "meg") == 2);
	assert(lookup(h, 2, "meg") == -1);
	assert(positionOfMax(h, 7) == 5);

	string g[4] = { "peter", "lois", "chris", "stewie" };
	assert(differ(h, 4, g, 4) == 2);
	assert(appendToAll(g, 4, " g") == 4 && g[0] == "peter g" && g[3] == "stewie g");
	assert(rotateLeft(g, 4, 1) == 1 && g[1] == "chris g" && g[3] == "lois g");

	string e[4] = { "meg", "chris", "", "stewie" };

	assert(subsequence(h, 7, g, 4) == -1);
	assert(rotateRight(e, 4, 1) == 1 && e[0] == "chris" && e[2] == "");
	
	string f[3] = { "chris", "meg", "brian" };
	assert(lookupAny(h, 7, f, 3) == 2);
	assert(flip(f, 3) == 3 && f[0] == "brian" && f[2] == "chris");
	
	string kin[6] = { "hal", "lois", "francis", "reese", "malcolm", "dewey" };
	assert(split(kin, 6, "jamie")==3);

	assert(split(h, 7, "meg") == 3);
	
	cerr << "All tests in Part 1 succeeded!" << endl;
}

void testPart2()
{
    string folks[6] = { "george", "lucille", "gob", "michael", "lindsay", "buster" };
    int j = appendToAll(folks, 6, "!!!");  // returns 6
	assert(j==6);
	assert(folks[0]=="george!!!");
	assert(folks[1]=="lucille!!!");
	assert(folks[5]=="buster!!!");

	/*
    string folksTwo[6] = { "george", "lucille", "gob", "michael", "lindsay", "buster" };
    int k = positionOfMax(folksTwo, 8); //USER ERROR; cannot be handled
	assert(k==3);
	*/

	string humanz[5] = { "homer", "marge", "bart", "lisa", "maggie" };
    int m = rotateLeft(humanz, 5, 1);
	assert(m==1);
	assert(humanz[0]=="homer");
	assert(humanz[1]=="bart");
	assert(humanz[2]=="lisa");
	assert(humanz[3]=="maggie");
	assert(humanz[4]=="marge");

    string humansTwo[5] = { "homer", "marge", "bart", "lisa", "maggie" };
    int p = rotateRight(humansTwo, 5, 2);
	assert(p==2);
	assert(humansTwo[0]=="bart");
	assert(humansTwo[1]=="homer");
	assert(humansTwo[2]=="marge");
	assert(humansTwo[3]=="lisa");
	assert(humansTwo[4]=="maggie");

    string beings[6] = { "claire", "phil", "", "haley", "alex", "luke" };
    int q = flip(beings, 4);

	assert(q==4);
	assert(beings[0]=="haley");
	assert(beings[1]=="");
	assert(beings[2]=="phil");
	assert(beings[3]=="claire");
	assert(beings[4]=="alex");
	assert(beings[5]=="luke");
	
	string beingsTwo[6] = { "claire", "phil", "", "haley", "alex", "luke" };
    string group[4] = { "claire", "phil", "mitchell", "luke" };
    int r = differ(beingsTwo, 6, group, 4);  //  returns 2
    int s = differ(beingsTwo, 2, group, 1);  //  returns 1
	assert(r==2);
	assert(s==1);
	
    string family[10] = { "elyse", "steven", "alex", "mallory", "jennifer", "andrew" };
    string names1[10] = { "steven", "alex", "mallory" };
    int t = subsequence(family, 6, names1, 3);  // returns 1
    string names2[10] = { "elyse", "mallory" };
    int u = subsequence(family, 5, names2, 2);  // returns -1
	assert(t==1);
	assert(u==-1);

    string familyTwo[10] = { "elyse", "steven", "alex", "mallory", "jennifer", "andrew" };
    string set1[10] = { "skippy", "jennifer", "mallory", "steven" };
    int v = lookupAny(familyTwo, 6, set1, 4);  // returns 1 (a1 has "steven" there)
    string set2[10] = { "skippy", "nick" };
    int w = lookupAny(familyTwo, 6, set2, 2);  // returns -1 (a1 has none)
	assert(v==1);
	assert(w==-1);

    string kinTwo[6] = { "hal", "lois", "francis", "reese", "malcolm", "dewey" };
    int x = split(kinTwo, 6, "jamie");  //  returns 3
    	// kin might now be
    	//      "hal"  "dewey"  "francis"  "reese"  "malcolm"  "lois"
    	// or   "francis"  "hal"  "dewey"  "malcolm"  "lois"  "reese"
    	// or one of several other orderings.
    	// The first 3 elements are < "jamie"; the last 3 aren't.

    string sibs[4] = { "francis", "reese", "malcolm", "dewey" };
    int y = split(sibs, 4, "francis");  //  returns 1
    	// sibs must now be either
    	//      "dewey"  "francis"  "reese"  "malcolm"
    	// or   "dewey"  "francis"  "malcolm"  "reese"
    	// All elements < "francis" (i.e., "dewey") come before all others.
    	// All elements > "francis" (i.e., "malcolm" and "reese") come
    	//      after all others.
	assert(y==1);

	string hello[] = {"hello", "hello", "hello"};
	assert(lookup(hello, 3, "hello")==0); //make sure the index of first target string instance is returned
	assert(lookup(hello, 3, "Hello")==-1);	//make sure lookup returns -1 if element is not found (also, testing string case)
	assert(positionOfMax(hello, 3)==0); //since all strings are identical, this should return the first one

	string coolBeans[] = {"cool", "BEANS"}; //"BEANS" should come before "cool" (is less than) due to character collating sequence
	assert(lookup(coolBeans, 1, "cool")==0); //should return 0 since "cool" is within the first index of the array
	assert(lookup(coolBeans, 1, "BEANS")==-1); //"BEANS" is not within the first index of the array
	assert(positionOfMax(coolBeans, 1)==0); //returns the index of the only string in that array
	assert(positionOfMax(coolBeans, 2)==0); //even in both indexes, "cool" is larger and thus the same index is returned


	cerr << "All tests in Part 2 succeeded!" << endl;		
}

void testAppendToAll()
{
	string blanksAndMore[] = {"", "asdf", "!"};
	string blanksAndMoreAns[] = {"!!!!!", "asdf!!!!!", "!!!!!!"};
	assert(appendToAll(blanksAndMore, 3, "!!!!!")==3);
	for(int k=0; k<3; k++)
	{
		assert(blanksAndMore[k]==blanksAndMoreAns[k]); //Make sure different strings are all concatenated correctly
	}

	string bounds[] = {"a", "b", "c", "d"};
	string boundsAns[] = {"a123z", "b123z", "c", "d"};
	assert(appendToAll(bounds, 2, "123z")==2);
	for(int k=0; k<4; k++)
	{
		assert(bounds[k]==boundsAns[k]); //Make sure only the first n elements are appended with the specified string
	}

	string hiThere[] = {""};
	assert(appendToAll(hiThere, -5, "asdf")==-1); //Illegal array size testing!

	cerr << "All tests for testAppendToAll() succeeded!" << endl;
}

void testLookup()
{
	string greetings[] = {"hi", "hello", "bonjour", "great to see you", "hello"};
	assert(lookup(greetings, 5, "hello there")==-1); //Check to see if -1 is returned for a string not found
	assert(lookup(greetings, 5, "Hello")==-1); //Check to verify that the function is case-sensitive
	assert(lookup(greetings, 5, "hello")==1); //Make sure the first index is given if more than one instance is found
	assert(lookup(greetings, 0, "boo!")==-1); //See that -1 is returned if there is no array (hence no strings)
	assert(lookup(greetings, 2, "bonjour")==-1); //Make sure only the first n elements are checked
	assert(lookup(greetings, -1, "bonjour")==-1); //Illegal array size testing!

	cerr << "All tests for testLookup() succeeded!" << endl;
}

void testPositionOfMax()
{
	string inOrder[] = {"a", "b", "c", "d", "e"};
	assert(positionOfMax(inOrder, 5)==4); //Test to see if the index of "e" is returned, since it's latest in alphabet
	assert(positionOfMax(inOrder, 4)==3); //Test to see if the function is limited to the first n elements

	string backwardsOrder[] = {"e", "d", "c", "b", "a"};;
	assert(positionOfMax(backwardsOrder, 5)==0); //Test to see if the index of "e" is returned, since it's latest in alphabet
	assert(positionOfMax(backwardsOrder, 2)==0); //Test to see if bounds will throw the program off
	
	string randomOrder[] = {"great", "cool", "zillion", "ton", "massive", "zillion", "billion"};
	assert(positionOfMax(randomOrder, 2)==0); //See if only the first n elements are compared
	assert(positionOfMax(randomOrder, 7)==2); //See if only the index of the first max string is returned
	assert(positionOfMax(randomOrder, -2)==-1); //Illegal array size testing!

	cerr << "All tests for testPositionOfMax() succeeded!" << endl;
}

void testRotateLeft()
{
	string alpha[] = {"a", "b", "c", "d", "e", "f"};
	string alphaRotatedLeftOnA[] = {"b", "c", "d", "e", "f", "a"};
	string alphaRotatedLeftOnC[] = {"a", "b", "d", "e", "f", "c"};
	string alphaRotatedLeftOnF[] = {"a", "b", "c", "d", "e", "f"};
	string alphaRotatedLeftOnCFour[] = {"a", "b", "d", "c", "e", "f"};

	assert(rotateLeft(alpha, 6, 0)==0); //test if rotateLeft is performed
	for(int k=0; k<6; k++)
	{
		assert(alpha[k]==alphaRotatedLeftOnA[k]); //See if all elements of the new array are correct as predicted (first string targetted)
	}
	
	string alpha2[] = {"a", "b", "c", "d", "e", "f"};
	assert(rotateLeft(alpha2, 6, 2)==2);
	for(int k=0; k<6; k++)
	{
		assert(alpha2[k]==alphaRotatedLeftOnC[k]); //See if all elements of the new array are correct as predicted (string "c" targetted)
	}

	string alpha3[] = {"a", "b", "c", "d", "e", "f"};
	assert(rotateLeft(alpha3, 6, 5)==5);
	for(int k=0; k<6; k++)
	{
		assert(alpha3[k]==alphaRotatedLeftOnF[k]); //See if all elements of the new array are correct as predicted (last string targetted)
	}

	string alpha4[] = {"a", "b", "c", "d", "e", "f"};
	assert(rotateLeft(alpha4, 4, 2)==2); //test if rotateLeft is performed
	for(int k=0; k<6; k++)
	{
		assert(alpha4[k]==alphaRotatedLeftOnCFour[k]); //See if only the first n elements (4 in this case) are considered by the function
	}


	string blank[] = {""};
	assert(rotateLeft(blank, 0, 0)==-1); //test if a -1 is given when n is an array of zero size
	assert(rotateLeft(blank, -5, 0)==-1); //test if a -1 is given when n is an array of negative size
	assert(rotateLeft(blank, 1, 0)==0); //test if rotateLeft is performed
	assert(blank[0]=="");

	string blankAlpha[] = {"a", "", "b", "c"};
	string blankAlphaAnswer[] = {"a", "b", "c", ""};
	assert(rotateLeft(blankAlpha, 0, 0)==-1); //test if a -1 is given when n is an array of zero size
	assert(rotateLeft(blankAlpha, -5, 0)==-1); //test if a -1 is given when n is an array of negative size
	assert(rotateLeft(blankAlpha, 4, 1)==1); //test if rotateLeft is performed

	for (int k=0; k<4; k++)
	{
		assert(blankAlpha[k]==blankAlphaAnswer[k]); //check if all the elements are rotated as expected
	}

	cerr << "All tests for rotateLeft() succeeded!" << endl;
}

void testRotateRight()
{
	string alpha[] = {"a", "b", "c", "d", "e", "f"};
	string alphaRotatedRightOnA[] = {"a", "b", "c", "d", "e", "f"};
	string alphaRotatedRightOnC[] = {"c", "a", "b", "d", "e", "f"};
	string alphaRotatedRightOnF[] = {"f", "a", "b", "c", "d", "e"};

	assert(rotateRight(alpha, 6, 0)==0); //test if rotateRight is performed
	for(int k=0; k<6; k++)
	{
		assert(alpha[k]==alphaRotatedRightOnA[k]); //See if all elements of the new array are correct as predicted (first string targetted)
	}
	
	string alpha2[] = {"a", "b", "c", "d", "e", "f"};
	assert(rotateRight(alpha2, 6, 2)==2);
	for(int k=0; k<6; k++)
	{
		assert(alpha2[k]==alphaRotatedRightOnC[k]); //See if all elements of the new array are correct as predicted (string "c" targetted)
	}

	string alpha3[] = {"a", "b", "c", "d", "e", "f"};
	assert(rotateRight(alpha3, 6, 5)==5);
	for(int k=0; k<6; k++)
	{
		assert(alpha3[k]==alphaRotatedRightOnF[k]); //See if all elements of the new array are correct as predicted (last string targetted)
	}

	string alpha4[] = {"a", "b", "c", "d", "e", "f"};
	assert(rotateRight(alpha4, 4, 2)==2); //test if rotateRight is performed
	for(int k=0; k<6; k++)
	{
		assert(alpha4[k]==alphaRotatedRightOnC[k]); //See if only the first n elements (4 in this case) are considered by the function
	}

	string blank[] = {""};
	assert(rotateRight(blank, 0, 0)==-1); //test if a -1 is given when n is an array of zero size
	assert(rotateRight(blank, -5, 0)==-1); //test if a -1 is given when n is an array of negative size
	assert(rotateRight(blank, 1, 0)==0); //test if rotateRight is performed
	assert(blank[0]==""); //make sure array did not change

	string blankAlpha[] = {"a", "", "b", "c"};
	string blankAlphaAnswer[] = {"", "a", "b", "c"};
	assert(rotateRight(blankAlpha, 0, 0)==-1); //test if a -1 is given when n is an array of zero size
	assert(rotateRight(blankAlpha, -5, 0)==-1); //test if a -1 is given when n is an array of negative size
	assert(rotateRight(blankAlpha, 4, 1)==1);  //test if rotateRight is performed

	for (int k=0; k<4; k++)
	{
		assert(blankAlpha[k]==blankAlphaAnswer[k]); //check if all the elements are rotated as expected
	}

	cerr << "All tests for rotateRight() succeeded!" << endl;
}

void testFlip()
{
	string blank[] = {""};
	assert(flip(blank, 0)==0); //nothing should happen to a blank array
	assert(flip(blank, -5)==-1); //check to make sure it catches impossible array sizes
	assert(flip(blank, 1)==1); //check to see if flip was performed
	assert(blank[0]==""); //check to make sure the only string element is "flipped" with itself

	string alpha[] = {"a", "b", "c", "d"};
	string alphaFlipped[] = {"d", "c", "b", "a"};
	assert(flip(alpha, 4)==4); //check to see if flip was performed

	for(int k=0; k<4; k++)
	{
		assert(alpha[k]==alphaFlipped[k]); //verify that all elements have been flipped
	}

	string random[] = {"apple", "cookie", "", "pie", "123"};
	string randomFlipped[] = {"123", "pie", "", "cookie", "apple"};
	assert(flip(random, 5)==5); //check to see if flip was performed

	for(int k=0; k<5; k++)
	{
		assert(random[k]==randomFlipped[k]); //verify that more-complicated elements can be flipped
	}

	string subRandom[] = {"apple", "cookie", "", "pie", "123"};
	string subRandomFlipped[] = {"", "cookie", "apple", "pie", "123"};
	assert(flip(subRandom, 3)==3); //check to see if flip was performed

	for(int k=0; k<5; k++)
	{
		assert(subRandom[k]==subRandomFlipped[k]); //verify that flip works even when not all element indexes are targetted
	}

	cerr << "All tests for flip() succeeded!" << endl;
}

void testDiffer()
{
	string blank[] = {""};
	assert(differ(blank, 1, blank, 1)==1); //since array is entirely equal to itself, the "smaller" n should be given: 1

	string test1[] = {"hello", "no thanks", "nice to meet you"};
	string test2[] = {"hello", "NO THANKS", "nice to meet you"};

	assert(differ(test1, 1, test2, 1)==1); //since the sub-array is entirely equal, the "smaller" n should be given: 1
	assert(differ(test1, 2, test2, 1)==1); //verifies that an irrelevant change in size makes no difference
	assert(differ(test1, 1, test2, 2)==1); //verifies that an irrelevant change in size makes no difference
	assert(differ(test1, 2, test2, 2)==1); //since a difference has been found, the index 1 where it occurs is returned
	assert(differ(test1, 3, test2, 3)==1); //since a difference is found halfway in, the rest of the array doesn't matter

	string stuff1[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff2[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff3[] = {"animals", "bagels", "camels", "dolphins", "nothing"};
	assert(differ(stuff1, 1, stuff2, 1)==1); //since array is entirely equal to itself, the "smaller" n should be given
	assert(differ(stuff1, 3, stuff2, 1)==1); //verifies that an irrelevant change in size makes no difference
	assert(differ(stuff1, 5, stuff2, 5)==5); //since the sub-array is entirely equal, the "smaller" n should be given: 5
	assert(differ(stuff1, 3, stuff2, 5)==3); //since the sub-array is entirely equal, the "smaller" n should be given: 3

	assert(differ(stuff1, 3, stuff3, 5)==3); //one array has run out, so its n is returned
	assert(differ(stuff1, 5, stuff3, 5)==4); //the fifth element is different
	assert(differ(stuff1, 3, stuff3, 3)==3); //since we do not reach the last element, we are told the arrays match

	cerr << "All tests for differ() succeeded!" << endl;
}

void testSubsequence()
{
	string blank[] = {""};
	assert(subsequence(blank, 1, blank, 1)==0); //since array is entirely equal to itself, it starts matching at index 0

	string test1[] = {"hello", "no thanks", "nice to meet you"};
	string test2[] = {"hello", "NO THANKS", "nice to meet you"};
	assert(subsequence(test1, 3, test2, 3)==-1); //although the fronts match up, test2 is not entirely a subsequence of test1
	assert(subsequence(test1, 1, test2, 1)==0); //since "hello" is at the first index of the first array, it's always 0
	assert(subsequence(test1, 2, test2, 1)==0); //since "hello" is at the first index of the first array, it's always 0
	assert(subsequence(test1, 1, test2, 2)==-1); //test2 is larger than test1, so subsequence is not possible
	
	string stuff1[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff2[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff3[] = {"animals", "bagels", "camels", "dolphins", "nothing"};
	assert(subsequence(stuff1, 1, stuff2, 1)==0); //since "animals" is at the first index of the first array, it's always 0
	assert(subsequence(stuff1, 5, stuff2, 2)==0); //since "animals" and "bagels" is at the first index of the first array, it's always 0
	assert(subsequence(stuff1, 5, stuff3, 5)==-1); //the element "nothing" isn't in stuff1
	assert(subsequence(stuff1, 3, stuff3, 3)==0); //since we don't see the part where the array differs, 0 is returned

	string sub1[] = {"here", "there", "nowhere", "somewhere", "anywhere"};
	string sub2[] = {"there", "nowhere", "somewhere", "anywhere"};
	assert(subsequence(sub1, 5, sub2, 4)==1); //sub2 is found in sub1 starting at index 1
	assert(subsequence(sub1, 4, sub2, 4)==-1); //sub2 is found in sub1, but we can't see the part where they completely coincide
	assert(subsequence(sub1, 4, sub2, 3)==1); //see if sub2 can shift correctly and be found in sub1 at index 1
	assert(subsequence(sub1, 2, sub2, 1)==1); //see if sub2 can shift correctly and be found in sub1 at index 1

	cerr << "All tests for subsequence() succeeded!" << endl;
}

void testLookupAny()
{
	string blank[] = {""};
	assert(lookupAny(blank, 1, blank, 1)==0); //since array is entirely equal to itself, the first match is at 0

	string test1[] = {"hello", "no thanks", "nice to meet you"};
	string test2[] = {"hello", "NO THANKS", "nice to meet you"};
	assert(lookupAny(test1, 1, test2, 1)==0); //since the first one matches, index 0 is always returned
	assert(lookupAny(test1, 3, test2, 3)==0); //since the first one matches, index 0 is always returned
	
	string test3[] = {"NO THANKS", "nice to meet you"};
	string test4[] = {"hello", "nice to meet you"};
	assert(lookupAny(test3, 2, test4, 2)==1); //see if two matching strings at the end can be found
	assert(lookupAny(test3, 1, test4, 1)==-1); //see if different single-string arrays return -1
	assert(lookupAny(test3, 2, test4, 1)==-1); //see if two arrays without any common strings return -1
	assert(lookupAny(test3, 1, test4, 2)==-1); //see if two arrays without any common strings return -1
	assert(lookupAny(test1, 3, test3, 1)==-1); //verify that only first n elements are checked, and function is case sensitive
	assert(lookupAny(test1, 3, test3, 2)==2); //test that matches can be found without going out-of-bounds

	string stuff1[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff2[] = {"earwax", "camels"};
	string stuff3[] = {"animals", "bagels", "camels", "dolphins", "nothing"};
	assert(lookupAny(stuff1, 4, stuff2, 1)==-1); //see if only the first n elements are considered
	assert(lookupAny(stuff1, 5, stuff2, 1)==4); //see if matches can be found at the end without bounds problems
	assert(lookupAny(stuff1, 5, stuff2, 2)==2); //see if matches with lower indexes are returned first
	assert(lookupAny(stuff1, 3, stuff2, 2)==2); //see if only the first n are considered

	string sub1[] = {"here", "there", "nowhere", "somewhere", "anywhere"};
	string sub2[] = {"there", "nowhere", "somewhere", "anywhere"};
	assert(lookupAny(sub1, 5, sub2, 4)==1); //check that only the first match at earliest index is returned
	assert(lookupAny(sub1, 5, sub2, 1)==1); //check that only the first match at earliest index is returned

	cerr << "All tests for lookupAny() succeeded!" << endl;
}

void testSplit()
{
	string stuffAns[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff1[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff2[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff3[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff4[] = {"animals", "bagels", "camels", "dolphins", "earwax"};
	string stuff5[] = {"animals", "bagels", "camels", "dolphins", "earwax"};

	assert(split(stuff1, 5, "camels")==2); //test if a sorted array (target in the middle) returns the right index
	assert(split(stuff2, 5, "animals")==0); //test if a sorted array (target in the front) returns the right index
	assert(split(stuff3, 5, "az")==1); //test if a sorted array (target nonexistent but at index 1) returns the right index
	assert(split(stuff4, 5, "ear")==4); //test if a sorted array (target one before the end) returns the right index
	assert(split(stuff5, 5, "ez")==5); //test if n is returned if all strings are less than "ez"

	for(int k=0; k<5; k++) //check that no arrays are changed, since they were already sorted
	{
		assert(stuff1[k]==stuffAns[k]);
		assert(stuff2[k]==stuffAns[k]);
		assert(stuff3[k]==stuffAns[k]);
		assert(stuff4[k]==stuffAns[k]);
		assert(stuff5[k]==stuffAns[k]);
	}
	

	string stuffAns6[] = {"c", "b", "a", "q", "d", "z"};
	string stuffAns7[] = {"c", "d", "q", "b", "a", "z"};
	string stuff6[] = {"c", "q", "d", "b", "a", "z"};
	string stuff7[] = {"c", "q", "d", "b", "a", "z"};

	assert(split(stuff6, 6, "ce")==3); //see if correct position is returned in an unsorted array

	for(int k=0; k<5; k++)
	{
		assert(stuff6[k]==stuffAns6[k]); //see if the array is sorted as expected
	}

	assert(split(stuff7, 3, "darnit")==2); //see if correct position is returned in an unsorted array

	for(int k=0; k<5; k++)
	{
		assert(stuff7[k]==stuffAns7[k]); //see if the array is sorted as expected
	}

	cerr << "All tests for split() succeeded!" << endl;
}

int main()
{
	testPart1();
	testPart2();

	testAppendToAll();
	testLookup();
	testPositionOfMax();
	testRotateLeft();
	testRotateRight();
	testFlip();
	testDiffer();
	testSubsequence();
	testLookupAny();
	testSplit();
}


int appendToAll(string a[], int n, string value)
{
	if(n<0)
		return -1; //If the array size is impossible, return -1
	if(n==0)
		return 0; //If the array has no elements, there is no need to append

	for(int k=0; k<n; k++)
	{
		a[k] += value; //For each element in the array, concatenate the value onto the end of the string
	}
	return n; //Return the number of elements assumed to be in the array
}

int lookup(const string a[], int n, string target)
{
	if(n<=0)
		return -1; //No element is found if the array holds no strings
	for(int k=0; k<n; k++)
	{
		if(a[k]==target) //If any element starting from the front matches, return the index immediately
			return k;
	}
	return -1; //If no element matches, return -1
}

int positionOfMax(const string a[], int n)
{
	if(n<=0)
		return -1; //If there are no elements are an impossible number of elements, return -1
	for(int k=0; k<n; k++)
	{
		int j;
		for(j=0; j<n; j++)
		{
			if(a[k]<a[j]) //Compare that element to the rest; if it's smaller than any other element, break
				break;
		}

		if(j==n) //It means a[k]>=a[j] for all j's
		{
			return k; //Return the index of the first element that is greater than or equal to the rest
		}
	}
	return n-1; //This should never be reached, since at least one element will be greater than or equal to the rest
}

int rotateLeft(string a[], int n, int pos)
{
	if(n<=0) //If n==0, there is no array and thus position does not exist
		return -1;
	if(pos<0)
		return -1;
	if(pos==n-1) //If position is the last one, there is no need to rotate
		return pos;
	string temp = a[pos]; //Store string at position teporarily

	for(int k=pos; k<n-1; k++) //Move everything on the right of position to the left
	{
		a[k]=a[k+1];
	}
	a[n-1]=temp; //Set the last index to the temporary string
	return pos;
}

int rotateRight(string a[], int n, int pos)
{
	if(n<=0) //If n==0, there is no array and thus position does not exist
		return -1;
	if(pos<0)
		return -1;
	if(pos==0) //If position is the first one, there is no need to rotate
		return pos;
	string temp = a[pos]; //Store string at position temporarily

	for(int k=pos; k>0; k--) //Move everything on the left of position to the right
	{
		a[k]=a[k-1];
	}
	a[0]=temp; //Set the first index to the temporary string
	return pos;
}

int flip(string a[], int n)
{
	if(n<0)
		return -1;
	if(n==0)
		return 0;
	for(int k=0; k<n/2; k++) //switch elements up to the middle most element (which, in a case of an odd number of elements, would remain the same)
	{
		string temp=a[k]; //Make a copy of the first string
		a[k]=a[n-1-k]; //Set the array of the first string to the second string
		a[n-1-k]=temp; //Set the array fo the second string to the copy of the first
	}
	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
	if(n1<0 || n2<0)
		return -1;
	
	int n;
	if(n1<n2) //Find the smallest n value
		n=n1;
	else
		n=n2;

	for(int k=0; k<n; k++)
	{
		if(a1[k]!=a2[k]) //Find the first index where strings in respective arrays do not match
			return k;
	}

	return n; //If they are exactly the same, return n

}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if(n2>n1) //A longer array cannot be a subsequence of a shorter one
		return -1;
	if(n2==n1)
	{
		if(n1<=0) //if both arrays are negative, no subsequence is possible
			return -1;

		for(int k=0; k<n1; k++)
		{
			if(a1[k]!=a2[k]) //If both arrays are the same length, every element must match
				return -1;
		}
		return 0;
	}

	for(int k=0; k<=n1-n2; k++) //Loop enough times so that the smaller array always remains within length
	{
		for(int j=0; j<n2; j++)
		{
			if(a2[j]!=a1[k+j])
				break;
			else if(j==n2-1) //If there are no differences starting at position k of the first array, return it
				return k;
		}
	}
	return -1; //If no match is found, return -1

}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	if(n1<=0 || n2<=0)
		return -1;

	for(int k=0; k<n1; k++)
	{
		for(int j=0; j<n2; j++)
		{
			if(a1[k]==a2[j]) //For each element in the first array, check if there is a match in the second array
				return k; //If so, return that index in the first array
		}
	}
	return -1; //Otherwise, return -1 if no match is found at all
}

int split(string a[], int n, string splitter)
{	
	if(n<=0)
		return -1;

	string temp[9999]; //Make an array of an arbitrarily-large size
	int count=0; //This is the index used in array temp
	int ans=0;

	for(int k=0; k<n; k++)
	{
		if(a[k]<splitter)
		{
			temp[count]=a[k]; //All elements in the array less than splitter are thrown into the temp array
			count++;
		}
	}

	if(count>=n)
		return n; //If all strings are less than splitter, return n
	
	ans=count; //Otherwise, store the next index where the string is not less than splitter

	for(int k=0; k<n; k++)
	{
		if(a[k]==splitter)
		{
			temp[count]=a[k]; //All elements in the array equal to splitter are thrown into the temp array
			count++;
		}
	}

	for(int k=0; k<n; k++)
	{
		if(a[k]>splitter)
		{
			temp[count]=a[k]; //All elements in the array greater than splitter are thrown into the temp array
			count++;
		}
	}
	
	for(int k=0; k<n; k++)
	{
		a[k]=temp[k]; //For the first n elements, copy those from temp into the array
	}

	return ans;
}