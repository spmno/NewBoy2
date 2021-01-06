
typedef struct _gps_info {
	float longitude;
	float latitude;
} gps_info;

void save_gps_info(int longitude, int latitude);
gps_info* get_gps_info(void); 

