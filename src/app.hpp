#pragma once

#include <filesystem>
#include <string>

namespace icloud
{

std::string targetUrl ( const std::string& target, const std::string& tld );
std::string windowTitle ( const std::string& target, const std::string& name, bool popup = false );
std::filesystem::path persistentStorageDir ( const std::string& snapCommon,
                                             const std::string& xdgDataHome,
                                             const std::string& home );

}  // namespace icloud
