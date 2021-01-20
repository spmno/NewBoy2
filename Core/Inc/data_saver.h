
typedef struct _gps_info {
	float longitude;
	float latitude;
} gps_info;

void save_gps_info(int longitude, int latitude);
gps_info* get_gps_info(void); 

void save_version_string(const char* version);
char* get_version_string(void);

