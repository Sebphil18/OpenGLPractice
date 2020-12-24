#pragma once
#include <vector>

class HeightMap {
private:
	std::vector<float> values;
	int lengthX;
	int lengthY;

	void copyValues(int lengthX, int lengthY, std::vector<float>& newValues);

public:
	HeightMap(int lengthX, int lengthY);
	HeightMap(int lengthX, int lengthY, const std::vector<float>& values);

	void normalize();
	void clear();

	void setValue(int x, int y, float value);
	void setValues(int lengthX, int lengthY, const std::vector<float>& values);

	void resize(int lengthX, int lengthY);

	float getValue(int x, int y) const;
	int getLengthX() const;
	int getLengthY() const;

};
