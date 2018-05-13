#ifndef LAMPPOST_FILESYSTEM_H
#define LAMPPOST_FILESYSTEM_H


#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include "Shlwapi.h"
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif


namespace lp {
	class Filesystem {
	public:
		Filesystem();
		~Filesystem();

		static bool PathExists(std::string path);
		static bool IsFile(std::string path);
		static bool IsDirectory(std::string path);
	};
}


#endif //LAMPPOST_FILESYSTEM_H
