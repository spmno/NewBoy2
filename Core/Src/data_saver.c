#include "data_saver.h"
#include "stdio.h"
#include "string.h"

static gps_info current_gps_info = {
	123.2859, 41.4165
};

char version_string[10];
char download_file_url[128];

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

void save_version_string(const char* version)
{
	strcpy(version_string, version);
}

char* get_version_string()
{
	return version_string;
}

char* get_download_file_url()
{
	return download_file_url;
}



