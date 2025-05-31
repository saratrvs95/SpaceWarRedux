#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

// Famous Quake 3 inverse square root, shamelessly copied from good old Wikipedia
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
inline float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

namespace sf
{
    #define PI std::acosf(-1)

    /**
     * Angle Conversions
     */
    template <typename T>
    inline float radToDeg(T radians)
    {
        return static_cast<float>(radians * 180.0f / PI);
    }

    template <typename T>
    inline float degToRad(T degree)
    {
        return static_cast<float>(degree / 180.0f * PI);
    }

    template <typename T>
    T Length(const Vector2<T>& vec)
    {
        return sqrt((vec.x * vec.x) + (vec.y * vec.y));
    }

	template <typename T>
	T SquaredLength(const Vector2<T>& vec)
	{
		return (vec.x * vec.x) + (vec.y * vec.y);
	}

    // Realistically, the normalize functions should only be called on vector2f
    // but if you can, if you so choose, call it on vector2i and vector2u
    // The functions will probably return (0,0), but that's what you asked for

    // Safe normalize that returns a copy of the vector
	template <typename T>
    Vector2<T> SafeNormalize(const Vector2<T>& vec)
	{
        T sqrLen = SquaredLength(vec);
        if (sqrLen <= 0)
            return Vector2<T>(0,0);

        float invsqrt = Q_rsqrt(static_cast<float>(sqrLen));

		return vec * static_cast<T>(invsqrt);
	}

	// In-place normalize that modifies the incoming vector
	template <typename T>
    Vector2<T>& Normalize(Vector2<T>& vec)
	{
		T sqrLen = SquaredLength(vec);
        if (sqrLen > 0)
        {
            float invsqrt = Q_rsqrt(static_cast<float>(sqrLen));
            vec *= static_cast<T>(invsqrt);
        }
        return vec;
	}

	template <typename T>
	T SquaredDistance(const Vector2<T>& vec1, const Vector2<T>& vec2)
	{
        Vector2<T> tempVec = vec2 - vec1;
        return SquaredLength(tempVec);
	}

    template <typename T>
    T Distance(const Vector2<T>& vec1, const Vector2<T>& vec2)
    {
        Vector2<T> tempVec = vec2 - vec1;
        return Length(tempVec);
    }

	template <typename T>
	Vector2<T> RotationToUnitVector(T rotation)
	{
        return sf::Vector2<T>(std::sinf(sf::degToRad(rotation)), -std::cosf(sf::degToRad(rotation)));
	}

    template <typename T>
    float DotProduct(const Vector2<T>& vec1, const Vector2<T>& vec2)
    {
        return (vec1.x * vec2.x) + (vec1.y * vec2.y);
    }
}

template<typename T>
requires std::is_scalar<T>::value
T RandomInRange(T low, T high)
{
    if (low > high) return RandomInRange<T>(high, low);

    if (high == low) return low;

    float randomFactor = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    return static_cast<T>(((high - low + 1) * randomFactor) + low);
}