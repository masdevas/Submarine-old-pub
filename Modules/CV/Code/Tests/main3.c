#include "Code/Headers/gen_cvdata.h"

CVData genCVData(char *phName);

FILE *logFile;

int main(void)
{
	CVData data;
	printf(":::\n");
	logFile = fopen("log.txt", "w");
	char phName[] = "Data/real_tst1.png";
	data = genCVData(phName);
	printf("%i %i\n", data.sx, data.sy);
	return 0;
}
