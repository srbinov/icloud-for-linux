#include "app.hpp"

namespace icloud
{

static bool isICloudSlug ( const std::string& target )
{
    return target.find ( "://" ) == std::string::npos && target.find ( '.' ) == std::string::npos;
}

std::string targetUrl ( const std::string& target, const std::string& tld )
{
    if ( target.find ( "://" ) != std::string::npos )
        return target;

    if ( target.find ( '.' ) != std::string::npos )
        return "https://" + target;

    return "https://www.icloud" + tld + "/" + target;
}

std::string windowTitle ( const std::string& target, const std::string& name, bool popup )
{
    std::string title = isICloudSlug ( target ) ? ( "iCloud " + name ) : name;

    if ( popup )
        title += " ⧉";

    return title;
}

std::filesystem::path persistentStorageDir ( const std::string& snapCommon,
                                             const std::string& xdgDataHome,
                                             const std::string& home )
{
    if ( ! snapCommon.empty() )
        return std::filesystem::path ( snapCommon ) / "webkit";

    if ( ! xdgDataHome.empty() )
        return std::filesystem::path ( xdgDataHome ) / "icloud-for-linux";

    return std::filesystem::path ( home ) / ".local" / "share" / "icloud-for-linux";
}

}  // namespace icloud
