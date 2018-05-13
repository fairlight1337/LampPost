#include <lamppost/Filesystem.h>


namespace lp {
	Filesystem::Filesystem() {
	}

	Filesystem::~Filesystem() {
	}

	bool Filesystem::IsDirectory(std::string path) {
		bool isDirectory = false;

#if defined(_WIN32) || defined(_WIN64)
		DWORD ftyp = GetFileAttributesA(path.c_str());

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY && !(ftyp & INVALID_FILE_ATTRIBUTES)) {
			isDirectory = true;
		}
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
		struct stat path_stat;

    if(stat(path.c_str(), &path_stat) == 0) {
	    isFile = S_ISDIR(path_stat.st_mode);
    }
#endif

		return isDirectory;
	}

	bool Filesystem::IsFile(std::string path) {
		bool isFile = false;

#if defined(_WIN32) || defined(_WIN64)
		DWORD ftyp = GetFileAttributesA(path.c_str());

		if (ftyp & FILE_ATTRIBUTE_NORMAL && !(ftyp & INVALID_FILE_ATTRIBUTES)) {
			isFile = true;
		}
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
		struct stat path_stat;

    if(stat(path.c_str(), &path_stat) == 0) {
	    isFile = S_ISREG(path_stat.st_mode);
    }
#endif

		return isFile;
	}

	bool Filesystem::PathExists(std::string path) {
		bool exists = false;

#if defined(_WIN32) || defined(_WIN64)
		exists = (bool)PathFileExists(path.c_str());
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
		exists = access(path.c_str()) != -1;
#endif

		return exists;
	}
}