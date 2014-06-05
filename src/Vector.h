#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <cstdint>
#include <chrono>
#include <random>

template <typename T, uint32_t D> class Vector
{
public:
    template <typename... Args> Vector(const Args&... args)
    {
        if (sizeof...(args) == 0)
            m_elems.resize(D);
        else
        {
            m_elems.reserve(D);
            VectorInit(args...);
        }
    }

    Vector(const Vector<T, D>& vec) : m_elems(vec.m_elems) {}

    Vector<T, D>& operator =(const Vector<T, D>& vec)
    {
        m_elems = vec.m_elems;
        return *this;
    }

    ~Vector() {}

    const T& operator[](uint32_t index) const
    {
        if (index >= D)
            throw 1;

        return m_elems[index];
    }

	bool operator==(const Vector<T, D>& vec)
	{
		if (m_elems.size() != vec.m_elems.size())
			return false;

		for (uint32_t i = 0; i < vec.m_elems.size(); ++i)
			if (m_elems[i] != vec.m_elems[i])
				return false;

		return true;
	}

    Vector<T, D> operator-() const
    {
        Vector<T,D> vec(*this);
        for (T& elem : vec.m_elems)
            elem = -elem;

        return vec;
    }

	Vector<T, D> operator+(const Vector<T, D>& vec) const
	{
		Vector<T, D> result(*this);
		for (uint32_t i = 0; i < vec.m_elems.size(); ++i)
			result.m_elems[i] += vec.m_elems[i];

		return result;
	}

	Vector<T, D> operator-(const Vector<T, D>& vec) const
	{
		Vector<T, D> result(*this);
		for (uint32_t i = 0; i < vec.m_elems.size(); ++i)
			result.m_elems[i] -=  vec.m_elems[i];

		return result;
	}

	Vector<T, D> operator*(double mod) const
	{
		Vector<T, D> result(*this);
		for (T& elem : result.m_elems)
			elem = static_cast<T>(elem * mod);

		return result;
	}

	double Abs()
	{
		double sum = 0.0;
		for (T& elem : m_elems)
			sum += elem * elem;

		return std::sqrt(sum);
	}

	double DistanceFrom(const Vector<T, D>& point) const
	{
		return (*this - point).Abs();
	}

	static double DistanceBetween(const Vector<T, D>& pointA, const Vector<T, D>& pointB)
	{
		return pointA.DistanceFrom(pointB);
	}

    static Vector<T, D> Random(const Vector<T,D>& lowBound, const Vector<T,D>& highBound)
    {
        static std::mt19937 randGen(static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count()));
		Vector<T, D> vec;

        for (uint32_t i = 0; i < vec.m_elems.size(); ++i)
            vec.m_elems[i] = (T)((randGen() % (uint32_t)(highBound[i] * 1000 - lowBound[i] * 1000)) / (T)1000 + lowBound[i]);

        return vec;
    }

    friend std::ostream& operator<<(std::ostream& ostream, const Vector<T,D>& vec)
    {
        ostream << "(";
        for (uint32_t i = 0; i < vec.m_elems.size() - 1; ++i)
            ostream << vec.m_elems[i] << ",";
        ostream << vec.m_elems.back() << ")";
        return ostream;
    }

private:
    void VectorInit() {}
    template <typename... Args> void VectorInit(const T& first, const Args&... args)
    {
        if (m_elems.size() > D)
            throw 1;

        m_elems.push_back(first);
        VectorInit(args...);
    }

    std::vector<T> m_elems;
};

typedef Vector<int32_t, 2>      Vector2i;
typedef Vector<uint32_t, 2>     Vector2u;
typedef Vector<float, 2>		Vector2f;

#endif
