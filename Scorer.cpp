#include "Scorer.h"

Scorer::Scorer(Capture* c)
{
	cap = c;
	sstates[0] = -1;
	sstates[1] = -1;
	sstates[2] = -1;
	sstates[3] = -1;
	sstates[4] = -1;
	sstates[5] = -1;
}

char Scorer::Test(int s1, int s2, int s3, int s4, int s5, int s6)
{
	float* buf = cap->readLast(1000);
	return 0;
}
