#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H
// 云平台配置常量
#define API_KEY "eyJpYXQiOjE3NzMyODA5NjEsImV4cCI6MjA4ODY0MDk2MSwiYWxnIjoiSFMyNTYifQ.eyJpZCI6NTc0N30.PDYqVpgVgxCPXhOHfLZTo5AUEcKpjlHTXOtjDbrDTCM:"
#define CLOUD_IP "119.29.98.16"
#define CLOUD_PORT 80
#define DEVICE_ID "5861"
#define SENSOR_TEMP "16551"
#define SENSOR_HUMI "16557"
#define SENSOR_SWITCH "16547"

extern int upload_sensor_data(const char *ip, const char *device, const char *sensor, const char *id, double data);
extern int download_sensor_status(const char *ip, const char *device, const char *sensor, const char *id);
extern int upload_sensor_status(const char *ip, const char *device, const char *sensor, const char *id, char *status);
extern int upload_device_datas(const char *ip, const char *device, const char *id, int sensor_id_arrary[], double sensor_data_arrary[], int size);

#endif

