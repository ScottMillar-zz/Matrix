#include <iostream>
#include "Matrix.h"
#include "binaryImage.h"
#include <sstream> // stringstream
#include <fstream> // ifstream
#include <istream>

	using namespace std;

	// Reads .txt file representing an image of R rows and C Columns stored in filename 
	// and converts it to a 1D array of doubles of size R*C
	// Memory allocation is performed inside readTXT
	double* readTXT(char *fileName, int sizeR, int sizeC);

	// Converts a 1D array of doubles of size R*C to .pgm image of R rows and C Columns 
	// and stores .pgm in filename
	// Use Q = 255 for greyscale images and Q=1 for binary images.
	void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q);

	Matrix add(Matrix& one, Matrix& two);


	int main()
	{
		int M = 512; int N = 512;
		// input_data is a pointer to a 1D array of M*N doubles stored in heap. Memory allocation is performed 
		// inside readTXT. readTXT will read an image (in .pgm format) of size MxN and will  store the result in input_data.
		// once you're done with data DO NOT forget to delete the memory as in the end of this main() function
		double* input_data = 0;
		double* output_data = 0;
		
		
		// .pgm image is stored in inputFileName, change the path in your program appropriately
		char* inputFileName = "shuffled_logo.txt";
		input_data = readTXT(inputFileName, M, N);
		Matrix shuffled(512, 512, input_data);
		binaryImage binaryShuffled(512, 512, input_data, 75);
		
		inputFileName = "unshuffled_logo_noisy.txt";
		input_data = readTXT(inputFileName, M, N);
		Matrix noisy(512, 512, input_data);
		binaryImage binaryNoisy(512, 512, input_data, 75);
		delete input_data;
		
		output_data = shuffled.getData();
		Matrix unshuffled(512, 512, output_data);
		
		unshuffled = unshuffled - unshuffled;
		unshuffled = unshuffled.NNS(shuffled, noisy, unshuffled);
		output_data = unshuffled.getData(); 
		// writes data back to .pgm file stored in outputFileName
		char* outputFileName = "logo_restored.pgm";
		char* binaryOutputFileName = "binary_logo_restored.pgm";
		// Use Q = 255 for greyscale images and 1 for binary images.
		int Q = 255;
		WritePGM(outputFileName, output_data, M, N, Q);

		unshuffled = unshuffled - unshuffled;
		unshuffled = unshuffled.NNS(binaryShuffled, binaryNoisy, unshuffled);
		output_data = unshuffled.getData();
		// writes data back to .pgm file stored in outputFileName
		outputFileName = "binary_logo_restored.pgm";
		// Use Q = 255 for greyscale images and 1 for binary images.
		Q = 1;
		WritePGM(outputFileName, output_data, M, N, Q);
		

		system("pause");
		return 0;
	}

	Matrix add(Matrix& one, Matrix& two)
	{
		//create 'new' data array on heap
		double* data = new double[one.getM()*one.getN()];

		//fill that data array
		for (int x = 0; x < (one.getM()*one.getN()); x++)
		{
			data[x] = one.getData()[x] + two.getData()[x];
		}

		//create new Matrix object with the row/column/data information
		Matrix temp(one.getM(), one.getN(), data);
		//delete 'data' array
		//remember: because of 'deep copy' we can delete this array, as it has been 'deep copied'
		//when the 'temp' object was created.
		delete[] data;

		return temp;
	}

	// Read .txt file with image of size RxC, and convert to an array of doubles
	double* readTXT(char *fileName, int sizeR, int sizeC)
	{
		double* data = new double[sizeR*sizeC];
		int i = 0;
		ifstream myfile(fileName);
		if (myfile.is_open())
		{

			while (myfile.good())
			{
				if (i>sizeR*sizeC - 1) break;
				myfile >> *(data + i);
				// cout << *(data+i) << ' '; // This line display the converted data on the screen, you may comment it out. 
				i++;
			}
			myfile.close();
		}

		else cout << "Unable to open file";
		//cout << i;

		return data;
	}

	// convert data from double to .pgm stored in filename
	void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q)
	{

		int i, j;
		unsigned char *image;
		ofstream myfile;

		image = (unsigned char *) new unsigned char[sizeR*sizeC];

		// convert the integer values to unsigned char

		for (i = 0; i<sizeR*sizeC; i++)
			image[i] = (unsigned char)data[i];

		myfile.open(filename, ios::out | ios::binary | ios::trunc);

		if (!myfile) {
			cout << "Can't open file: " << filename << endl;
			exit(1);
		}

		myfile << "P5" << endl;
		myfile << sizeC << " " << sizeR << endl;
		myfile << Q << endl;

		myfile.write(reinterpret_cast<char *>(image), (sizeR*sizeC)*sizeof(unsigned char));

		if (myfile.fail()) {
			cout << "Can't write image " << filename << endl;
			exit(0);
		}

		myfile.close();

		delete[] image;

	}
