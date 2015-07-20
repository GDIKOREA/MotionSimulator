#pragma once


namespace common
{

	//-----------------------------------------------------------------------------
	// Name: getRandomMinMax()
	// Desc: Gets a random number between min/max boundaries
	// http://www.codesampler.com/dx9src/dx9src_7.htm
	//-----------------------------------------------------------------------------
	inline float GetRandomMinMax( float fMin, float fMax )
	{
		float fRandNum = (float)rand () / RAND_MAX;
		return fMin + (fMax - fMin) * fRandNum;
	}

	//-----------------------------------------------------------------------------
	// Name: getRandomVector()
	// Desc: Generates a random vector where X,Y, and Z components are between
	//       -1.0 and 1.0
	// http://www.codesampler.com/dx9src/dx9src_7.htm
	//-----------------------------------------------------------------------------
	inline Vector3 GetRandomVector( void )
	{
		Vector3 vVector;

		// Pick a random Z between -1.0f and 1.0f.
		vVector.z = GetRandomMinMax( -1.0f, 1.0f );

		// Get radius of this circle
		float radius = (float)sqrt(1 - vVector.z * vVector.z);

		// Pick a random point on a circle.
		float t = GetRandomMinMax( -MATH_PI, MATH_PI);

		// Compute matching X and Y for our Z.
		vVector.x = (float)cosf(t) * radius;
		vVector.y = (float)sinf(t) * radius;

		return vVector;
	}


	// 보간.
	inline void lerp(OUT Vector3 &out, const Vector3 &a, const Vector3 &b, float t)
	{
		out.x = a.x + (b.x - a.x) * t;
		out.y = a.y + (b.y - a.y) * t;
		out.z = a.z + (b.z - a.z) * t;
	}
	inline void lerp(OUT Vector2 &out, const Vector2 &a, const Vector2 &b, float t)
	{
		out.x = a.x + (b.x - a.x) * t;
		out.y = a.y + (b.y - a.y) * t;
	}


	// 베지어 곡선을 리턴한다.
	// point[ 0 ~ 3]
	// t : 0 ~ 1
	inline void bezier(OUT Vector3 &out, const vector<Vector3> &points, const float t)
	{
		Vector3 ab, bc, cd, abbc, bccd;
		common::lerp(ab, points[0], points[1], t);
		common::lerp(bc, points[1], points[2], t);
		common::lerp(cd, points[2], points[3], t);
		common::lerp(abbc, ab, bc, t);
		common::lerp(bccd, bc, cd, t);
		common::lerp(out, abbc, bccd, t);
	}
	inline void bezier(OUT Vector2 &out, const vector<Vector2> &points, const float t)
	{
		Vector2 ab, bc, cd, abbc, bccd;
		common::lerp(ab, points[0], points[1], t);
		common::lerp(bc, points[1], points[2], t);
		common::lerp(cd, points[2], points[3], t);
		common::lerp(abbc, ab, bc, t);
		common::lerp(bccd, bc, cd, t);
		common::lerp(out, abbc, bccd, t);
	}


}
