#pragma once


#ifdef FR_DEBUG
	#define FR_ENABLE_ASSERTS
	#define FR_ENABLE_DEBUG_LOGS
#endif


//------------ Asserts -----------------------------

#ifdef  FR_ENABLE_ASSERTS
	#define FR_ASSERT(x, ...)		{ if(!(x)) { std::cout << "ERROR: Assertion Failed: " << __LINE__ << " " __VA_ARGS__ << std::endl; __debugbreak(); } }
	#define FR_CORE_ASSERT(x, ...)	{ if(!(x)) { std::cout << "CORE ERROR: Assertion Failed" << __LINE__ << " " <<  __VA_ARGS__ << std::endl; __debugbreak(); } }
#else
	#define FR_ASSERT(x, ...)
	#define FR_CORE_ASSERT(x, ...)
#endif //  CD_ENABLE_ASSERTS



//------------ Logging -----------------------------


#ifdef FR_ENABLE_DEBUG_LOGS
	#define DLOG_CORE_FATAL(...)	std::cout << "CORE FATAL:   " << __VA_ARGS__ << std::endl;
	#define DLOG_CORE_ERROR(...)	std::cout << "CORE ERROR:   " << __VA_ARGS__ << std::endl;
	#define DLOG_CORE_WARN(...)		std::cout << "CORE WARNING: " << __VA_ARGS__ << std::endl;
	#define DLOG_CORE_INFO(...)		std::cout << "CORE INFO:    " << __VA_ARGS__ << std::endl;
	
	#define DLOG_FATAL(...)	std::cout << "APP FATAL:   " << __VA_ARGS__ << std::endl;
	#define DLOG_ERROR(...)	std::cout << "APP ERROR:   " << __VA_ARGS__ << std::endl;
	#define DLOG_WARN(...)	std::cout << "APP WARNING: " << __VA_ARGS__ << std::endl;
	#define DLOG_INFO(...)	std::cout << "APP INFO:    " << __VA_ARGS__ << std::endl;

#else
	#define DLOG_CORE_FATAL(...)
	#define DLOG_CORE_ERROR(...)
	#define DLOG_CORE_WARN(...)	
	#define DLOG_CORE_INFO(...)	

	#define DLOG_FATAL(...)
	#define DLOG_ERROR(...)
	#define DLOG_WARN(...)	
	#define DLOG_INFO(...)	
#endif