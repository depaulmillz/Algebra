//
// Created by dePaul Miller on 3/22/21.
//

#ifndef ALGEBRA_MATRIX_HH
#define ALGEBRA_MATRIX_HH

#include <cassert>

template<class F>
class IndexObject {
public:
    IndexObject(int row_idx, int rows, int columns, F *data) : row_idx_(row_idx), rows_(rows), columns_(columns),
                                                                   data_(data) {
    }

    ~IndexObject() {

    }

    inline F &operator[](int i) {
        return data_[row_idx_ + rows_ * i];
    }

private:
    int row_idx_;
    int rows_;
    int columns_;
    F *data_;
};

// This is column major order

template<class F>
class Matrix {
public:
    Matrix(int rows, int columns, const F& zero) : rows_(rows), columns_(columns), Zero(zero), data(new F[rows * columns]) {

    }

    Matrix(int rows, int columns, const F& zero, F *d) : rows_(rows), columns_(columns), Zero(zero), data(d) {

    }

    ~Matrix() {
        delete[] data;
    }

    inline Matrix multiply(const Matrix &rhs) {

        assert(columns_ == rhs.rows_);

        F *result = new F[rows_ * rhs.columns_];

        for (int i = 0; i < rows_ * rhs.columns_; i++) {
            result[i] = Zero;
        }

        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < rhs.columns_; j++) {
                for (int k = 0; k < columns_; k++) {
                    result[i + j * rows_] += data[i + k * rows_] * rhs.data[k + j * columns_];
                }
            }
        }

        return Matrix(rows_, rhs.columns_, Zero, result);
    }

    inline IndexObject<F> operator[](int i) {
        return IndexObject(i, rows_, columns_, data);
    }

    inline void swapRows(int r1, int r2) {
        for(int i = 0; i < columns_; i++){
            auto tmp = this->operator[](r1)[i];
            this->operator[](r1)[i] = this->operator[](r2)[i];
            this->operator[](r2)[i] = tmp;
        }
    }

    inline Matrix<F> augment(const Matrix<F>& vector) {
        assert(vector.columns_ == 1 && rows_ == vector.rows_);
        //int rows = rows_;
        //int columns = columns_ + 1;
        F* result = new F[rows_ * (columns_ + 1)];
        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < columns_; j++) {
                result[i + j * rows_] = this->data[i * j * rows_];
            }
            result[i + columns_ * rows_] = vector.data[i];
        }

        return Matrix<F>(rows_, columns_, Zero, result);
    }

private:
    int rows_;
    int columns_;
    const F Zero;
    F *data;
};

#endif //ALGEBRA_MATRIX_HH
