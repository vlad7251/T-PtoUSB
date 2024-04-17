#include "WebServer.h"
#include <WiFi.h>
#include <ResourceNode.hpp>
#include <string>
WebServer g_WebServer;
/*unsigned char cert [] = {
    #include <
}*/


void WebServer::handleRootWrapper(httpsserver::HTTPRequest * request, httpsserver::HTTPResponse * response) 
{
    g_WebServer.handleRoot(request, response);
}

// Оболочка для handle404
void WebServer::handle404Wrapper(httpsserver::HTTPRequest * request, httpsserver::HTTPResponse * response) 
{
    g_WebServer.handle404(request, response);
}

void WebServer::WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
 {
    Serial.write(("\n" + WiFi.localIP().toString() + "\n").c_str());
}

void WebServer::begin() 
{
    saveSettings.begin();

    Serial.write("Connecting to SSID: ");
    Serial.println(saveSettings.getStaSSID());

    WiFi.begin(saveSettings.getStaSSID(), saveSettings.getStaPassword());
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    WiFi.onEvent(std::bind(&WebServer::WiFiGotIP, this, std::placeholders::_1, std::placeholders::_2), WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    Serial.println("Settings loaded and WiFi connected.");
    Serial.println("Creating a new self-signed certificate.");
    Serial.println("This may take up to a minute, so be patient ;-)");

    cert = new httpsserver::SSLCert();
    int createCertResult = createSelfSignedCert(
        *cert,
        KEYSIZE_2048,
        "CN=myesp32.local,O=FancyCompany,C=DE",
        "20190101000000",
        "20300101000000"
    ); 
    //Моргание лампочкой после генерации
    /*pinMode(LED, OUTPUT);
    delay(1000);
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);*/
    //**********************************
    if (createCertResult != 0) {
        Serial.printf("Creating certificate failed. Error Code = 0x%02X\n", createCertResult);
        while(true) delay(500);
    }
    Serial.println("Creating the certificate was successful");

    secureServer = new httpsserver::HTTPSServer(cert, SERVER_PORT);

    /*Serial.println("Setting up WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PSK);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("Connected. IP=");
    Serial.println(WiFi.localIP());*/

    //ResourceNode * nodeRoot = new ResourceNode("/", "GET", std::bind(&WebServer::handleRoot, this, std::placeholders::_1, std::placeholders::_2));
    //ResourceNode * node404 = new ResourceNode("", "GET", std::bind(&WebServer::handle404, this, std::placeholders::_1, std::placeholders::_2));
    // Создание ResourceNode с помощью std::function
    // Создаем объект HTTPSCallbackFunction для handleRoot
    // Создаем HTTPSCallbackFunction для handleRoot

   ResourceNode * nodeRoot = new ResourceNode("/", "GET", &WebServer::handleRootWrapper);
   ResourceNode * node404 = new ResourceNode("", "GET", &WebServer::handle404Wrapper);


    secureServer->registerNode(nodeRoot);
    secureServer->setDefaultNode(node404);

    Serial.println("Starting server...");
    secureServer->start();
    if (secureServer->isRunning()) {
        Serial.println("Server ready.");
    }
}

void WebServer::handleRoot(httpsserver::HTTPRequest * request, httpsserver::HTTPResponse * response)
{
   String html = R"html(
        <html>
            <head>
                <style>
                    body {
                        font-family: Arial, sans-serif;
                        margin: 20px;
                    }
                    h1 {
                        color: #333333;
                    }
                    form {
                        width: 300px;
                        margin: 20px 0;
                    }
                    label {
                        display: block;
                        margin-bottom: 5px;
                    }
                    input {
                        width: 100%;
                        padding: 8px;
                        margin-bottom: 10px;
                    }
                    input[type="submit"] {
                        background-color: #4CAF50;
                        color: white;
                        cursor: pointer;
                    }
                </style>
            </head>
            <body>
                <h1>WiFi Settings</h1>
                <form action='/save' method='post'>
                    <label for='ssid'>SSID:</label>
                    <input type='text' name='ssid' value='" + String(settings.staSSID) + "' required><br>
                    <label for='password'>Password:</label>
                    <input type='password' name='password' placeholder='Enter new password' required><br>
                    <label for='baud'>Baud Rate:</label>
                    <input type='number' name='baud' value='" + String(settings.baudRate) + "' required><br>
                    <label for='transPort'>Transparent Port:</label>
                    <input type='number' name='transPort' value='" + String(settings.transparentPort) + "' required><br>
                    <label for='commandPort'>Command Port:</label>
                    <input type='number' name='commandPort' value='" + String(settings.commandPort) + "' required><br>
                    <input type='submit' value='Save'>
                </form>
            </body>
        </html>
    )html";
    
    response->setHeader("Content-Type", "text/html");
    response->write((const uint8_t *)html.c_str(), html.length());
        //TODO промежуточная версия, дополнить записью в память, создать объект

    std::string bufferParams = "";
    if (request->getParams()->isQueryParameterSet("ssid"))
    {
         //strncpy(newSettings.staSSID,  request->getParam("ssid", true)->value().c_str(), sizeof(newSettings.staSSID));
        std::string paramName = "ssid";
        request->getParams()->getQueryParameter(paramName, bufferParams);
        saveSettings.setStaSSID(bufferParams.c_str());
    }
       
    if (request->getParams()->isQueryParameterSet("password"))
    {
         //strncpy(newSettings.staPassword,  request->getParam("password", true)->value().c_str(), sizeof(newSettings.staPassword));
        std::string paramName = "password";
        request->getParams()->getQueryParameter(paramName, bufferParams);
        saveSettings.setStaPassword(bufferParams.c_str());
    }
       
    if (request->getParams()->isQueryParameterSet("baud")) 
    {
        //newSettings.baudRate = request->getParam("baud", true)->value().toInt();
        std::string paramName = "baud";
        request->getParams()->getQueryParameter(paramName, bufferParams);
        saveSettings.setBaudRate(std::stoi(bufferParams));
    }
        
    if (request->getParams()->isQueryParameterSet("transPort")) 
    {
        //newSettings.transparentPort = request->getParam("transPort", true)->value().toInt();
        std::string paramName = "transPort";
        request->getParams()->getQueryParameter(paramName, bufferParams);
        saveSettings.setTransparentPort(std::stoi(bufferParams));
    }
       
    if (request->getParams()->isQueryParameterSet("commandPort"))
    {
        //newSettings.commandPort = request->getParam("commandPort", true)->value().toInt();
        std::string paramName = "commandPort";
        request->getParams()->getQueryParameter(paramName, bufferParams);
        saveSettings.setCommandPort(std::stoi(bufferParams));
    }

}

void WebServer::handle404(httpsserver::HTTPRequest * request, httpsserver::HTTPResponse * response) 
{

    discardRequestBody(request);
    response->setStatusCode(404);
    response->setStatusText("Not Found");
    response->setHeader("Content-Type", "text/html");
    response->println("<!DOCTYPE html>");
    response->println("<html>");
    response->println("<head><title>Not Found</title></head>");
    response->println("<body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body>");
    response->println("</html>");
}

void WebServer::discardRequestBody(httpsserver::HTTPRequest * request) 
{
    byte buf[16];
    while(!request->requestComplete()) {
        request->readBytes(buf, 16);
    }
}
