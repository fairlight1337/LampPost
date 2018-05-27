#include <lamppost/Filesystem.h>


namespace lp {
  bool Filesystem::IsDirectory(const std::string& path) {
    bool isDirectory = false;

#if defined(_WIN32) || defined(_WIN64)
    DWORD ftyp = GetFileAttributesA(path.c_str());

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
      isDirectory = true;
    }
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    struct stat path_stat = {};

    if(stat(path.c_str(), &path_stat) == 0) {
      isDirectory = S_ISDIR(path_stat.st_mode);
    }
#endif

    return isDirectory;
  }

  bool Filesystem::IsFile(const std::string& path) {
    bool isFile = false;

#if defined(_WIN32) || defined(_WIN64)
    DWORD ftyp = GetFileAttributesA(path.c_str());

    if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY)) {
      isFile = true;
    }
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    struct stat path_stat = {};

    if(stat(path.c_str(), &path_stat) == 0) {
      isFile = S_ISREG(path_stat.st_mode);
    }
#endif

    return isFile;
  }

  bool Filesystem::PathExists(const std::string& path) {
    bool exists = false;

#if defined(_WIN32) || defined(_WIN64)
    exists = (bool)PathFileExists(path.c_str());
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    exists = access(path.c_str(), F_OK) != -1;
#endif

    return exists;
  }

  std::list<std::string> Filesystem::GetDirectoryContents(const std::string& path, lp::FilesystemObjectType filter) {
    std::list<std::string> contents;

#if defined(_WIN32) || defined(_WIN64)
    HANDLE hFind;
    WIN32_FIND_DATA FindData;

    hFind = FindFirstFile(Filesystem::CombinePaths(path, "*").c_str(), &FindData);
    if(FindData.cFileName != nullptr && FindData.cFileName[0] != 0) {
      do {
        std::string name = FindData.cFileName;

        if(name != "." && name != "..") {
          std::string fullPath = CombinePaths(path, name);

          if(static_cast<int>(filter) & static_cast<int>(lp::FilesystemObjectType::File) && IsFile(fullPath)) {
            contents.push_back(name);
          }

          if(static_cast<int>(filter) & static_cast<int>(lp::FilesystemObjectType::Directory) && IsDirectory(fullPath)) {
            contents.push_back(name);
          }
        }
      } while (FindNextFile(hFind, &FindData));
    }
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    DIR* d;
    struct dirent* dir;

    d = opendir(path.c_str());
    if(d != nullptr) {
      while((dir = readdir(d)) != nullptr) {
        std::string name = static_cast<char*>(dir->d_name);
        std::string fullPath = CombinePaths(path, name);

        bool filterIncludesFiles = (static_cast<int>(filter) & static_cast<int>(lp::FilesystemObjectType::File)) != 0;
        if(filterIncludesFiles && IsFile(fullPath)) {
          contents.push_back(name);
        }

        bool filterIncludesDirectories = (static_cast<int>(filter) & static_cast<int>(lp::FilesystemObjectType::File)) != 0;
        if(filterIncludesDirectories && IsDirectory(fullPath)) {
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
      path = std::string(buffer, bufferLength);
    }
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    std::string procExePath = "/proc/" + std::to_string(getpid()) + "/exe";
    int bytes = std::min(static_cast<int>(readlink(procExePath.c_str(), buffer, bufferLength)), bufferLength - 1);

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
    if(getcwd(buffer, bufferLength) != nullptr) {
      path = static_cast<char*>(buffer);
    }
#endif

    return path;
  }

  std::string Filesystem::CombinePaths(const std::string& root, const std::string& relative) {
    std::string combined = root;

#if defined(_WIN32) || defined(_WIN64)
    combined += "\\";
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    combined += "/";
#endif

    combined += relative;

    return combined;
  }

  std::string Filesystem::GetFilename(const std::string& path) {
    std::string filename;

#if defined(_WIN32) || defined(_WIN64)
    size_t pos = path.find_last_of('\\');
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    size_t pos = path.find_last_of('/');
#endif

    if(pos != std::string::npos && path.size() > pos + 1) {
      filename = path.substr(pos + 1);
    }

    return filename;
  }

  std::string Filesystem::GetBaseDirectory(const std::string& path) {
    std::string directory;

#if defined(_WIN32) || defined(_WIN64)
    size_t pos = path.find_last_of('\\');
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    size_t pos = path.find_last_of('/');
#endif

    if(pos != std::string::npos) {
      directory = path.substr(0, pos);
    }

    return directory;
  }

  std::string Filesystem::GetFileExtension(const std::string& path) {
    std::string extension;

    std::string filename = GetFilename(path);
    if(filename != "") {
      size_t pos = filename.find_last_of('.');

      if(pos != std::string::npos) {
        extension = filename.substr(pos + 1);
      }
    }

    return extension;
  }
} // namespace lp
