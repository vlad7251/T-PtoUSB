#ifndef WEBSERVER_H
#define WEBSERVER_H
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <SaveSettings.h>
using namespace httpsserver;

class WebServer {
public:
    void begin();
    void handleRoot(httpsserver::HTTPRequest * request, httpsserver::HTTPResponse * response);
    void handle404(httpsserver::HTTPRequest * request, httpsserver::HTTPResponse * response);
    

private:
    static void handleRootWrapper(httpsserver::HTTPRequest * request, httpsserver::HTTPResponse * response);
    static void handle404Wrapper(httpsserver::HTTPRequest * request, httpsserver::HTTPResponse * response);
    void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
    void discardRequestBody(httpsserver::HTTPRequest * request); 
    httpsserver::SSLCert *cert;
    httpsserver::HTTPSServer *secureServer;
    SaveSettings saveSettings;
    const uint16_t SERVER_PORT = 443;
};
extern WebServer g_WebServer;

#endif  
