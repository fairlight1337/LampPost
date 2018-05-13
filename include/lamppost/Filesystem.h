#ifndef LAMPPOST_FILESYSTEM_H
#define LAMPPOST_FILESYSTEM_H


#include <string>
#include <list>
#include <algorithm>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include "Shlwapi.h"
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#endif


namespace lp {
	enum class FilesystemObjectType : int {
		File = 1,
		Directory = 2,
		Any = 3
	};

	class Filesystem {
	public:
		Filesystem();
		~Filesystem();

		static bool PathExists(std::string path);
		static bool IsFile(std::string path);
		static bool IsDirectory(std::string path);

		static std::list<std::string> GetDirectoryContents(std::string path, FilesystemObjectType filter);

		static std::string GetPathOfRunningExecutable();
		static std::string GetWorkingDirectory();
	};
}


#endif //LAMPPOST_FILESYSTEM_H
