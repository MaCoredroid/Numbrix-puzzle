// getyournumber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include<bitset>//The object is initialized with zeros.
//------------------------------------------------------------------------------
using namespace std;

//------------------------------------------------------------------------------
struct node
{
	int val;
	bitset<8> neighbors;
};
//------------------------------------------------------------------------------
class Solver
{
private:
	//member variables
	vector<node> arr;//store every value, if it is'*',leave it as -1
	vector<bool> weHave;//if we have n,then weHave[n]=true;
	int wid=0, hei=0, max=0;//width,height, the max of all the number
	//member functions
	void findStart(int& x, int& y, int& z);//find the minium number
	bool solveIt();//change the arr
	bool search(int x, int y, int w, int dr);//origin point :(x,y),search for value:w; dr:+1or -1
	bitset<8> getNeighbors(int x, int y);//if there is a number,set it to true
	vector<int> dx = vector<int>({ -1, 1, 0, 0,1,1,-1 ,-1});//eight directions
	vector<int> dy = vector<int>({ 0, 0, -1, 1,1,-1,-1,1 });
	
public:
	//constructors & deconstructors
	Solver()
	{
	}

	//member functions
	bool solve(vector<string>& puzz, int max_wid)
	{
		int c = 0;
		wid = max_wid;
		hei = int(puzz.size()) / wid;
		max = wid * hei;
		arr = vector<node>(max, node({ 0, 0 }));
		weHave = vector<bool>(max + 1, false);
		for (const auto& s : puzz)
		{
			if (s == "*") 
			{
				max--;
				arr[c++].val = -1; //there is no number here
				continue; 
			}
			arr[c].val = atoi(s.c_str());//record value
			if (arr[c].val > 0)
			{
				weHave[arr[c].val] = true;
			}
			c++;
		}
		if(solveIt());
		{
			c = 0;
			for (auto& s : puzz)
			{
				if (s == ".")
				{
					s = std::to_string(arr[c].val);//if we write a value,write it to the puzzle
				}
				c++;
			}
			return true;
		}
		return false;
	}
};
bitset<8> Solver::getNeighbors(int x, int y)
{
	bitset<8> retval;//initate it with 0
	for (int xx = 0; xx < 8; xx++)
	{
		int a = x + dx[xx], b = y + dy[xx];
		if (a < 0 || b < 0 || a >= wid || b >= hei)
		{
			continue;
		}
		if (arr[a + b * wid].val > -1)
		{
			retval.set(xx);//set it to 1
		}
	}
	return retval;
}
bool Solver::search(int x, int y, int w, int dr)
{
	if ((w > max && dr > 0) || (w < 1 && dr < 0) || (w == max && weHave[w]))
	{
		return true;//search reaches end
	}

	node& n = arr[x + y * wid];
	n.neighbors = getNeighbors(x, y);
	if (weHave[w])
	{
		for (int d = 0; d < 8; d++)
		{
			if (n.neighbors[d])
			{
				int a = x + dx[d], b = y + dy[d];
				if (arr[a + b * wid].val == w)
					if (search(a, b, w + dr, dr))
						return true;
			}
		}
		return false;
	}

	for (int d = 0; d < 8; d++)
	{
		if (n.neighbors[d])
		{
			int a = x + dx[d], b = y + dy[d];
			if (arr[a + b * wid].val == 0)
			{
				arr[a + b * wid].val = w;
				if (search(a, b, w + dr, dr))
					return true;
				arr[a + b * wid].val = 0;
			}
		}
	}
	return false;
}
bool Solver::solveIt()
{
	int x=0, y=0, z=0;
	findStart(x, y, z);
	if (z == 99)
	{ 
		
		return false; 
	}
	if (search(x, y, z + 1, 1))
	{
		if (z > 1)
		{
			if (search(x, y, z - 1, -1))
			{
				return true;
			}
		}
	}
	
		return false;
	
	
}
void Solver::findStart(int& x, int& y, int& z)//(x,y) , value
{
	z = 99;//the biggest number below 100
	for (int b = 0; b < hei; b++)
	{
		for (int a = 0; a < wid; a++)
		{
			if (arr[a + wid * b].val > 0 && arr[a + wid * b].val < z)
			{
				x = a; 
				y = b;
				z = arr[a + wid * b].val;
			}
		}
	}
}

struct puzzle
{
	int width;
	vector<string> data;
};

void printPuzzle(puzzle p)
{
	int c = 0;
	for (vector<string>::iterator i = p.data.begin(); i != p.data.end(); i++)
	{
		if ((*i) != "*" && (*i) != ".")
		{
			if (atoi((*i).c_str()) < 10) cout << "0";
			cout << (*i) << " ";
		}
		else
			cout << "   ";
		if (++c >= p.width)
		{
			cout << endl; c = 0;
		}
	}
	cout << endl << endl;

}
//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	struct puzzle puzzles[3];

	istringstream iss1("32 . . . 37 * * * . 34 . . 38 * * * . 25 . . 12 . * * 29 . 20 . 40 . * * . 28 . 19 . 10 1 * * * 15 . . . . * * * * * . . . 3 * * * * * * . 4");
	copy(istream_iterator<string>(iss1), istream_iterator<string>(), back_inserter<vector<string> >(puzzles[0].data));
	puzzles[0].width = 8;

	istringstream iss2(". 53 . 59 . 67 66 . . 52 . . 57 . . 68 72 . . . 56 . 64 * * * * 49 . . . * * 17 . * . 10 . * . 16 . . 22 . 46 . * . 2 . . 24 . 44 . . 4 1 . . 25 . . 7 6 . 34 . . 31 42 40 . . . . . 29 .");
	copy(istream_iterator<string>(iss2), istream_iterator<string>(), back_inserter<vector<string> >(puzzles[1].data));
	puzzles[1].width = 9;

	istringstream iss3("56 . . . . 62 . . 67 . 55 . 53 . . 65 . . 9 . 52 . . 48 . . 69 . 6 . * * * 47 . . 5 . . * * * 72 . 42 . . 13 * * * . . 41 . 3 . 18 . . 38 . . 2 . . 22 23 . . . 32 1 20 . . 25 . 35 . .");
	copy(istream_iterator<string>(iss3), istream_iterator<string>(), back_inserter<vector<string> >(puzzles[2].data));
	puzzles[2].width = 9;
	
		int puzzleIndex = 0;

	if (argc > 1)
	{
		puzzleIndex = atoi(argv[1]);
	}

	cout << "The original puzzle:" << endl;

	printPuzzle(puzzles[puzzleIndex]);

	Solver s;
	bool bFound = s.solve(puzzles[puzzleIndex].data, puzzles[puzzleIndex].width);

	if (bFound)
	{
		cout << "The answer: " << endl;
		printPuzzle(puzzles[puzzleIndex]);
	}
	else
	{
		cout << "No answer." << endl;
	}

	return 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
