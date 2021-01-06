#include "data_saver.h"
#include "stdio.h"

static gps_info current_gps_info;

void save_gps_info(int longitude, int latitude)
{
	printf("gps: %d, %d\n", longitude, latitude);
	current_gps_info.longitude = longitude*1.0/10000000;
	current_gps_info.latitude = latitude*1.0/10000000;
}

gps_info* get_gps_info(void) 
{
	return &current_gps_info;
}

