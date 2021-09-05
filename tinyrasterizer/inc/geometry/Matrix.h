#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Vector.h"

namespace core {
	//N cols, M rows
	template<typename T, std::size_t N, size_t M>
	class Matrix {
	public:
		using ArrayType = std::array<T, N* M>;
		Matrix() :_pdata(std::make_unique<ArrayType>(ArrayType({}))), _index(0) { }
		explicit Matrix(std::initializer_list<T> il) : _pdata(std::make_unique<ArrayType>(ArrayType())), _index(0)
		{
			std::size_t i = 0;
			for (auto iter = il.begin(); iter != il.end(); ++iter)
				(*_pdata)[i++] = *iter;
		}

		//注意Rule of Three
		// copy ctor
		Matrix(const Matrix& rhs) :_pdata(std::make_unique<ArrayType>(ArrayType())), _index(rhs._index)
		{
			//_pdata = std::make_unique<std::array<T, N>>(std::array<T, N>());
			//_pdata = std::make_unique<std::array<T, N * M>>(std::array<T, N * M>({}));
			for (auto i = 0; i < N * M; i++)
				(*_pdata)[i] = (*rhs._pdata)[i];
		}

		// move ctor
		Matrix(Matrix&& rhs) noexcept : _pdata(std::move(rhs._pdata)) {}

		//dctor
		~Matrix() noexcept
		{
			_pdata.reset(nullptr);
		}

		//copy assignment
		Matrix& operator=(const Matrix& rhs) noexcept
		{
			for (auto i = 0; i < N * M; i++)
				(*_pdata)[i] = (*rhs._pdata)[i];
			return *this;
		}

		Matrix& operator=(Matrix&& rhs) noexcept
		{
			_pdata = std::move(rhs._pdata);
			_index = rhs._index;
			return *this;
		}

		// Get
		Vector<T, M> row(std::size_t i) const noexcept { assert(i < N); return Vector<T, M>((*_prows)[i]); }
		T operator()(std::size_t i) const noexcept { assert(i <= M * N); return (*_pdata)[i]; }
		T& operator()(std::size_t i) noexcept { assert(i <= M * N); return (*_pdata)[i]; }
		T operator()(std::size_t i, std::size_t j) const noexcept { assert(i * M + j <= M * N); return (*_pdata)[i * M + j]; }
		T& operator()(std::size_t i, std::size_t j) noexcept { assert(i * M + j <= M * N); return (*_pdata)[i * M + j]; }

		//operator
		//Vector<T, M>& operator[](std::size_t i) { assert(i < N);  return (*_prows)[i]; } //无法支持的操作
		Vector<T, M> operator[](size_t i) const { assert(i < N); return (*_prows)[i]; }

		Matrix& operator+=(const Matrix& rhs)
		{
			for (std::size_t i = 0; i < N * M; i++)
				(*_pdata)[i] += (*rhs._pdata)[i];
			return *this;
		}
		Matrix operator+(const Matrix& rhs) const {
			return Matrix(rhs)
				+= *this;
		}

		Matrix& operator-=(const Matrix& rhs)
		{
			for (std::size_t i = 0; i < N * M; i++)
				(*_pdata)[i] -= (*rhs._pdata)[i];
			return *this;
		}
		Matrix operator-(const Matrix& rhs) const { return Matrix(rhs) -= *this; }

		template<typename _Mul>
		Matrix& operator*=(_Mul n)
		{
			for (std::size_t i = 0; i < N * M; i++)
				(*_pdata)[i] *= n;
			return *this;
		}

		template<typename _Mul>
		Matrix operator*(_Mul num) const { return Matrix(*this) *= num; }

		template<typename T, std::size_t N, std::size_t M>
		friend Matrix<T, N, N> operator*(const Matrix<T, N, M>& lhs, const Matrix<T, M, N>& rhs);

		//用于输入
		Matrix& operator<<(const T& val);
		Matrix& operator,(const T& val);

		//简单算法
		template<typename = enable_if<N == M>::type>
		Matrix& transpose()
		{
			for (std::size_t i = 0; i < N; i++) {
				for (std::size_t j = i + 1; j < M; j++) {
					std::swap((*_pdata)[i * M + j], (*_pdata)[j * M + i]);
				}
			}
			return *this;
		}

		//M cols, N rows
		Matrix<T, M, N> transpose_matrix()
		{
			Matrix<T, M, N> ret;
			for (std::size_t i = 0; i < N; i++) {
				for (std::size_t j = 0; j < M; j++) {
					//因为Matrix<T, M, N>与Matrix<T, N, M>可能不属于一个类，故可能无法访问ret._pdata
					ret(j, i) = (*_pdata)[i * M + j];
				}
			}
			return ret;
		}

		template<typename = enable_if<N == M>::type>
		static constexpr Matrix Identity()
		{
			Matrix<T, M, N> ret;
			for (std::size_t i = 0; i < N; i++)
				(*ret._pdata)[i * N + i] = 1;
			return ret;
		}

		template<typename T, std::size_t N, std::size_t M>
		friend std::ostream& operator<<(std::ostream& os, const Matrix<T, N, M >& m);

	private:
		//proxy class
		union {
			std::unique_ptr<std::array<T, M* N>> _pdata;
			std::unique_ptr<std::array<std::array<T, M>, N>> _prows;
		};
		std::size_t _index;
	};

	template<typename T, std::size_t N, std::size_t M>
	Matrix<T, N, N> operator*(const Matrix<T, N, M>& lhs, const Matrix<T, M, N>& rhs)
	{
		Matrix<T, N, N> ret;
		//Strassen Optimization
		for (std::size_t i = 0; i < N; i++) {
			for (std::size_t k = 0; k < M; k++) {
				auto tmp = (*lhs._pdata)[i * M + k];
				for (std::size_t j = 0; j < N; j++) {
					(*ret._pdata)[i * N + j] += tmp * (*rhs._pdata)[k * N + j];
				}
			}
		}
		return ret;
	}

	template<typename T, std::size_t N, std::size_t M>
	std::ostream& operator<<(std::ostream& os, const Matrix<T, N, M>& m)
	{
		if (m._pdata.get()) {
			for (std::size_t i = 0; i < N; i++) {
				os << "(";
				for (std::size_t j = 0; j < M; j++) {
					os << (*m._pdata)[i * M + j] << ", ";
				}
				os << ")";
				os << std::endl;
			}
		}
		else {
			os << "(null)";
		}
		return os;
	}

	template<typename T, std::size_t N, std::size_t M>
	inline Matrix<T, N, M>& Matrix<T, N, M>::operator<<(const T& val)
	{
		(*_pdata)[_index] = val;
		return *this;
	}

	template<typename T, std::size_t N, std::size_t M>
	inline Matrix<T, N, M>& Matrix<T, N, M>::operator,(const T& val)
	{
		(*_pdata)[++_index] = val;
		if (_index == N * M) _index = 0;
		return *this;
	}

	typedef Matrix<float, 3, 3> Matrix3f;
	typedef Matrix<float, 4, 4> Matrix4f;
}
#endif // __MATRIX_H__