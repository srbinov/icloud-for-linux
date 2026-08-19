#include "app.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

static int g_failed = 0;

static void expectEqual ( const std::string& name, const std::string& actual, const std::string& expected )
{
    if ( actual != expected )
    {
        std::cerr << "FAIL " << name << "\n  expected: " << expected << "\n  actual:   " << actual << "\n";
        ++g_failed;
        return;
    }

    std::cout << "PASS " << name << "\n";
}

int main()
{
    expectEqual ( "mail slug uses icloud host",
                  icloud::targetUrl ( "mail", ".com" ),
                  "https://www.icloud.com/mail" );
    expectEqual ( "china tld is applied to icloud slug",
                  icloud::targetUrl ( "mail", ".com.cn" ),
                  "https://www.icloud.com.cn/mail" );
    expectEqual ( "hostname becomes https url",
                  icloud::targetUrl ( "maps.apple.com", ".com" ),
                  "https://maps.apple.com" );
    expectEqual ( "full url is used as-is",
                  icloud::targetUrl ( "https://maps.apple.com/", ".com" ),
                  "https://maps.apple.com/" );

    expectEqual ( "icloud app title is prefixed",
                  icloud::windowTitle ( "mail", "Mail" ),
                  "iCloud Mail" );
    expectEqual ( "external app title is used as-is",
                  icloud::windowTitle ( "https://maps.apple.com/", "Apple Maps" ),
                  "Apple Maps" );
    expectEqual ( "popup title adds marker",
                  icloud::windowTitle ( "mail", "Mail", true ),
                  "iCloud Mail ⧉" );
    expectEqual ( "external popup title adds marker",
                  icloud::windowTitle ( "maps.apple.com", "Apple Maps", true ),
                  "Apple Maps ⧉" );

    expectEqual ( "snap common is preferred for persistence",
                  icloud::persistentStorageDir ( "/home/user/snap/icloud-for-linux/common", "/xdg", "/home/user" ).string(),
                  "/home/user/snap/icloud-for-linux/common/webkit" );
    expectEqual ( "xdg data home is used when not in a snap",
                  icloud::persistentStorageDir ( "", "/xdg", "/home/user" ).string(),
                  "/xdg/icloud-for-linux" );
    expectEqual ( "home is used when xdg is unset",
                  icloud::persistentStorageDir ( "", "", "/home/user" ).string(),
                  "/home/user/.local/share/icloud-for-linux" );

    if ( g_failed != 0 )
    {
        std::cerr << g_failed << " test(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "all tests passed\n";
    return EXIT_SUCCESS;
}
