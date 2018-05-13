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
#endif

		return isFile;
	}

	bool Filesystem::PathExists(std::string path) {
		bool exists = false;

#if defined(_WIN32) || defined(_WIN64)
		exists = PathFileExists(path.c_str());
#endif

		return exists;
	}
}