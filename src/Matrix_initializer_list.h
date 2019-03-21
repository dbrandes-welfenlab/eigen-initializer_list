/* Author: hauptmech <hauptmech@gmail.com>, Nov 2013 */

#include <initializer_list>


/** Constructs a matrix from a C++11 initializer_list

  MatrixXd a {1,2,3,4} //4x1 matrix
  VectorXd b {6,5,8,6} // 4 element vector
  Vector3d c {1,2,3}

  The constructor will have an assertion failure if a fixed size vector or
  matrix given a initializer list of the wrong size.

 **/
EIGEN_STRONG_INLINE Matrix(std::initializer_list<double > initlist) : Base()
{
    Base::_check_template_params();
    int size = initlist.size();

    if (base().size() == 0)
    { //Empty matrix, gotta resize it
        this->resize(size,1 );
    }

    int initializer_list_size = initlist.size();
    int Matrix_size = base().size();
    eigen_assert(initializer_list_size == Matrix_size);

    if ((int)initlist.size() == base().size())
    {
        int i = 0;
        for (auto x: initlist)
        {
            coeffRef(i)=Scalar(x);
            i++;
        }
    }
}

/** Constructs a matrix from a C++11 initializer_list

  MatrixXd a {1,2,3,4} //4x1 matrix
  VectorXd b {6,5,8,6} // 4 element vector
  Vector3d c {1,2,3}

  The constructor will have an assertion failure if a fixed size vector or
  matrix given a initializer list of the wrong size.

 **/
EIGEN_STRONG_INLINE Matrix(std::initializer_list<std::initializer_list<double> > initlist) : Base()
{
    Base::_check_template_params();
    int rows = initlist.size();
    if (rows == 0) return;
    int cols = initlist.begin()->size();

    if (base().size() == 0)
    { //Empty matrix, gotta resize it
        this->resize(rows, cols);
    }

    int initializer_list_size = rows*cols;
    int Matrix_size = base().size();
    eigen_assert(initializer_list_size == Matrix_size);

    if (rows*cols == base().size())
    {
        int i = 0;
        for (auto x: initlist)
        {
            int j=0;
            for (auto y: x)
            {
                coeffRef(rows*j+i)=Scalar(y);
                j++;
            }
            i++;
        }
    }
}

template <int _Rows_Inner, int _Cols_Inner>
EIGEN_STRONG_INLINE Matrix(std::initializer_list<Matrix<_Scalar, _Rows_Inner, _Cols_Inner> > initlist) : Base()
{
    typedef Matrix<_Scalar, _Rows_Inner, _Cols_Inner> MatrixInner;
    Base::_check_template_params();
    static_assert(_Rows_Inner == 1 || _Cols_Inner == 1 || _Rows_Inner == Dynamic || _Cols_Inner == Dynamic, "Eigen: initializer list contains invalid matrices.");

    int rows = initlist.size();
    if (rows == 0)
    {
        int cols = 0;
        if (_Cols_Inner == 1)
        {
            if (_Rows_Inner != Dynamic) cols = _Rows_Inner;
            else cols = 1;
        }
        if (_Rows_Inner == 1)
        {
            if (_Cols_Inner != Dynamic) cols = _Cols_Inner;
            else cols = 1;
        }
        eigen_assert((_Rows == 0 || _Rows == Dynamic) && (_Cols == cols || _Cols == Dynamic));
        this->resize(rows, cols);
        return;
    }
    int mode = -1;//0: init from vectors, 1: init from horizontal vectors
    const MatrixInner *first = initlist.begin();
    if (_Cols_Inner == 1) mode = 0;
    else if (_Rows_Inner == 1) mode = 1;
    else if (_Cols_Inner == Dynamic && first->cols() == 1) mode = 0;
    else if (_Rows_Inner == Dynamic && first->rows() == 1) mode = 1;
    eigen_assert(mode != -1);
    int cols = first->rows();
    if (mode == 1) cols = first->cols();

    if (base().size() == 0) { //Empty matrix, gotta resize it
        this->resize(rows, cols);
    }

    eigen_assert(base().rows() == rows && base().cols() == cols);

    int i = 0;
    for (auto x: initlist)
    {
        base().row(i) = x;
        i++;
    }
}
