#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <pthread.h>
#define COM_ADD 1
#define COM_SUB 2 
#define COM_MUL 3
#define COM_EXIT 4
	using namespace std;

struct arguments {
	int row;
	int col;
	double **matrix;
};

void *print_matrix(void * arg_void_ptr) {
	struct arguments *arg_ptr = (struct arguments *)arg_void_ptr;
	cout << "Size = (" << arg_ptr->row << "," << arg_ptr->col << ")" << endl;
	for (int i=0;i<arg_ptr->row;i++) {
		for (int j=0;j<arg_ptr->col;j++) {
			cout << arg_ptr->matrix[i][j] << " ";
		}
		cout << endl;
	}
	return NULL;
}

bool validateOP(struct arguments args,int command) {
	if (command == COM_ADD || command == COM_SUB) {

	}

}
int main() {
	int command = 0;
	string line;
	struct arguments matrices[2];
	
	// Read matrices from file
	ifstream inFile("inputfile");
	if (inFile.is_open()) {
		for (int k = 0; k < 2; k++) {
			inFile >> matrices[k].row >> matrices[k].col;
			double **m = new double*[matrices[k].row];
			for(int i = 0; i < matrices[k].row; i++) {
				m[i] = new double[matrices[k].col];
			}
			for(int i = 0; i < matrices[k].row;i++) {
				for( int j = 0; j < matrices[k].col;j++){
					inFile >> m[i][j];		
				}
			}			

		
			matrices[k].matrix = m;			
			print_matrix(matrices);


		}
		inFile.close();
	} else {
		cout << "File could not be opened" << endl;
	}
	// Show menu options 
	while (true) {
		cout << "Select Operations " << endl;
		cout << "\tAddition       (1)" << endl;
		cout << "\tSubtraction    (2)" << endl;
		cout << "\tMultiplication (3)" << endl;
		cout << "\tExit           (4)" << endl;
		cout << "Command: ";
		cin >> command;	
		
		switch(command) {
			case COM_ADD:
//				validateOP(matrix, COM_ADD);

			break;

			case COM_SUB:

			break;

			case COM_MUL:
			break;

			default:
				cout << "not valid option" << endl;
		}
		
		if (command == COM_EXIT) break;
	


	}
	return 0;
	


}
