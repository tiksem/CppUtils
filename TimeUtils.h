//
// Created by Semyon Tikhonenko on 1/11/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_TIMEUTILS_H
#define VOCALTRAINER_TIMEUTILS_H


#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <sstream>

namespace CppUtils {
namespace TimeUtils {
	int64_t NowInMicroseconds();
	std::string NowInMicrosecondsInPrettyFormat();
	double NowInSeconds();

	// Uses std::put_time format
	template<typename Char, typename Stream>
	void WriteFormattedTimeToStream(const Char* format, int timeInSeconds, Stream& out) {
		time_t asTimeT = timeInSeconds;
		auto time = *std::localtime(&asTimeT);
		out << std::put_time(&time, format);
	}

	template<typename Char>
	std::basic_string<Char> ConvertSecondsToFormattedTimeString(const Char* format, int timeInSeconds) {
		std::basic_stringstream<Char> stream;
		WriteFormattedTimeToStream(format, timeInSeconds, stream);
		return stream.str();
	}

	template <typename Func, typename Stream = std::ostream>
	void LogExecutionTime(const char* label, Func func, Stream& os = std::cout) {
		auto now = NowInMicroseconds();
		func();
		auto diff = NowInMicroseconds() - now;
		os<<label<<" = "<<diff<<"\n";
	}

	namespace {
	    std::unordered_map<std::string, int64_t> times;
	}

	template <typename Char>
    void LogExecutionTimeStart(const Char* label) {
        times[label] = NowInMicroseconds();
	}

    template <typename Stream = std::ostream>
    void LogExecutionTimeEnd(const char* label, Stream& os = std::cout) {
        int64_t now = NowInMicroseconds();
        auto before = times[label];
        auto diff = now - before;
        os<<label<<" = "<<diff<<"\n";
    }

    struct ExecuteTimeLoggerGuard {
	    std::string label;
	    std::ostream& os;

        ExecuteTimeLoggerGuard(const char* label, std::ostream& os = std::cout);
        ~ExecuteTimeLoggerGuard();
	};
}
}


#endif //VOCALTRAINER_TIMEUTILS_H
