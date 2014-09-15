/*****
   Sudoku Program for 9 by 9 grid
   assuming puzzle length is 81
   Author by Vinod
   Email: vinod.designer1@gmail.com
*****/

#include <iostream>
#include <cstdio>
#include <vector>
#include <limits>

using namespace std;
#define N 9
#define NS N*N
#define pb push_back

vector< vector<int> > NEIGHBOURS(NS);

void printGrid(vector<int> grid)
{
	int i,j;
	for (i = 0; i < N; ++i)
	{
		for (j = 0; j < N; ++j)
		{
			cout << grid[N*i+j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void printVector(vector<int> vec)
{
	int i, j;
	for (i = 0; i < vec.size(); ++i)
	{
		cout << vec[i] << " ";
	}
	cout << endl;
}


vector<int> generateGrid(string puzzle)
{
	int i, j;
	vector<int> grid(NS);

	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
		{
			if (puzzle[N*i + j] == '.')
			    grid[N*i+j] = 0;
			else
				grid[N*i+j] = (puzzle[N*i + j] - '0');
		}
	return grid;
}

class SudokuSolver
{
	vector<int> _grid;
	bool _isFilled[NS];
	vector< vector<int> > _possibilites;
public:
	SudokuSolver(vector<int>);
	void setValue(int, int);
	void removeValue(int, int);
	void generatePossiblities();
	int findMinPossiblities();
	void solve();
	bool isCompleted();
};

SudokuSolver::SudokuSolver(vector<int> puzzle)
:_grid(NS), _possibilites(NS)
{
	int i;
	_grid.swap(puzzle);
	for (i = 0; i < NS; ++i)
	{
		_isFilled[i] = false;
	}

	for (i = 0; i < NS; ++i)
	{
		if (_grid[i] != 0) {
			_isFilled[i] = true;
		}
	}

	generatePossiblities();
}

void SudokuSolver::setValue(int idx, int val)
{
	int i,
	    tmpIdx,
	    vec_sz = NEIGHBOURS[idx].size();

	_isFilled[idx] = true;
	_grid[idx] = val;

	for (i = 0; i < vec_sz; ++i)
	{
		tmpIdx = NEIGHBOURS[idx][i];
		if (!_isFilled[tmpIdx])
		{
			removeValue(tmpIdx, val);
		}
	}

	_possibilites[idx].clear();
}

void SudokuSolver::removeValue(int idx, int val)
{
	int i,
	    vec_sz = _possibilites[idx].size();

	for (i = 0; i < vec_sz; ++i)
	{
		if (val == _possibilites[idx][i])
		{
			break;
		}
	}

	if (i != vec_sz)
	{
		_possibilites[idx].erase(_possibilites[idx].begin()+i);
	}
}

void SudokuSolver::generatePossiblities()
{
	int i, j, crv;
	int posVal[10] = {0};
	for (i = 0; i < NS; ++i)
	{
		if (!_isFilled[i])
		{
			for (j= 0; j < 10; ++j)
			{
				posVal[j] = 0;
			}

			for (j = 0; j < NEIGHBOURS[i].size(); ++j)
			{
				if (_isFilled[NEIGHBOURS[i][j]])
				{
					crv = _grid[NEIGHBOURS[i][j]];
					posVal[crv] = 1;
				}
			}

			for (j = 1; j < 10; ++j)
				if (posVal[j] == 0)
					_possibilites[i].pb(j);
			//cout << "Index: " << i << endl;
			//printVector(_possibilites[i]);
		}
	}
}

int SudokuSolver::findMinPossiblities()
{
	int minV = INT_MAX, i, tmp, minI = -1;
	for (i = 0; i < NS; ++i)
	{
		if (!_isFilled[i])
		{
			tmp = _possibilites[i].size();
			if (minV > tmp)
			{
				minV = tmp;
				minI = i;
			}
		}
	}

	return minI;
}



void SudokuSolver::solve()
{
	int i, minP = findMinPossiblities();

	while (minP > -1 && _possibilites[minP].size() == 1)
	{
		setValue(minP, _possibilites[minP][0]);
		minP = findMinPossiblities();
	}

	if (minP == -1)
	{
		printGrid(_grid);
	} else {
		if (_possibilites[minP].size() > 1)
			for (i = 0; i < _possibilites[minP].size(); ++i)
			{
				// cout << "Filling Value at " << minP << endl;
				// cout << " Value Filled " << _possibilites[minP][i] << endl;
				// printGrid(_grid);
				_grid[minP] = _possibilites[minP][i];
				SudokuSolver sdksl(_grid);
				sdksl.solve();
			}
		// else
		// 	cout << " Zero size Found!! " << _possibilites[minP].size() << endl;
	}
}

void generateNeighbours()
{
	int i, r, c, j, k;
	for (i = 0; i < NS; ++i)
	{
		r = i/9;
		c = i%9;
		for (j = 0; j < 9; ++j)
		{
			if (9*r+j != i)
			   NEIGHBOURS[i].pb(9*r+j);
		}

		for (j = 0; j < 9; ++j)
		{
			if (c+9*j != i)
			   NEIGHBOURS[i].pb(c+9*j);
		}

		// neighbours in square 
		for (j = 3*(r/3); j < (3*((r/3)+1)); ++j)
			for (k = 3*(c/3); k < (3*((c/3)+1)); ++k)
				if (j != r && k != c)
					NEIGHBOURS[i].pb(9*j+k);
	}
}

string testPuzzule()
{
	string pz1 = ".....6....59.....82....8....45........3........6..3.54...325..6..................",
	       pz2 = "4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......",
	       pz3 = "85...24..72......9..4.........1.7..23.5...9...4...........8..7..17..........36.4.",
	       pz4 = "..53.....8......2..7..1.5..4....53...1..7...6..32...8..6.5....9..4....3......97..",
	       pz5 = "12..4......5.69.1...9...5.........7.7...52.9..3......2.9.6...5.4..9..8.1..3...9.4",
	       pz6 = "...57..3.1......2.7...234......8...4..7..4...49....6.5.42...3.....7..9....18.....",
	       pz7 = "7..1523........92....3.....1....47.8.......6............9...5.6.4.9.7...8....6.1.",
	       pz8 = "1....7.9..3..2...8..96..5....53..9...1..8...26....4...3......1..4......7..7...3..",
	       pz9 = "1...34.8....8..5....4.6..21.18......3..1.2..6......81.52..7.9....6..9....9.64...2",
	      pz10 = "...92......68.3...19..7...623..4.1....1...7....8.3..297...8..91...5.72......64...",
	      pz11 = ".6.5.4.3.1...9...8.........9...5...6.4.6.2.7.7...4...5.........4...8...1.5.2.3.4.",
	      pz12 = "7.....4...2..7..8...3..8.799..5..3...6..2..9...1.97..6...3..9...3..4..6...9..1.35",
	      pz13 = "....7..2.8.......6.1.2.5...9.54....8.........3....85.1...3.2.8.4.......9.7..6....";
	return pz6;
}

int main()
{
	string puzzle = testPuzzule();
	vector<int> grid = generateGrid(puzzle);
	printGrid(grid);
	generateNeighbours();
	SudokuSolver solvr(grid);
	solvr.solve();
}