#include <iostream>
#include <vector>
#include <deque>
#include <chrono>
#include <random>
#include <cmath>

using namespace std;

bool checkValid(vector<__int8>& state, const int j) { // checks if a move is valid.
	const int stateSize = state.size();

	for (int i = 0; i < stateSize; i++) {
		// Column Check
		if (state[i] == j) {
			return false;
		}

		// Diagonal Check
		if (state[i] + stateSize - i == j ||
			state[i] - stateSize + i == j) {
			return false;
		}
	}

	return true;
}

void displayBoard(const vector<__int8>& state, const int n) { // creates a 2d array and places the queens as a 1 for displaying the board.
	vector<vector<int>> board;
	vector<int> temp;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			temp.push_back(0);
		}
		board.push_back(temp);
		temp.clear();
	}

	for (int i = 0; i < n; i++) {
		board[i].at(state[i]) = 1;
	}

	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			cout << board[i].at(j) << " ";
		}
		cout << endl;
	}
}

void bfs(const int n) { // Breadth-first search algorithm.
	deque<vector<__int8>> q(1); // Creates a queue and initialises it with an empty board

	int depth = 0; // Counts depth of tree
	int goalCount = 0;
	__int64 parentCount = 1; // 1 for initial state/
	__int64 childCount = 0;

	// starts the timer.
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	while (!q.empty()) { // While q is not empty
		for (__int64 i = 0; i < parentCount; i++) { // Iterates for number of parents.
			for (int j = 0; j < n; j++) { // Iterates for number of childs per node (subtree).
				q.push_back(q.at(i)); // Creates child node and adds to queue
				q.back().push_back(j); // Adds queen to child.
				childCount++;
			}
		}

		for (int i = 0; i < parentCount; i++) { // Pops parents
			q.pop_front();
		}
		parentCount = childCount; // counter for children becoming the parents.
		childCount = 0;
		depth++; // Depth counter
		chrono::duration<double> depthTime = chrono::system_clock::now() - start;
		cout << "Depth: " << depth << " in " << depthTime.count() << "s\n";

		if (depth == n) { // If at n depth
			while (!q.empty()) { // Forloop for final level of depth
				int valid = 0;
				vector<__int8> check; // seperate board used to check if q.front() is a goal.
				check.push_back(q.front().at(0)); // push back q.fronts first position.
				for (int i = 1; i < n; i++) { // for q.fronts second position to q.fronts final position
					if (checkValid(check, q.front().at(i))) { // if q.fronts next position is valid, valid++
						valid++;
					}
					else { // else pop q.front and move to next q.front
						break;
					}
					check.push_back(q.front().at(i)); // push back next position in q.front
					if (valid == n - 1) { // if valid positions is n-1 goals++, n-1 because you dont need to check for q.fronts first position
						if (n < 7) { // displays the board
							displayBoard(q.front(), n);
							cout << endl;
						}
						goalCount++;
					}
				}
				q.pop_front(); // Throughout the forloop, this pops all children and evidently empties the queue.
			}
		}
	}

	// ends the timer.
	end = std::chrono::system_clock::now();
	chrono::duration<double> totalTime = end - start;

	cout << "Goals found = " << goalCount << endl;
	cout << "Time elapsed: " << totalTime.count() << "s" << endl;
}

void bfsPruned(const int n) { // Breadth-first search algorithm.
	deque<vector<__int8>> q(1); // Creates a queue and initialises it with an empty board

	int depth = 0; // Counts depth of tree
	int goalCount = 0;
	__int64 parentCount = 1; // 1 for initial state
	__int64 childCount = 0;

	// starts the timer.
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	while (!q.empty()) { // While q is not empty
		for (__int64 i = 0; i < parentCount; i++) { // Iterates for number of parents.
			for (int j = 0; j < n; j++) { // Iterates for number of childs per node (subtree).
				if (checkValid(q.front(), j)) { // Checks if is a valid place to add a queen
					q.push_back(q.front()); // Creates child node and adds to queue
					q.back().push_back(j); // Adds queen to child.
					childCount++;
				}
			}
			q.pop_front();
		}

		parentCount = childCount; // counter for children becoming the parents.
		childCount = 0;
		depth++; // Depth counter
		chrono::duration<double> depthTime = chrono::system_clock::now() - start;
		cout << "Depth: " << depth << " in " << depthTime.count() << "s\n";
		if (depth == n) { // If at n depth
			for (int i = 0; i < parentCount; i++) { // Forloop for final level of depth
				goalCount++;
				if (n < 7) { // displays the board.
					displayBoard(q.front(), n);
					cout << endl;
				}
				q.pop_front(); // Throughout the forloop, this pops all children and evidently empties the queue.
			}
		}
	}

	// ends the timer.
	end = std::chrono::system_clock::now();
	chrono::duration<double> totalTime = end - start;

	cout << "Goals found = " << goalCount << endl << "Time elapsed: " << totalTime.count() << "s" << endl;
}

