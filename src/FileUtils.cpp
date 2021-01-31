#include "FileUtils.hpp"

#include <sstream>
#include <filesystem>
#include <fstream>

FileUtils FileUtils::_instance;

FileUtils::FileUtils()
{
}

FileUtils::~FileUtils()
{
}

const FileUtils &FileUtils::GetInstance()
{
    return FileUtils::_instance;
}

std::string FileUtils::getFullPath(const std::string &path) const
{
    std::filesystem::path p{__FILE__};

    std::stringstream ss;
    ss << p.parent_path().parent_path().c_str();
    ss << "/assets";
    ss << path;

    return ss.str();
}

std::string FileUtils::getFileContent(const std::string &path) const
{
    std::ifstream infile(getFullPath(path));

    std::ifstream ifs(getFullPath(path));
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));

    return content;
}
