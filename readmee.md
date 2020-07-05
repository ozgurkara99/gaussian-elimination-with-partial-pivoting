You should compile the cpp file with writing into terminal "g++ ozgur-kara-main.cpp -o main.exe". It makes an exe file of the cpp. Then you can run this exe file,
command line arguments are "main.exe input_A_filename.txt input_b_filename.txt" and output will be "X.txt" (you should write in this form to terminal)

This program is basically read the A matrix and b vector from files whose names are given. Then solves Ax=b system using "gaussian elimination with
partial pivotting". It means that, while making forward substitution, the program must use the pivot as the value which is the absolute
maximum of this column.

When reading A matrix from input_A_filename.txt , I dynamically create A matrix. (I reallocate the matrix's size when new row has appeared)

If given matrix is 2x2 matrix, then it prints out the condition number of (one and infinity which are the same actually) this matrix.

If given matrix is singular matrix, then the output will be writing "singular matrix" in "X.txt" file.

If we look on that problem in procedure:
     A                    b1       b2
1.000 1.000             2.000    2.000
1.000 1.001             2.000    2.001

I have solved this equation and the outputs x1 and x2 like this:

   x1    x2
   2     1
   0     1

It is clear that x vector has extremely changed (2,0 to 1,1) because the condition number is very large (4004) so it's nearly singular matrix. so when
b vector changes a little bit, x vector will change a large value.
