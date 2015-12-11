#pragma once
#include "Matrix.h"



class binaryImage : public Matrix
{
public:
	binaryImage::binaryImage();
	binaryImage::~binaryImage();

	binaryImage::binaryImage(int M, int N, double* input_data, double threshold);

	binaryImage binaryImage::operator=(const binaryImage& other);
	binaryImage binaryImage::operator-(const binaryImage& other);

	Matrix binaryImage::NNS(binaryImage shuffled, binaryImage noisy, Matrix unshuffled);

	
private:
	int _threshold;
};

