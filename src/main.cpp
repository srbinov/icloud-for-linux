#include "app.hpp"

#include <gui/choc_WebView.h>

#include <gui/choc_DesktopWindow.h>
#include <gui/choc_MessageLoop.h>

#if defined( __linux__ )
#include <webkit2/webkit2.h>
#endif

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

namespace
{

std::string envOrEmpty ( const char* name )
{
    const char* value = getenv ( name );
    return ( value != nullptr ) ? std::string ( value ) : std::string();
}

void configureWebView ( void* viewHandle, const std::filesystem::path& persistDir )
{
#if defined( __linux__ )
    if ( viewHandle == nullptr )
        return;

    auto* webView = WEBKIT_WEB_VIEW ( viewHandle );
    auto* context = webkit_web_view_get_context ( webView );
    auto* dataManager = webkit_web_context_get_website_data_manager ( context );
    auto* cookieManager = webkit_website_data_manager_get_cookie_manager ( dataManager );
    const auto cookiePath = ( persistDir / "cookies.sqlite" ).string();

    webkit_cookie_manager_set_persistent_storage (
        cookieManager,
        cookiePath.c_str(),
        WEBKIT_COOKIE_PERSISTENT_STORAGE_SQLITE );
    webkit_cookie_manager_set_accept_policy ( cookieManager, WEBKIT_COOKIE_POLICY_ACCEPT_ALWAYS );

    g_signal_connect (
        webView,
        "permission-request",
        G_CALLBACK ( +[] ( WebKitWebView*, WebKitPermissionRequest* request, gpointer ) -> gboolean {
            if ( WEBKIT_IS_GEOLOCATION_PERMISSION_REQUEST ( request ) )
            {
                webkit_permission_request_allow ( request );
                return TRUE;
            }

            return FALSE;
        } ),
        nullptr );
#else
    (void) viewHandle;
    (void) persistDir;
#endif
}

}  // namespace

int main ( int, char** argv )
{
    std::string tld = ".com";

    const auto persistDir = icloud::persistentStorageDir (
        envOrEmpty ( "SNAP_USER_COMMON" ),
        envOrEmpty ( "XDG_DATA_HOME" ),
        envOrEmpty ( "HOME" ) );
    std::error_code errorCode;
    std::filesystem::create_directories ( persistDir / "data", errorCode );
    std::filesystem::create_directories ( persistDir / "cache", errorCode );

    const auto dataHome = ( persistDir / "data" ).string();
    const auto cacheHome = ( persistDir / "cache" ).string();
    setenv ( "XDG_DATA_HOME", dataHome.c_str(), 1 );
    setenv ( "XDG_CACHE_HOME", cacheHome.c_str(), 1 );

    const char* snapCommon = getenv ( "SNAP_USER_COMMON" );
    if ( snapCommon != nullptr && snapCommon[0] != 0 )
    {
        std::filesystem::path tldPath ( std::filesystem::path ( snapCommon ) / "tld" );
        if ( std::filesystem::exists ( tldPath ) )
        {
            std::ifstream tldIs ( tldPath );
            tld = std::string ( ( std::istreambuf_iterator<char> ( tldIs ) ), {} );
            tldIs.close();
        }
    }

    const std::string target = argv[1];
    const std::string name = argv[2];
    const std::string url = icloud::targetUrl ( target, tld );

    choc::ui::DesktopWindow appWin ( choc::ui::Bounds{ 0, 0, 1000, 600 } );
    appWin.setVisible ( false );
    appWin.centreWithSize ( 1000, 600 );
    appWin.setVisible ( true );
    appWin.setWindowTitle ( icloud::windowTitle ( target, name ) );
    appWin.windowClosed = []() { choc::messageloop::stop(); };

    choc::ui::WebView::Options webViewOptions;
    // https://www.whatismybrowser.com/guides/the-latest-user-agent/chrome
    webViewOptions.customUserAgent = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/141.0.0.0 Safari/537.36";
    choc::ui::WebView webView ( webViewOptions );
    configureWebView ( webView.getViewHandle(), persistDir );
    webView.navigate ( url );

    appWin.setContent ( webView.getViewHandle() );
    appWin.toFront();

    webView.onNewWindow ( [target, name, persistDir] ( const std::string& popupUrl ) {
        choc::ui::DesktopWindow appWin2 ( choc::ui::Bounds{ 0, 0, 1000, 600 } );
        appWin2.setVisible ( false );
        appWin2.centreWithSize ( 1000, 600 );
        appWin2.setVisible ( true );
        appWin2.setWindowTitle ( icloud::windowTitle ( target, name, true ) );

        choc::ui::WebView::Options webView2Options;
        webView2Options.customUserAgent = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/135.0.7123.45 Safari/537.36";
        choc::ui::WebView webView2 ( webView2Options );
        configureWebView ( webView2.getViewHandle(), persistDir );
        webView2.navigate ( popupUrl );

        appWin2.setContent ( webView2.getViewHandle() );
        appWin2.toFront();

        return appWin2.getWindowHandle();
    } );

    choc::messageloop::run();
    return 0;
}
