#include "Block.h"

void Block::SetBlock(float z, float width) {
	zPos = z;
	Width = width;
}

void Block::MoveBlock() {
	zPos += V;
	if (zPos > 2) {
		zPos = -5;
	}
}

bool Block::isCoverd(float tolerance)
{
	//bool f = (zPos < tolerance + Width * 0.5f) && (zPos > -tolerance - Width * 0.5f);
	//std::cout << "Z:" << zPos << "  width:" << Width <<"  pd:"<< f << std::endl;
	return (zPos < tolerance + Width * 0.25f) && (zPos > -tolerance - Width * 0.25f);
}