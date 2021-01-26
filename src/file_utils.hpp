#ifndef __FILE_UTILS_HPP__
#define __FILE_UTILS_HPP__

#include <string>

class FileUtils
{
public:
    static const FileUtils &GetInstance();

public:
    std::string getFullPath(const std::string &path) const;
    std::string getFileContent(const std::string &path) const;

private:
    FileUtils();
    ~FileUtils();

    static FileUtils _instance;
};

#endif /* __FILE_UTILS_HPP__ */
