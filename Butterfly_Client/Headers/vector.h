#pragma once

struct VECTOR2
{
	float x;
	float y;
public:
	float Length() {
		return sqrt(x * x + y * y);
	}
	VECTOR2& operator=(POINT _pt) {
		x = (float)_pt.x;
		y = (float)_pt.y;
	}
	VECTOR2& Normalize() {
		float fLen = Length();

		assert(fLen != 0.f);

		x /= fLen;
		y /= fLen;

		return *this;
	}
	VECTOR2 operator +(VECTOR2 _vOther) {
		return VECTOR2(x + _vOther.x, y + _vOther.y);
	}
	void operator +=(VECTOR2 _vOther) {
		x += _vOther.x;
		y += _vOther.y;
	}
	VECTOR2 operator -(VECTOR2 _vOther) {
		return VECTOR2(x - _vOther.x, y - _vOther.y);
	}
	VECTOR2 operator /(VECTOR2 _vOther) {
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));
		return VECTOR2(x / _vOther.x, y / _vOther.y);
	}
	VECTOR2 operator /(float _f) {
		assert(!(0.f == _f));
		return VECTOR2(x / _f, y / _f);
	}
	VECTOR2 operator *(VECTOR2 _vOther) {
		return VECTOR2(x * _vOther.x, y * _vOther.y);
	}
	VECTOR2 operator *(int _i) {
		return VECTOR2(x * (float)_i, y * (float)_i);
	}
	VECTOR2 operator *(float _f) {
		return VECTOR2(x * _f, y * _f);
	}
	VECTOR2() : x(0.f), y(0.f) {}
	VECTOR2(float _x, float _y) : x(_x), y(_y) {}
	VECTOR2(int _x, int _y) : x((float)_x), y((float)_y) {}
	VECTOR2(const POINT _pt) : x((float)_pt.x), y((float)_pt.y) {}
};