int eval(const vector<__int8>& board, const int n) { // determines the total number of conflicts throughout the board state.
	int conflicts = 0;

	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (board[i] == board[j]) // column checking
				conflicts++;
			if (board[i] - (j - i) == board[j]) // right diagonal checking
				conflicts++;
			if (board[i] + (j - i) == board[j]) // left diagonal checking
				conflicts++;
		}
	}

	return conflicts;
}

deque<vector<__int8>> neighbourhood(const vector<__int8>& board, const int n) { // generates a neighbourhood of the current state.
	deque<vector<__int8>> neighbours;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			neighbours.push_back(board);
			neighbours.back().at(i) = j;
			if (neighbours.back().at(i) == board.at(i)) {
				neighbours.pop_back();
			}
		}
	}

	return neighbours;
}

vector<__int8> regenerate(vector<__int8>& board, const int n) { // creates a board of random queen placements.
	random_device device;
	mt19937 generator(device());
	uniform_real_distribution<double> distribution(0, n - 1);

	board.clear();

	for (int i = 0; i < n; i++) {
		board.push_back(distribution(generator));
	}

	return board;
}

void hillClimbing(const int n) {
	vector<__int8> board, temp;

	regenerate(board, n); // generates a random board.
	temp = board; // for displaying the original board at the end.

	deque<vector<__int8>> neighbours = neighbourhood(board, n); //  finds neighbours of current board.

	int min = 0, iter = 0, restarts = 0;

	// Starts the timer.
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	while (true) {
		int conflicts = eval(board, n); // evaluates number of conflicts on current board.
		vector<__int8> next; // creates board to be next.
		int min = INT_MAX;

		for (int i = 0; i < neighbours.size(); i++) { // Finds neighbour with best fitness
			if (eval(neighbours.at(i), n) < min) {
				next = neighbours.at(i); // becomes neighbour if neighbour is better than current board.
				min = eval(board, n);
			}
		}

		if (eval(next, n) < conflicts) { // if next board has less conflicts than current, moves to the next state.
			conflicts = eval(next, n);
			board = next;
			if (conflicts == 0) {
				if (n < 7) { // displays the boards.
					cout << "\nOriginal board:\n";
					displayBoard(temp, n);
					cout << "\nFinal board:\n";
					displayBoard(board, n);
					cout << endl;
					// ends the timer.
					end = std::chrono::system_clock::now();
					chrono::duration<double> totalTime = end - start;

					cout << "Final conflicts = " << conflicts << "\nIterations on current board: "
						<< iter << "\nRestarts = " << restarts << "\nTime elapsed: " << totalTime.count() << "s";
					return;
				}
				else {
					// ends the timer.
					end = std::chrono::system_clock::now();
					chrono::duration<double> totalTime = end - start;

					cout << "Final conflicts = " << conflicts << "\nIterations on current board: "
						<< iter << "\nRestarts = " << restarts << "\nTime elapsed: " << totalTime.count() << "s\n";
					return;
				}
			}
			neighbours.clear();
			neighbours = neighbourhood(board, n);
		}
		else { // if no better neighbour is found (at local max) regenerate to a new board state and start again.
			regenerate(board, n);
			temp = board;
			neighbours.clear();
			neighbours = neighbourhood(board, n);
			restarts++;
			iter = 0;
			continue;
		}
		iter++;
	}
}

