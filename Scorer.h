#ifndef SCORER
#define SCORER

#include "Capture.h"

class Scorer
{
public:
	Scorer(Capture* c);

	char Test(int s1, int s2, int s3, int s4, int s5, int s6);
private:
	Capture* cap;
	int sstates[6];
};

#endif
