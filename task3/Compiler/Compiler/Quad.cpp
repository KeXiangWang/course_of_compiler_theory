#include "pch.h"
#include "Quad.h"

int Quad::count = 0;
int QuadTable::count = 0;
void QuadTable::addQuad(Quad * quad) {
	quads.push_back(quad);
}