void simulatedAnnealing(const int n) {
	vector<__int8> currentBoard, tempBoard;
	currentBoard = regenerate(currentBoard, n); // Creating a random board to be used in the algorithm.
	tempBoard = currentBoard; // for displaying the original board at the end.

	deque<vector<__int8>> neighbours = neighbourhood(currentBoard, n); // finds neighbouring boards
	int currentBoardConflicts = eval(currentBoard, n); // finds number of conflicts in current board.
	vector<__int8> nextBoard;
	int nextBoardConflicts = 0;

	// Randomisers
	random_device device;
	mt19937 generator(device());
	uniform_real_distribution<double> distribution(0.0, neighbours.size() - 1); // For choosing a random neighbour.
	uniform_real_distribution<double> randomPercent(0.0, 100.0); // For probability for becoming a random neighbour.

	double p = 0;
	double temperature = pow(n, 2);
	double coolingRateInput = 5;
	int k = 1000000; // Number iterations per constant temperature
	int costDifference = 0; // for comparing the cost difference of both boards

	cout << "Enter K (number of iterations before temperature cooling): ";
	cin >> k;
	cout << "Enter percent the percent temperature should cool by: ";
	cin >> coolingRateInput;
	if (k < 0 || coolingRateInput > 100) {
		cout << "\nInvalid input\n";
		return;
	}

	double coolingRate = (100 - coolingRateInput) / 100; // Cooling rate

	// Starting the timer
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	while (temperature > 0.001) {
		while (k > 0) {
			nextBoard = neighbours.at(distribution(generator)); // next board becomes a random neighbour of current board.
			nextBoardConflicts = eval(nextBoard, n); // evaluates conflicts of next board.

			costDifference = nextBoardConflicts - currentBoardConflicts; // The cost difference between the two states.
			p = exp((-costDifference) / temperature); // Decalring p value for the cost difference in the boards.
			int rando = randomPercent(generator); // Random percent for choosing next board in next if statement.

			if (nextBoardConflicts <= currentBoardConflicts || rando > p * 100) { // If next board is better than current, or chance of randomly going downhill, become the next board.
				currentBoard = nextBoard;
				currentBoardConflicts = nextBoardConflicts;
				neighbours = neighbourhood(currentBoard, n); // finds neighbours of new board.
			}
			if (currentBoardConflicts == 0) { // if at goal state.
				// Ending the timer
				end = std::chrono::system_clock::now();
				chrono::duration<double> totalTime = end - start;

				if (n < 7) { // Displaying initial state and final board to console.
					cout << "Initial state:\n";
					displayBoard(tempBoard, n);
					cout << endl;

					cout << "Global maximum found:\n";
					displayBoard(currentBoard, n);
					cout << endl;
				}

				cout << "Goal found for n = " << n << "\nTime elapsed: " << totalTime.count() << "s\n";
				return;
			}
			k--;
		}
		k = 0;
		temperature *= coolingRate; // Cooling the temperature.
	}

	// Ending the timer
	end = std::chrono::system_clock::now();
	chrono::duration<double> totalTime = end - start;

	if (n < 7) {
		displayBoard(currentBoard, n);
	}

	cout << endl;
	cout << "No global solution found.\n" << "Time elapsed: " << totalTime.count() << "s\n";;
}

int main() {
	int input = 0, n = 0;

	while (input != -1) {
		system("cls");
		cout << "N-QUEENS\n";
		cout << "1. Breadth-first search (BFS)\n2. Hill-climbing search\n3. Simulated Annealing (SA) search\n";
		cout << "Enter (-1 to exit): ";
		cin >> input;

		if (input == 1) {
			cout << "1. BFS without pruning\n2. BFS with pruning\n";
			cout << "Enter: ";
			cin >> input;

			cout << "Enter number of queens: ";
			cin >> n;

			if (n < 1) {
				cout << "\nInvalid input. Exiting...\n";
				return EXIT_FAILURE;
			}

			if (input == 1) {
				bfs(n);
			}
			else if (input == 2) {
				bfsPruned(n);
			}
			else {
				cout << "Invalid input. Exiting...\n";
				return EXIT_FAILURE;
			}
		}
		else if (input == 2) {
			cout << "Enter number of queens: ";
			cin >> n;

			if (n < 1) {
				cout << "\nInvalid input. Exiting...\n";
				return EXIT_FAILURE;
			}

			hillClimbing(n);
		}
		else if (input == 3) {
			cout << "Enter number of queens: ";
			cin >> n;

			if (n < 1) {
				cout << "\nInvalid input. Exiting...\n";
				return EXIT_FAILURE;
			}

			simulatedAnnealing(n);
		}
		else if (input == -1) {
			return EXIT_SUCCESS;
		}
		else {
			cout << "Invalid input. Exiting...\n";
			return EXIT_FAILURE;
		}
		system("pause");
	}
}