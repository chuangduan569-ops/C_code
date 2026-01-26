/*
完成mycal命令
需要支持 mycal.c -> gcc mycal.c -o mycal
./mycal    对标 Linux系统自带的cal命令
./mycal 月份 年份 对标 Linux系统自带的cal命令
如何确定某年某月的1号是星期几?
蔡勒公式
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//判断是否为闰年
int isLeapYear(int year){
	return (year % 400 == 0)||(year % 4 ==0 && year % 100 !=0);
}
//蔡勒公式：计算某年某月某日是星期几
int getWeekDay(int year, int month, int day){
	int m = month, y = year;
	if(month == 1 || month == 2){
		m += 12;
		y -= 1;
	}
	int c = y/100;//世纪数
	int d = day;//日期
	int y2 = y % 100;//年份后两位
	int w = (y2 + y2/4 +c/4 - 2*c + 26*(m+1)/10 + d-1);
	//处理模运算和负数
	w = w % 7;
	if(w<0)
	w += 7;
	//蔡勒公式中：0 = 周六  1 = 周日     6 = 周五
	//转换为  1 = 周一     6 = 周六  0 = 周日
	//return (w+6) % 7;
}
//获取某月一号星期几  调用getWeekDay
int getFirstDayofMonth(int year, int month){
	//计算1号星期几
	return getWeekDay(year,month,1);
}
//获取某年某月的天数
int getMonthDay(int year, int month){
	int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if(month == 2 && isLeapYear(year)){
		return 29;
	}
	return days[month-1];
	
}
//打印月份名称
void printMonthName(int month){
	//定义一个指针数组，用来指向数组成员
	char *months[] =	{"一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月" };
	printf("	%s",months[month - 1]); 
	
}
//打印日历
void printCalendar(int year, int month){
	//打印提示
	printMonthName(month);//月
	printf(" %d\n", year);//年
	printf("日 一 二 三 四 五 六\n");//星期
	//获取当月天数
	int days = getMonthDay(year,month);
	
	//获取1号星期几
	int firstDay = getWeekDay(year,month,1);//泰勒公式

	//打印前导空格
	for(int i = 0;i<firstDay;i++)
		printf("   ");
	
	//打印日期
	for(int day = 1;day <= days;day++){
		printf("%2d ",day);

	//每7天换行
	if((firstDay + day) % 7 == 0){
		printf("\n");
		}
	}
	//最后如果没有换行，就打印换行
	if((firstDay + days) % 7 != 0){
		printf("\n");
		}

}




int main(int argc, char *argv[])
{
//	if(atoi(argv[1]) > 12 || atoi(argv[1]) < 1)
//	printf("%s neither a month number (1..12) nor a name\n",argv[1]);
//  用month取argv[1]的值
	int year, month;
	//获取当前时间(这里使用结构体)
	time_t now;
	struct tm *local;
	
	time(&now);
	local = localtime(&now);
	int current_year = local->tm_year + 1900;
	int current_month = local->tm_mon + 1;
	
	//处理参数
	if(argc == 1){
		//无参数：显示当前日历
		year = current_year;
		month = current_month;
	}	
	else if(argc == 2){
		//一个参数：月份
		month = atoi(argv[1]);
		if(month<1 || month>12){
			printf("mycal: %s is neither a month number (1..12) nor a name\n", argv[1]);
			return 1;
		}
		year = current_year;
	}
	else if(argc == 3){
		//两个参数   月  年
	month = atoi(argv[1]);
	year = atoi(argv[2]);
	if(month<1 || month >12){
	printf("mycal: %s is neither a month number (1..12) nor a name\n", argv[1]);
	return 1;
	}
	if(year < 1 || year >9999){
	printf("mycal: year %d not in range 1..9999\n", year);
        return 1;
	}
	//else{
	//printf("Usage: mycal [month] [year]\n");//当用户输入错误的参数时，程序会显示这个帮助信息。
	//return 1;
	
	}	
	printCalendar(year,month);
	return 0;
		
		
	
	
	
	
	


}

// 获取当前日期
//int current_day = local->tm_mday;  // 直接就是1-31，不需要转换

// 获取当前星期
//int current_weekday = local->tm_wday;  // 0=周日, 1=周一, ..., 6=周六

// 获取当前小时
//int current_hour = local->tm_hour;  // 0-23，24小时制

// 获取当前分钟
//int current_minute = local->tm_min;  // 0-59

// 获取当前秒
//int current_second = local->tm_sec;  // 0-60 (60用于闰秒)

