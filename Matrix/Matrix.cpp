#include <iostream>
#include "Matrix.h"

Matrix::Matrix()
{
	_M = 1;
	_N = 1;

	_data = new double[_M*_N];

	for (int x = 0; x < _M*_N; x++)
	{
		_data[x] = 0;
	}

}

Matrix::Matrix(int sizeR, int sizeC, double* input_data)
{
	_M = sizeR;
	_N = sizeC;

	_data = new double[_M*_N];

	for (int x = 0; x < _M*_N; x++)
	{
		_data[x] = input_data[x];
	}
}
//copy constructor
Matrix::Matrix(const Matrix& m)
{

	_M = m._M;
	_N = m._N;

	_data = new double[_M*_N];

	for (int i = 0; i < _M*_N; i++)
	{
		_data[i] = m._data[i];
	}
}

//operator overloads
Matrix Matrix::operator+(const Matrix& other)
{

	Matrix temp;
	temp._M = other._M;
	temp._N = other._N;

	temp._data = new double[temp._M*temp._N];

	for (int x = 0; x < (temp._M*temp._N); x++)
	{
		temp._data[x] = this->_data[x] + other._data[x];
	}

	return temp;
}

Matrix Matrix::operator-(const Matrix& other)
{

	Matrix temp;
	temp._M = other._M;
	temp._N = other._N;

	temp._data = new double[temp._M*temp._N];

	for (int x = 0; x < (temp._M*temp._N); x++)
	{
		temp._data[x] = this->_data[x] - other._data[x];
	}

	return temp;
}

Matrix Matrix::operator*(const Matrix& other)
{

	Matrix temp;
	temp._M = other._M;
	temp._N = other._N;

	temp._data = new double[temp._M*temp._N];

	for (int x = 0; x < (temp._M*temp._N); x++)
	{
		temp._data[x] = this->_data[x] * other._data[x];
	}

	return temp;
}

Matrix Matrix::operator=(const Matrix& other)
{

	//delete existing _data information - as we are going to replace it with 'other._data'
	delete[] _data;
	_M = other._M;
	_N = other._N;

	//reserve memory for new array
	_data = new double[_M*_N];

	//'this' pointer refers to the current object
	for (int x = 0; x < (_M*_N); x++)
	{
		this->_data[x] = other._data[x];
	}

	return *this;
}

double Matrix::getTotal()
{
	double total;

	for (int x = 0; x < (_M*_N); x++)
	{
		total = this->_data[x];
	}

	return total;
}


//functions
int Matrix::getM()
{
	return _M;
}

int Matrix::getN()
{
	return _N;
}

double Matrix::get(int i, int j)
{
	return _data[(i*_N) + j];

}

Matrix Matrix::getBlock(int start_row, int end_row, int start_column, int end_column)
{
	//set number of rows and columns in the subimage
	int rows = (end_row - start_row) + 1;
	int columns = (end_column - start_column) + 1;
	//reserve the memory for data extracted from the main image
	double* tempData = new double[rows*columns];

	//variable to set the new array index
	int count = 0;

	//iterate through the image data, extracting the values according to the rows/columns
	for (int x = start_row; x < (rows + start_row); x++)
	{
		for (int y = start_column; y < (columns + start_column); y++)
		{
			tempData[count] = get(x, y);
			count++;
		}
	}

	//create a temporary matrix based on the row/column/data extracted
	Matrix temp(rows, columns, tempData);
	//delete the heap memory reserved for the extracted data
	delete[] tempData;

	//return the temporary matrix
	return temp;
}

Matrix Matrix::setBlock(int start_row, int start_column, Matrix block)
{
	//iterate through the image data, setting the values according to the rows/columns
	for (int x = 0; x < block.getM() ; x++)
	{
		for (int y = 0; y < block.getN(); y++)
		{
			this->_data[((x+start_row)*_N) + (y+start_column)] = block.get(x, y);
			/*std::cout << start_column << std::endl;*/
		}
	}

	return *this;
}


double* Matrix::getData()
{
	return _data;	
}

double Matrix::compare(Matrix shuffled, Matrix noisy)
{
	double counter = 0;

	for (int i = 0; i < shuffled.getN(); i++)
	{
		for (int j = 0; j < shuffled.getM(); j++)
		{
			if (shuffled.get(i, j) == noisy.get(i, j))
			{
				counter++;
			}
		}
	}

	return counter;
}



Matrix Matrix::NNS(Matrix shuffled, Matrix noisy, Matrix unshuffled)
{
	std::cout << "NSS is initialized" << std::endl;
	int shuffledX1, shuffledX2, shuffledY1, shuffledY2;
	int noisyX1, noisyX2, noisyY1, noisyY2, bestX1, bestY1;
	double similarity;
	double bestSimiliarty = 0;
	Matrix diff, blockShuffled, blockNoisy;

	for (int i = 0; i < 16; i++)
	{
		//setting y for shuffled.getBlock
		shuffledY1 = i * 32;
		shuffledY2 = (i * 32) + 32; 

		for (int j = 0; j < 16; j++)
		{
			//setting x for shuffled.getBlock
			shuffledX1 = j * 32;
			shuffledX2 = (j * 32) + 32;
			blockShuffled = shuffled.getBlock(shuffledX1, shuffledX2-1, shuffledY1, shuffledY2-1);

			for (int m = 0; m < 16; m++)
			{
				//setting y for shuffled.get
				noisyY1 = m * 32;
				noisyY2 = (m * 32) + 32;


				for (int n = 0; n < 16; n++)
				{

					//setting x for shuffled.get
					noisyX1 = n * 32;					
					noisyX2 = (n * 32) + 32;
					
					
					blockNoisy = noisy.getBlock(noisyX1, noisyX2-1, noisyY1, noisyY2-1);

					//SSD
					//diff =  blockNoisy - blockShuffled; //find the difference between the two matrices
					//diff = diff*diff; //square the values of the diff matrices
					//similarity = diff.getTotal();

					similarity = compare(blockNoisy, blockShuffled);
					

					if (similarity > bestSimiliarty)
					{
						bestX1 = noisyX1;
						bestY1 = noisyY1;
						bestSimiliarty = similarity;
					}
					
						
				}
				
			}
			unshuffled.setBlock(bestX1, bestY1, blockShuffled);			
			bestSimiliarty = 0;
			
		}
		
	}

	return unshuffled;
}



Matrix::~Matrix()
{
	delete[] _data;
}