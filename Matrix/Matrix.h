#pragma once
class Matrix
{
public:
	Matrix::Matrix();
	Matrix::~Matrix();

	Matrix::Matrix(int sizeR, int sizeC, double* input_data);
	Matrix::Matrix(const Matrix& m);

	Matrix Matrix::operator+(const Matrix& other);
	Matrix Matrix::operator=(const Matrix& other);
	Matrix Matrix::operator-(const Matrix& other);
	Matrix Matrix::operator*(const Matrix& other);
	double Matrix::getTotal();

	int Matrix::getM();
	int Matrix::getN();
	double Matrix::get(int i, int j);
	Matrix Matrix::getBlock(int start_row, int end_row, int start_column, int end_column);
	Matrix Matrix::setBlock(int start_row, int start_column, Matrix block);
	double* Matrix::getData();
	double Matrix::compare(Matrix shuffled, Matrix noisy);
	Matrix Matrix::NNS(Matrix shuffled, Matrix noisy, Matrix unshuffled);


protected:
	int _M, _N;
	double *_data;
};

