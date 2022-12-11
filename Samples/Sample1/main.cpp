#include <MemManager.h>
void Fun()
{
	char * k = NOELLE_NEW char[10];
}
void main()
{
	int * a = NOELLE_NEW int;
	*a = 5;
	Fun();
}