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


struct position {
	int row;
	int col;
};
struct position_mul {
	double *e_row;
	double *e_col;
	int n_row;
	int n_col;
};

static struct arguments matrices[2];
static struct arguments result;
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



bool validateOP(struct arguments arg1,struct arguments arg2, int command) {
	if (command == COM_ADD || command == COM_SUB) {
		if ((arg1.row == arg2.row) && (arg1.col == arg2.col))
			return true;
	} else if (command == COM_MUL) {
		if (arg1.col == arg2.row)
			return true;
	}
	
	return false;

}



void *add(void *p){
	struct position *pos = (struct position *)p;
	int a = pos->row;
	int b = pos->col;
	printf("%d %d\n", a, b);
	result.matrix[a][b] = matrices[0].matrix[a][b] + matrices[1].matrix[a][b]; 	
	return NULL;
}

void *subtract(void *p){
	struct position *pos = (struct position *)p;
	int a = pos->row;
	int b = pos->col;
	printf("%d %d\n", a, b);
	result.matrix[a][b] = matrices[0].matrix[a][b] - matrices[1].matrix[a][b]; 	
	return NULL;
}


void *multiply(void *p){
	struct position_mul *pos = (struct position_mul *)p;
	int a = pos->n_row;
	int b = pos->n_col;
	printf("%d %d\n", a, b);
	result.matrix[a][b] = 2; 	
	//result.matrix[a][b] = c++; 	
	return NULL;
}
int main() {


	int command = 0;
	string line;
	
	
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
			print_matrix(& matrices[k]);

		}
		inFile.close(); // close file after successful reading
	} else {
		cout << "File could not be opened" << endl;
	}
	// test code-------------------------------------------
	if (validateOP(matrices[0],matrices[1],COM_SUB)) 
		cout << "TRUE" << endl;
	else 
		cout << "FALSE" << endl;
	// test code-------------------------------------------


	// Show menu options 
	while (true) {
		cout << "Select Operations " << endl;
		cout << "\tAddition       (1) " << (validateOP(matrices[0],matrices[1], COM_ADD) ? "Y" : "N") << endl;
		cout << "\tSubtraction    (2) " << (validateOP(matrices[0],matrices[1], COM_SUB) ? "Y" : "N") << endl;
		cout << "\tMultiplication (3) " << (validateOP(matrices[0],matrices[1], COM_MUL) ? "Y" : "N") << endl;
		cout << "\tExit           (4) " << endl;
		cout << "Command: ";
		cin >> command;	
		
		if (command == COM_EXIT) break;
		switch(command) {
			case COM_ADD:
				if (validateOP(matrices[0],matrices[1], COM_ADD)) {
					result.row = matrices[0].row;
					result.col = matrices[0].col;	
					double **m = new double*[result.row];
					for(int i = 0; i < result.row; i++) {
						m[i] = new double[result.col];
					}

					result.matrix = m; 
					
					int numThread = result.row * result.col;

					struct position pos;
					int k = 0;
					pthread_t tid[numThread];
					for(int i = 0; i < result.row; i++){
						for(int j=0;j<result.col;j++) {
							pos.row = i;
							pos.col = j;
							cout << endl <<"inside loop " << pos.row << " " << pos.col << endl;
							pthread_create(&tid[k++],NULL,add,&pos);	
							cout << tid[k-1] << endl;
						}


					}
					//wait until all thread execution is complete	
					for (int i =0 ;i<numThread; i++)
						pthread_join(tid[i],NULL);	


					print_matrix(&result);
				} 
					

			break;

			case COM_SUB:
				if (validateOP(matrices[0],matrices[1], COM_SUB)) {
					result.row = matrices[0].row;
					result.col = matrices[0].col;	
					double **m = new double*[result.row];
					for(int i = 0; i < result.row; i++) {
						m[i] = new double[result.col];
					}

					result.matrix = m; 
					
					int numThread = result.row * result.col;

					struct position pos;
					int k = 0;
					pthread_t tid[numThread];
					for(int i = 0; i < result.row; i++){
						for(int j=0;j<result.col;j++) {
							pos.row = i;
							pos.col = j;
							pthread_create(&tid[k++],NULL,subtract,&pos);	
							cout << tid[k-1] << endl;
						}
					}
					//wait until all thread execution is complete	
					for (int i =0 ;i<numThread; i++)
						pthread_join(tid[i],NULL);	


					print_matrix(&result);
				} 
			break;

			case COM_MUL:
				if (validateOP(matrices[0],matrices[1], COM_MUL)) {

					struct position_mul pos;

					result.row = matrices[0].row;
					result.col = matrices[1].col;	
					cout << result.row << " by " << result.col;
					double **m = new double*[result.row];
					for(int i = 0; i < result.row; i++) {
						m[i] = new double[result.col];
					}

					// take all entire row 
					double *r = new double[matrices[0].col];
					double *c = new double[matrices[0].col];
					for (int j = 0; j < matrices[0].row; j++) { 
					for (int i = 0; i < matrices[0].col; i++) {
						r[i] = matrices[0].matrix[j][i];
						c[i] = matrices[1].matrix[i][j];

						}
						pos.e_row = r; 
						pos.e_col = c;
					}

					result.matrix = m; 
					int numThread = result.row * result.col;
					pthread_t tid[numThread];
					int k = 0;
					for (int i = 0; i < result.row; i++) {
						for(int j = 0; j < result.col;j++) {
							pos.n_row = i;
							pos.n_col = j;
						
							pthread_create(&tid[k],NULL,multiply,&pos);	

							cout << endl << pos.n_row << " | "  <<  pos.n_col<< endl;
							cout << tid[k] << endl;
							k = k+1;
						}
					}	


					for (int i =0 ;i<numThread; i++)
						pthread_join(tid[i],NULL);	

					


					result.matrix = m; 
					print_matrix(&result);
				}		
			break;

			default:
				cout << "not valid option" << endl;
		}
		
	


	}
	return 0;
	


}
