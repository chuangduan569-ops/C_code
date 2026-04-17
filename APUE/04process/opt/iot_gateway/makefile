SRCS = main.c daemon.c log.c config.c network.c \
       io_multiplexing.c thread_pool.c process_pool.c \
       token_bucket.c alarm_scheduler.c ipc_modules.c \
       password_auth.c queue.c sensor_parser.c http_client.c base64.c

OBJS = $(SRCS:.c=.o)

TARGET = iot_gateway

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $^ -lpthread -lcrypt -lrt

clean:
	rm -f $(OBJS) $(TARGET)
