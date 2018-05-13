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
	    isDirectory = S_ISDIR(path_stat.st_mode);
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
		exists = access(path.c_str(), F_OK) != -1;
#endif

		return exists;
	}

	std::list<std::string> Filesystem::GetDirectoryContents(std::string path, lp::FilesystemObjectType filter) {
		std::list<std::string> contents;

#if defined(_WIN32) || defined(_WIN64)
		HANDLE hFind;
		WIN32_FIND_DATA FindData;

		hFind = FindFirstFile((path + "\\*").c_str(), &FindData);

		if(FindData.cFileName != nullptr && FindData.cFileName[0] != 0) {
			do {
				std::string name = FindData.cFileName;
				std::string fullPath = path + "\\" + name;

				if(static_cast<int>(filter) & static_cast<int>(lp::FilesystemObjectType::File) && IsFile(fullPath)) {
					contents.push_back(name);
				}

				if(static_cast<int>(filter) & static_cast<int>(lp::FilesystemObjectType::Directory) && IsDirectory(fullPath)) {
					contents.push_back(name);
				}
			} while (FindNextFile(hFind, &FindData));
		}
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
		DIR* d;
		struct dirent* dir;

		d = opendir(path.c_str());
		if(d) {
			while((dir = readdir(d)) != nullptr
				) {
				std::string name = dir->d_name;
				std::string fullPath = path + "/" + name;

				if(static_cast<int>(filter) & static_cast<int>(lp::FilesystemObjectType::File) && IsFile(fullPath)) {
					contents.push_back(name);
				}

				if(static_cast<int>(filter) & static_cast<int>(lp::FilesystemObjectType::Directory) && IsDirectory(fullPath)) {
					contents.push_back(name);
				}
			}

			closedir(d);
		}
#endif

		return contents;
	}

	std::string Filesystem::GetPathOfRunningExecutable() {
		std::string path;
		char buffer[1024];
		int bufferLength = sizeof(buffer);

#if defined(_WIN32) || defined(_WIN64)
		DWORD bytes = GetModuleFileName(nullptr, buffer, (DWORD)bufferLength);

		if(bytes > 0) {
			path = buffer;
		}
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
		char tmp[32];
		sprintf(tmp, "/proc/%d/exe", getpid());
		int bytes = std::min(readlink(tmp, buffer, bufferLength), bufferLength - 1);
		if(bytes >= 0) {
    	buffer[bytes] = '\0';
		}

		path = buffer;
#endif

		return path;
	}

	std::string Filesystem::GetWorkingDirectory() {
		std::string path;

		char buffer[1024];
		int bufferLength = sizeof(buffer);

#if defined(_WIN32) || defined(_WIN64)
		DWORD result = GetCurrentDirectory((DWORD)bufferLength, buffer);

		if(result > 0) {
			path = buffer;
		}
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
		if(getcwd(buffer, bufferLength) != NULL) {
			path = buffer;
		}
#endif

		return path;
	}
}