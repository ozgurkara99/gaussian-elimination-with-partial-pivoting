#include <iostream> 
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cmath> 
#include <cstdio>
#include <cstdlib>

using namespace std; 

//function for reading a 1d array from b.txt
void read_b(string name, double *arr)
{
	//open the file with given name
	ifstream file(name.c_str());
	//initialize row number as zero
	int R = 0;
	//create string "object" line
	string line;
	while (getline(file, line)) {
    	//strtod is converting the string to double, then assign the array's values
    	//strtod has an input char pointer, so we should convert "line" which is the string object to char array using c_str()
    	arr[R] = strtod(line.c_str(),NULL);
    	//update the Row number value
    	R = R + 1;	
	} 
}

//function for reading a 2d array from A.txt and finding the size of nxn matrix
//function takes the filename and pointer of size so that we can update the size when reading the A matrix
double** read_A(string name, int *R)
{
	//open the file with the name "name" (converting it to const char array from string using c_str())
	ifstream file(name.c_str());
	//initialize column number as zero
	int C = 0;
	//create null double 2d array (2d array is actually a pointer of pointer)
	double **arr = NULL;
	string line, temp;
	//while it gets line from the file, it updates the matrix 
	while (getline(file, line)) {
		//reallocate a pointer to pointer with the size of updating row number 
		arr = (double **)realloc(arr,(*R+1)*sizeof(double*));
		//initialize it's column as null so that we can update (reallocate) when it finds a number in the line string
		arr[*R] = NULL;
		//we should convert the string line to char array using c_str(). it converts to const char array so we duplicate it to make it char array
	    char * dup_str = strdup(line.c_str()); 
	    // Declaration of delimiter with any whitespace (tab or space)
	    //strtok splits the string using delimiters. it returns the first item until it sees the delimiter and the second part is in it's memory
		//for ex dup_str is 123 456 789, then tok is "123", and "456 789" is in the memory of strtok so that we can return other tokens in the while loop 
	    char* tok = strtok(dup_str, "\t "); 	  
	    while (1) {
	    	//if no token found, then break
	        if(tok == NULL)
	        	break;
	        //reallocate the columns of array when it finds a new number to add in a row
			arr[*R] = (double*)realloc(arr[*R],(C+1)*sizeof(double)); 
			//strtod converts the token to double and assign it to array
			arr[*R][C] = strtod(tok,NULL);
			//update column number by incrementing
    		C = C + 1;
    		//find the next number
    		tok = strtok(NULL, "\t ");
	    } 
   		C = 0;
		*R = *R + 1;		
  	}
  	return arr;
}



//function for swapping given 2 rows of given A matrix and b vector
void swapRows(double **arr, int row1, int row2, int size, double *b)
{
	double temp = 0;
	double temp2 = 0;
	//changing the rows of b vector
	temp2 = b[row1];	
	b[row1] = b[row2];
	b[row2] = temp2;
	//changing the values one by one in A matrix
	for (int i=0;i<size;i++)
	{
		temp = arr[row1][i];
		arr[row1][i] = arr[row2][i];
		arr[row2][i] = temp;
	}
}

//finding index of maximum value in a column
int findMaxColumn (double **arr, int size, int k)
{
	int index = k;
	for (int i=k;i<size;i++)
	{
		//fabs for the absolute value of double
		if(fabs(arr[i][k])>fabs(arr[index][k]))
		{
			index = i;
		}
	}
	return index;
}

//function for forward substitution
int forwardSub (double **arr, int size, double *b)
{
	int flag = 0;
	for(int k=0;k<size-1;k++)
	{
		//make partial pivoting, swap the kth row with the row that has the max pivot in it's kth column
		int pivotindex = findMaxColumn(arr,size,k);
		if (pivotindex!=k)
		{
			swapRows(arr, k, pivotindex, size, b);
		}

		for(int i=k+1;i<size;i++)
		{
			double rate = arr[i][k] / arr[k][k];
			//assign new values of b vector
			b[i] = b[i] - rate * b[k];
			for(int j=k;j<size;j++)
			{
				//assign new values of A matrix
				arr[i][j] = arr[i][j] - rate * arr[k][j];
				//for machine precision fabs is used, if pivot is zero, it's singular
				//double cannot be exact 0 so we should use like this
				if(fabs(arr[i][j])<0.00000001 && j!=k)
				{
					flag = flag + 1;
					break;
				}
			}
		}

	}
	return flag;
}
//function for backward substitution, finding the x values
void backwardSub(double **arr, double *x, double *b, int size)
{
    for(int i=size-1;i>=0;i--)
    {
    	//temporary sum
        double sum = 0;
        for(int j=i;j<size;j++)
        {
        	//sum of Aij * xj  as j increasing from i to size
        	sum = sum + arr[i][j]*x[j];
    	}
		//finding x vector one by one
    	x[i] = (b[i]-sum)/arr[i][i];
	}	
}

