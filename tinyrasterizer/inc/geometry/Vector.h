#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>
#include <iostream>
#include <algorithm>
#include <array>

namespace core {
	template<bool Cond, typename T = void> struct enable_if {};
	template<typename T> struct enable_if<true, T> { using type = T; };

	template<typename _ElemType>
	struct binary_op {
		static _ElemType op_add(_ElemType lhs, _ElemType rhs) { return lhs + rhs; }
		static _ElemType op_decrease(_ElemType lhs, _ElemType rhs) { return lhs - rhs; }
		template<typename _MulType>
		static _ElemType op_multiple(_ElemType lhs, _MulType multiplier) { return lhs * multiplier; }
		template<typename _DivType>
		static _ElemType op_divide(_ElemType lhs, _DivType divisor) { return lhs / divisor; }
	};

	template<typename T, std::size_t N>
	class Vector
	{
	public:
		using ArrayType = std::array<T, N>;
		// ctors
		Vector() :_pdata(std::make_unique<ArrayType>(ArrayType({}))) { }
		Vector(ArrayType arr) : _pdata(std::make_unique<ArrayType>(arr)) { }
		Vector(ArrayType&& arr) : _pdata(std::make_unique<ArrayType>(std::move(arr))) { }
		explicit Vector(std::initializer_list<T> il) : _pdata(std::make_unique<ArrayType>(ArrayType()))
		{
			std::size_t i = 0;
			for (auto iter = il.begin(); iter != il.end(); ++iter)
				(*_pdata)[i++] = *iter;
		}

		// 注意Rule of Three
		// copy ctor
		Vector(const Vector& rhs) : _pdata(std::make_unique<ArrayType>(ArrayType()))
		{
			for (auto i = 0; i < N; i++)
				(*_pdata)[i] = (*rhs._pdata)[i];
		}
		// move ctor
		Vector(Vector&& rhs) noexcept : _pdata(std::move(rhs._pdata)) { }
		//dctor
		~Vector() noexcept = default;
		//copy assignment
		Vector& operator=(const Vector& rhs) noexcept
		{
			for (auto i = 0; i < N; i++)
				(*_pdata)[i] = (*rhs._pdata)[i];
			return *this;
		}

		Vector& operator=(Vector&& rhs) noexcept
		{
			_pdata = std::move(rhs._pdata);
			return *this;
		}

		// Get 
		template<typename = enable_if<N >= 2>::type>
		T x() const noexcept { return (*_pdata)[0]; }
		template<typename = enable_if<N >= 2>::type>
		T u() const noexcept { return (*_pdata)[0]; }
		template<typename = enable_if<N >= 2>::type>
		T y() const noexcept { return (*_pdata)[1]; }
		template<typename = enable_if<N >= 2>::type>
		T v() const noexcept { return (*_pdata)[1]; }

		template<typename = enable_if<N >= 3>::type>
		T z() const noexcept { return (*_pdata)[2]; }
		template<typename = enable_if<N >= 4>::type>
		T w() const noexcept { return (*_pdata)[3]; }


		// Set
		template<typename = enable_if<N >= 2>::type>
		void x(T new_x) noexcept { (*_pdata)[0] = new_x; }
		template<typename = enable_if<N >= 2>::type>
		void u(T new_u) noexcept { (*_pdata)[0] = new_u; }
		template<typename = enable_if<N >= 2>::type>
		void y(T new_y) noexcept { (*_pdata)[1] = new_y; }
		template<typename = enable_if<N >= 2>::type>
		void v(T new_v) noexcept { (*_pdata)[1] = new_v; }
		template<typename = enable_if<N >= 3>::type>
		void z(T new_z) noexcept { (*_pdata)[2] = new_z; }
		template<typename = enable_if<N >= 3>::type>
		void w(T new_w) noexcept { (*_pdata)[3] = new_w; }

		T& operator[](size_t i) // 可被当成左值对返回元素进行修改
		{
			assert(i < N);
			return (*_pdata)[i];
		}

		T operator[](size_t i) const //不可被当左值，需要传副本，实例化后的const Vector会调用此函数
		{
			assert(i < N);
			return (*_pdata)[i];
		}

		Vector& operator+=(const Vector& rhs)
		{
			for (std::size_t i = 0; i < N; i++)
				(*_pdata)[i] += (*rhs._pdata)[i];
			return *this;
		}

		Vector operator+(const Vector& rhs) const { return Vector(rhs) += *this; }

		Vector& operator-=(const Vector& rhs)
		{
			for (std::size_t i = 0; i < N; i++)
				(*_pdata)[i] -= (*rhs._pdata)[i];
			return *this;
		}

		Vector operator-(const Vector& rhs) const { return Vector(rhs) -= *this; }


		template<typename = enable_if<N == 2>::type>
		T operator*(const Vector& rhs) const noexcept { return x() * rhs.y() - y() * rhs.x(); }

		template<typename = enable_if<N == 3>::type>
		Vector& operator*=(const Vector& rhs) noexcept
		{
			(*_pdata)[0] = (*_pdata)[1] * (*rhs._pdata)[2] - (*_pdata)[2] * (*rhs._pdata)[1];
			(*_pdata)[1] = (*_pdata)[2] * (*rhs._pdata)[0] - (*_pdata)[0] * (*rhs._pdata)[2];
			(*_pdata)[2] = (*_pdata)[0] * (*rhs._pdata)[1] - (*_pdata)[1] * (*rhs._pdata)[0];
			return *this;
		}

		template<typename = enable_if<N == 3>::type>
		Vector operator*(const Vector& rhs) const
		{
			return Vector({ (*_pdata)[1] * (*rhs._pdata)[2] - (*_pdata)[2] * (*rhs._pdata)[1],
							(*_pdata)[2] * (*rhs._pdata)[0] - (*_pdata)[0] * (*rhs._pdata)[2],
							(*_pdata)[0] * (*rhs._pdata)[1] - (*_pdata)[1] * (*rhs._pdata)[0]
				});
		}

		template<typename _Mul>
		Vector& operator*=(_Mul n)
		{
			for (std::size_t i = 0; i < N; i++)
				(*_pdata)[i] *= n;
			return *this;
		}

		template<typename _Mul>
		Vector operator*(_Mul n) const { return Vector(*this) * n; }

		//简单算法
		T magnitude()
		{
			T ret = 0;
			for (std::size_t i = 0; i < N; i++)
				ret += (*_pdata)[i] * (*_pdata)[i];
			return std::sqrt(ret);
		}

		Vector& normalize()
		{
			T factor = 1.f / magnitude();
			for (std::size_t i = 0; i < N; i++)
				(*_pdata)[i] *= factor;
			return *this;
		}

		//使用临时变量的引用，C++ Standard规定必须是const，否则编译错误
		template<typename T, std::size_t N> //必须加上模板声明，否则会链接错误
		friend std::ostream& operator<<(std::ostream& os, const Vector<T, N>& v);

	private:
		//std::array<T, N> _data;
		std::unique_ptr<std::array<T, N>> _pdata;
	};

	template<typename T, std::size_t N>
	inline std::ostream& operator<<(std::ostream& os, const Vector<T, N>& v)
	{
		if (v._pdata.get()) {
			os << "(";
			std::copy((*v._pdata).begin(), (*v._pdata).end(), std::ostream_iterator<T>(os, ", "));
			os << ")";
		}
		else {
			os << "(null)";
		}
		return os;
	}

	typedef Vector<float, 2> Vector2f;
	typedef Vector<float, 3> Vector3f;
	typedef Vector<float, 4> Vector4f;
}

#endif //__VECTOR_H__