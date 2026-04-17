#include "iot_gateway.h"
#include "sensor_parser.h"
#include "log.h"
#include "ipc_modules.h"

int parse_sensor_data(const char *buffer, sensor_data_t *sensor)
{
    memset(sensor, 0, sizeof(sensor_data_t));
    
    // 解析格式: "DHT:65/28 SHT:27.5/60.0 ADC:2048"
    if(sscanf(buffer, "DHT:%d/%d SHT:%f/%f ADC:%d",
              &sensor->dht_humidity,
              &sensor->dht_temperature,
              &sensor->sht_temperature,
              &sensor->sht_humidity,
              &sensor->adc_value) >= 5)
    {
        // 将ADC原始值(0-4095)转换为电压值(毫伏)
        sensor->adc_voltage = sensor->adc_value * 3300 / 4095;
        sensor->timestamp = time(NULL);
        
        LOG_INFO("解析成功: DHT=%d/%d, SHT=%.1f/%.1f, ADC=%d (电压=%dmV)",
                  sensor->dht_humidity, sensor->dht_temperature,
                  sensor->sht_temperature, sensor->sht_humidity,
                  sensor->adc_value, sensor->adc_voltage);
        
        return 0;
    }
    // ========== 新增格式2: 仅DHT数据 "DHT:65/28" ==========
    if(sscanf(buffer, "DHT:%d/%d",
              &sensor->dht_humidity,
              &sensor->dht_temperature) == 2)
    {
        // 使用DHT数据作为温度湿度
        sensor->sht_temperature = (float)sensor->dht_temperature;
        sensor->sht_humidity = (float)sensor->dht_humidity;
        sensor->adc_value = 0;
        sensor->adc_voltage = 0;
        sensor->timestamp = time(NULL);
        LOG_INFO("解析DHT格式成功: 湿度=%d%%, 温度=%d°C",
                 sensor->dht_humidity, sensor->dht_temperature);
        return 0;
    }
    
    LOG_WARN("解析失败: %s", buffer);
    return -1;
}

int save_to_shm(const sensor_data_t *sensor)
{
    int shmid;
    shm_buffer_t *shm;
    int write_pos;

    shmid = shmget(IPC_KEY_SHM, 0, 0666);
    if(shmid < 0) {
        LOG_ERROR("save_to_shm: 获取共享内存失败");
        return -1;
    }
    
    shm = (shm_buffer_t *)shmat(shmid, NULL, 0);
    if(shm == (void *)-1) {
        LOG_ERROR("save_to_shm: 附加共享内存失败");
        return -1;
    }
    
    write_pos = shm->write_pos;
    if(write_pos + sizeof(sensor_data_t) > SHM_SIZE) {
        write_pos = 0;
        shm->write_pos = 0;
    }
    
    memcpy(shm->data + write_pos, sensor, sizeof(sensor_data_t));
    shm->write_pos = write_pos + sizeof(sensor_data_t);
    
    shmdt(shm);
    return 0;
}

int read_from_shm(sensor_data_t *sensor)
{
    int shmid;
    shm_buffer_t *shm;
    int read_pos;

    shmid = shmget(IPC_KEY_SHM, 0, 0666);
    if(shmid < 0) {
        LOG_ERROR("read_from_shm: 获取共享内存失败");
        return -1;
    }
    
    shm = (shm_buffer_t *)shmat(shmid, NULL, 0);
    if(shm == (void *)-1) {
        LOG_ERROR("read_from_shm: 附加共享内存失败");
        return -1;
    }
    
    read_pos = shm->write_pos - sizeof(sensor_data_t);
    if(read_pos < 0) {
        LOG_WARN("read_from_shm: 共享内存中暂无数据");
        shmdt(shm);
        return -1;
    }
    
    memcpy(sensor, shm->data + read_pos, sizeof(sensor_data_t));
    shmdt(shm);
    
    return 0;
}