//function for finding inverse of an 2x2 matrix
void inverseMatrix(double **arr, double **inv)
{
	//find the determinant, then assign the inverse matrix values
	double det = arr[0][0] * arr[1][1] - arr[1][0] * arr[0][1];
	double temp = arr[1][1] / det;
	inv[1][1] = arr[0][0] / det;
	inv[0][0] = temp;
	inv[1][0] = -1*arr[1][0] / det;
	inv[0][1] = -1*arr[0][1] / det;
}
//function for finding condition number one of an 2x2 matrix
double conditionOne(double **arr)
{
	double sum1 = fabs(arr[0][0]) + fabs(arr[1][0]);
	double sum2 = fabs(arr[0][1]) + fabs(arr[1][1]);
	//return the maximum of sum1 or sum2 which are the column sums
	return sum1>=sum2 ? sum1 : sum2;
}
//function for finding condition number infinity of an 2x2 matrix
double conditionInf(double **arr)
{
	double sum1 = fabs(arr[0][0]) + fabs(arr[0][1]);
	double sum2 = fabs(arr[1][0]) + fabs(arr[1][1]);
	//return the maximum of sum1 or sum2 which are the row sums
	return sum1>=sum2 ? sum1 : sum2;
}

//function for writing x unknown vector into txt file whose name is given
void printX(int size, double *x, string filename)
{
	ofstream ofs(filename.c_str());
	for(int i=0;i<size;i++)
    	ofs << x[i] << endl;
}

int main(int argc, char** argv) 
{ 
	//gets the first input argument and assign it for the A matrix's filename
	string txt_file_A = argv[1];
	//gets the second input argument and assign it for the A matrix's filename
	string txt_file_b = argv[2];		
	// declaring dynamically 2d array , A matrix		 
	int size = 0;
    //the txt_file_A and assign it's values to A matrix
	double** A = read_A(txt_file_A,&size);

	//if the size of A matrix is 2, it prints the condition number of this matrix
	if(size == 2)
	{
		//dynamically create the inverse of matrix
		double **inv = new double*[2];
		for(int i = 0; i < 2; ++i)
		{
    		inv[i] = new double[2];
		}
		//assign the values to it's inverse
		inverseMatrix(A,inv);
		//print the condition number one and condition number infinity (they are equal in 2x2 matrix)
		//condition number of matrix is equal to the multiplication of it's norm and it's inverse's norm
		cout << "Condition number one of 2x2 matrix is: " << conditionOne(A) * conditionOne(inv)  << "\n";
		cout << "Condition number infinity of 2x2 matrix is: " << conditionInf(A) * conditionInf(inv)  << "\n";		
		//deleting the inverse matrix because no need
		for(int i = 0; i < 2; ++i) {
        	delete[] inv[i];   
    	}
    	delete[] inv;
	}
	//declaring b matrix
	double *b = new (nothrow) double[size];
	//read txt_file_b and assign it's values to b array
	read_b(txt_file_b, b);
	//create x array dynamically and initialize it with zeros
	double *x = new double[size]();
	//flag for detecting singularity of matrix
	int flag = 0;
	//flag becomes non zero if matrix is singular 
	flag = forwardSub(A,size,b);
	//if matrix is nonsingular, then make backward substitution and print it to X.txt file
	if (flag == 0)
	{
	backwardSub(A, x, b, size);	
	//delete b array because no need
	delete []b;
	//print the values of x to the file "X.txt"
	printX(size,x,"X.txt");
	}
	//if it's singular, then print the file "singular matrix"
	else
	{
		delete []b;
		ofstream ofs("X.txt");
		ofs<<"singular matrix";
	}
	
	//deleting the x vector
	delete []x;
	
	//freeing the A matrix because I reallocate it before dynamically
	for(int i = 0; i < size; ++i) {
       	free(A[i]);   
    }
    free(A);	
    return 0; 
} 
