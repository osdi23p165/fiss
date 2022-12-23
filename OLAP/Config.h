#ifndef CONFIG_H
#define CONFIG_H

class Config
{
private:
	static int LOG_WORKER_NUM;
	static const char CSV_DELIMITER = '|';

public:
	static int getLogWorkerNum();
};

#endif