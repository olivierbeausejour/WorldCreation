#pragma once

#include "wcpch.h"

constexpr float FLOAT_DIFFERENCE_LIMIT = 0.0000001f;

class MathHelper
{
public:
	static int closestToZero(const int &v1, const int &v2)
	{
		return abs(v1) <= abs(v2) ? v1 : v2;
	}

	static int closestToZero(const float &v1, const float &v2)
	{
		return abs(v1) <= abs(v2) ? v1 : v2;
	}

	static int isSameSignValue(const int &v1, const int &v2)
	{
		return (v1 >= 0 && v2 >= 0) || (v1 <= 0 && v2 <= 0);
	}

	static int isSameSignValue(const float &v1, const float &v2)
	{
		return (v1 >= 0 && v2 >= 0) || (v1 <= 0 && v2 <= 0);
	}

	static int getSameSignValue(const int &v1, const int &v2)
	{
		return isSameSignValue(v1, v2) ? v2 : -v2;
	}

	static int getSameSignValue(const float &v1, const float &v2)
	{
		return isSameSignValue(v1, v2) ? v2 : -v2;
	}

	// Conversion formulas taken from https://en.wikipedia.org/wiki/List_of_common_coordinate_transformations#To_cylindrical_coordinates
	static glm::vec3 cartesianToCylindrical(const glm::vec3 &cartesianCoords)
	{
		float distance = sqrt(cartesianCoords.x * cartesianCoords.x + cartesianCoords.z * cartesianCoords.z);
		float inclination;

		if (cartesianCoords.x == 0 && cartesianCoords.z == 0)
			inclination = 0.0f;
		else if (cartesianCoords.z >= 0)
			inclination = ofRadToDeg(asin(cartesianCoords.x / distance));
		else if (cartesianCoords.z > 0)
			inclination = ofRadToDeg(atan(cartesianCoords.x / cartesianCoords.z));
		else if (cartesianCoords.z < 0)
			inclination = ofRadToDeg(-asin(cartesianCoords.x / distance) + PI);

		if (inclination < 0)
			inclination += 360;

		float height = cartesianCoords.y;

		return glm::vec3(distance, height, inclination);
	}

	// Conversion formulas taken from https://en.wikipedia.org/wiki/List_of_common_coordinate_transformations#From_cylindrical_coordinates
	static glm::vec3 cylindricalToCartesian(const glm::vec3 &cylindricalCoords)
	{
		float x = cylindricalCoords.x * sin(ofDegToRad(cylindricalCoords.z));
		float y = cylindricalCoords.y;
		float z = cylindricalCoords.x * cos(ofDegToRad(cylindricalCoords.z));

		return glm::vec3(x, y, z);
	}

	static bool isFloatEqual(float x, float y)
	{
		return abs(x - y) <= FLOAT_DIFFERENCE_LIMIT;
	}

	static int factorial(int n)
	{
		if (n == 0 || n == 1)
		{
			return 1;
		}
		return n * factorial(n - 1);
	}

	static float bernstein(int v, int n, float x)
	{
		return (factorial(n) / (factorial(v) * factorial(n - v))) * pow(x, v) * pow(1 - x, n - v);
	}
};