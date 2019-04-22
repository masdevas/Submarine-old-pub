#include "../flags_bypass.h"

int main(void)
{
	FlagsBypass flags = flbypInitFlagsBypass(3);
	flbypAddFlag(flags, HAVE_WAY);
	flbypAddFlag(flags, HAVE_WAY);
	flbypAddFlag(flags, HAVE_WAY);
	flbypShowFlags(flags);
	printf("end:%i\n", flags.queue->end);
	flbypChangeLastFlag(flags, HAVE_NOT_WAY);
	flbypShowFlags(flags);
	return 0;
}