#include "binaryImage.h"
#include <iostream>

binaryImage::binaryImage()
{
	_M = 1;
	_N = 1;
	_threshold = 0;

	_data = new double[_M*_N];

	for (int x = 0; x < _M*_N; x++)
	{
		_data[x] = 0;
	}

}
	

binaryImage::binaryImage(int M, int N, double* input_data, double threshold)
{
	_M = M;
	_N = N;

	_data = new double[_M*_N];

	for (int x = 0; x < _M*_N; x++)
	{
		_data[x] = input_data[x];

	    if (_data[x] > threshold)
		{
			_data[x] = 1;
		}
		else
		{
			_data[x] = 0;
		}
	}
}

//operator overloads
binaryImage binaryImage::operator-(const binaryImage& other)
{

	binaryImage temp;
	temp._M = other._M;
	temp._N = other._N;

	temp._data = new double[temp._M*temp._N];

	for (int x = 0; x < (temp._M*temp._N); x++)
	{
		temp._data[x] = this->_data[x] - other._data[x];
	}

	return temp;
}

binaryImage binaryImage::operator=(const binaryImage& other)
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

Matrix binaryImage::NNS(binaryImage shuffled, binaryImage noisy, Matrix unshuffled)
{
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
			blockShuffled = shuffled.getBlock(shuffledX1, shuffledX2 - 1, shuffledY1, shuffledY2 - 1);

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


					blockNoisy = noisy.getBlock(noisyX1, noisyX2 - 1, noisyY1, noisyY2 - 1);

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

binaryImage::~binaryImage()
{
}
