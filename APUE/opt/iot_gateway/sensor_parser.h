#ifndef __SENSOR_PARSER_H
#define __SENSOR_PARSER_H

#include <time.h>

typedef struct {
    int device_id;           // 设备ID（从UDP源IP识别）
    int dht_humidity;        // DHT11 湿度数据 
    int dht_temperature;     // DHT11 温度数据 
    float sht_temperature;   // SHT30 温度数据 
    float sht_humidity;      // SHT30 湿度数据
    int adc_value;           // 可调电阻ADC原始值
    int adc_voltage;         // ADC转换后的电压值 
    time_t timestamp;        // 数据接收时间戳
} sensor_data_t;

// 解析STM32发送的UDP数据字符串
// 输入格式: "DHT:65/28 SHT:27.5/60.0 ADC:2048"
int parse_sensor_data(const char *buffer, sensor_data_t *sensor);

// 将传感器数据保存到共享内存
int save_to_shm(const sensor_data_t *sensor);

// 从共享内存读取最新的传感器数据
int read_from_shm(sensor_data_t *sensor);

#endif
