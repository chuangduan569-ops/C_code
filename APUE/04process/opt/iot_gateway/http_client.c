#include "base64.h"
#include "stdio.h"
#include "http_client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int upload_sensor_data(const char *ip, \
	const char *device, const char *sensor, \
		const char *id, double data)
{
	int ret = 0;
	char buf[32] = {0};
	char send_data[512] = {0};
	char len_buf[32] = {0};
	char base_id[256] = {0};
	
	ret = sprintf(buf, "%f", data);
	base64_encode((unsigned char *)base_id, (const unsigned char *)id);
	ret = sprintf(send_data, "POST /api/1.0/device/%s/sensor/%s/data HTTP/1.1\r\nHost: www.embsky.com\r\nAccept: */*\r\nAuthorization: Basic %s\r\nContent-Length: %d\r\nContent-Type: application/json;charset=utf-8\r\nConnection: close\r\n\r\n{\"data\":%f}\r\n", device, sensor, base_id, ret + 10, data);

	// 创建socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	// 设置服务器地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_aton(ip, &addr.sin_addr);
	// 连接
	connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	// 发送
	write(sock, send_data, strlen(send_data));
	// 关闭
	close(sock);

	return 0;
}

int upload_sensor_status(const char *ip, \
	const char *device, const char *sensor, \
		const char *id, char *status)
{
	int ret = 1;
	char send_data[512] = {0};
	char len_buf[32] = {0};
	char base_id[256] = {0};
	
	base64_encode((unsigned char *)base_id, (const unsigned char *)id);
	ret = sprintf(send_data, "POST /api/1.0/device/%s/sensor/%s/data HTTP/1.1\r\nHost: www.embsky.com\r\nAccept: */*\r\nAuthorization: Basic %s\r\nContent-Length: %d\r\nContent-Type: application/json;charset=utf-8\r\nConnection: close\r\n\r\n{\"data\":\"%s\"}\r\n", device, sensor, base_id, ret + 1 + 10, status);
	
	// 创建socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	// 设置服务器地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_aton(ip, &addr.sin_addr);
	// 连接
	connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	// 发送
	write(sock, send_data, strlen(send_data));
	// 关闭
	close(sock);

	return 0;
}

int upload_device_datas(const char *ip, \
	const char *device, const char *id, \
		int sensor_id_arrary[], double sensor_data_arrary[], int size)
{
	int i = 0;
	int ret = 0;
	char send_data[2048] = {0};
	char data_list[512] = {0};
	char len_buf[32] = {0};
	char base_id[256] = {0};
	
	ret = sprintf(data_list, "%s", "[");
	
	for(i = 0; i < size; i++)
	{
		if(i == size - 1)
		{
			ret += sprintf(data_list + ret, "{\"id\":%d, \"data\":%f}", sensor_id_arrary[i], sensor_data_arrary[i]); 
		}
		else
		{
			ret += sprintf(data_list + ret, "{\"id\":%d, \"data\":%f},", sensor_id_arrary[i], sensor_data_arrary[i]);
		}
	}
	ret += sprintf(data_list + ret, "%s", "]");

	base64_encode((unsigned char *)base_id, (const unsigned char *)id);
	ret = snprintf(send_data,sizeof(send_data), "POST /api/1.0/device/%s/datas HTTP/1.1\r\nHost: www.embsky.com\r\nAccept: */*\r\nAuthorization: Basic %s\r\nContent-Length: %d\r\nContent-Type: application/json;charset=utf-8\r\nConnection: close\r\n\r\n{\"datas\":%s}\r\n", device, base_id, ret + 10, data_list);
	
	// 创建socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	// 设置服务器地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_aton(ip, &addr.sin_addr);
	// 连接
	connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	// 发送
	write(sock, send_data, strlen(send_data));
	// 关闭
	close(sock);
	return 0;
}

int download_sensor_status(const char *ip, \
	const char *device, const char *sensor, \
		const char *id)
{
	int ret = 0;
	char send_data[512] = {0};
	char len_buf[32] = {0};
	char base_id[256] = {0};

	base64_encode((unsigned char *)base_id, (const unsigned char *)id);
	ret = sprintf(send_data, "GET /api/1.0/device/%s/sensor/%s/data HTTP/1.1\r\nHost: www.embsky.com\r\nAccept: */*\r\nAuthorization: Basic %s\r\nContent-Length: 0\r\nConnection: close\r\n\r\n", device, sensor, base_id);

	// 创建socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	// 设置服务器地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_aton(ip, &addr.sin_addr);
	// 连接
	connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	// 发送
	write(sock, send_data, strlen(send_data));
	// 关闭
	close(sock);

	return 0;
}


