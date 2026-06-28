#pragma once

//If we are in debug mode, enable debug logs and asserts
#ifdef FR_DEBUG
	#define FR_ENABLE_ASSERTS
	#define FR_ENABLE_DEBUG_LOGS
#endif


//------------ Asserts -----------------------------

#ifdef  FR_ENABLE_ASSERTS
	#define FR_ASSERT(expr, msg)                                                \
		do                                                                      \
		{                                                                       \
			if (!(expr))                                                        \
			{                                                                   \
				std::cerr << "Assertion failed!\n"                              \
						  << "Expression: " << #expr << '\n'                     \
						  << "Message: " << msg << '\n'                          \
						  << "File: " << __FILE__ << '\n'                        \
						  << "Line: " << __LINE__ << std::endl;                  \
				std::abort();                                                   \
			}                                                                   \
		} while (0)

	#define FR_CORE_ASSERT(expr, msg) FR_ASSERT(expr, msg)


#else
	#define FR_ASSERT(x, ...)
	#define FR_CORE_ASSERT(x, ...)
#endif //  CD_ENABLE_ASSERTS



//------------ Logging -----------------------------


#ifdef FR_ENABLE_DEBUG_LOGS
	#define DLOG_CORE_FATAL(...)	std::cerr << "CORE FATAL:   " << __VA_ARGS__ << std::endl;
	#define DLOG_CORE_ERROR(...)	std::cerr << "CORE ERROR:   " << __VA_ARGS__ << std::endl;
	#define DLOG_CORE_WARN(...)		std::cout << "CORE WARNING: " << __VA_ARGS__ << std::endl;
	#define DLOG_CORE_INFO(...)		std::cout << "CORE INFO:    " << __VA_ARGS__ << std::endl;
	
	#define DLOG_FATAL(...)	std::cerr << "APP FATAL:   " << __VA_ARGS__ << std::endl;
	#define DLOG_ERROR(...)	std::cerr << "APP ERROR:   " << __VA_ARGS__ << std::endl;
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