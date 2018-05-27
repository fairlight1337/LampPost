#ifndef LAMPPOST_FILESYSTEM_H
#define LAMPPOST_FILESYSTEM_H


#include <string>
#include <list>
#include <algorithm>
#include <iostream>

#include <string.h>


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
    Filesystem() = default;
    virtual ~Filesystem() = default;

    static bool PathExists(const std::string& path);
    static bool IsFile(const std::string& path);
    static bool IsDirectory(const std::string& path);

    static std::list<std::string> GetDirectoryContents(const std::string& path, FilesystemObjectType filter);

    static std::string GetPathOfRunningExecutable();
    static std::string GetWorkingDirectory();

    static std::string CombinePaths(const std::string& root, const std::string& relative);

    static std::string GetFilename(const std::string& path);
    static std::string GetBaseDirectory(const std::string& path);
    static std::string GetFileExtension(const std::string& path);
  };
} // namespace lp


#endif //LAMPPOST_FILESYSTEM_H
