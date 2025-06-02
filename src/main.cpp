#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>



int red = 27;
int green = 26;
int blue = 14;
int signalR = 2;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>

<html>
    <head>
        <title>ESP Server</title>
        
        <style>
            html {font-family: Arial; display: inline-block; text-align: center;}
            h2 {font-size: 3.0rem;} p {font-size: 3.0rem;} body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
            .switch{position: relative; display: inline-block; width: 120px; height: 68px;}
            .switch input {display: none} .slider {position: absolute; top: 0; left: 0; right: 0; bottom:
            0; background-color: #ccc; border-radius: 6px} .slider:before {position: absolute; content: ""; height:52px;
            width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
            input:checked+.slider {background-color: #b30000} input:checked+.slider:before {-webkit-transform: translateX(52px);
            -ms-transform: translateX(52px); transform: translateX(52px)}
        </style>
        
    </head>
        <body>
            <h2>Esp Web Serwer</h2>
            %BUTTONPLACEHOLDER%
            <script>
                function toggleCheckbox(element){
                    var xhr = new XMLHttpRequest();
                    if(element.checked){xhr.open("GET","/update?output="+element.id+"&state=1",true);}
                    else {xhr.open("GET", "/update?output="+element.id+"&state=0",true);}
                    xhr.send();
                }
            </script>

        </body>
</html>
 


)rawliteral";

const char aut_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Login Page</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f4;
      margin: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    .login-container {
      background-color: #fff;
      border-radius: 8px;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
      padding: 20px;
      width: 300px;
      text-align: center;
    }

    .login-container h2 {
      margin-bottom: 20px;
      color: #333;
    }

    .login-form input {
      width: 100%;
      padding: 10px;
      margin-bottom: 15px;
      border: 1px solid #ccc;
      border-radius: 4px;
      box-sizing: border-box;
    }

    .login-form button {
      background-color: #4caf50;
      color: #fff;
      padding: 10px;
      border: none;
      border-radius: 4px;
      cursor: pointer;
      width: 100%;
    }

    .login-form button:hover {
      background-color: #45a049;
    }
  </style>
</head>
<body>

<div class="login-container">
  <h2>Login</h2>
  <form id="form1"class="login-form">
    <input type="text" id="Username" placeholder="Username" required>
    <input type="password" id="Password" placeholder="Password" required>
    <button type="submit" >Login</button>
  </form>
</div>

<script>
    document.getElementById('form1').addEventListener('submit', function(event) {
        event.preventDefault();
        
        let username = document.getElementById('Username').value;
        let password = document.getElementById('Password').value;

        // ??????? URL ? ???????????
        let url = "/?username=" + encodeURIComponent(username) + 
                 "&password=" + encodeURIComponent(password);
        
        // ?????????? GET-?????? (????? ???????? ?? POST ??? ?????????????)
        fetch(url)
            .then(response => {
                if (response.ok) {
                    // ???? ????? ???????? (200-299), ?????????? ?????
                    return response.text();
                }
                throw new Error('Network response was not ok');
            })
            .then(html => {
                // ???????? ?????????? ???????? ?? ?????????? HTML
                document.open();
                document.write(html);
                document.close();
            })
            .catch(error => {
                console.error('Error:', error);
                alert("Login failed. Please try again.");
            });
    });
</script>

</body>
</html>
 


)rawliteral";

const char arr_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
    <title>Real-Time Array Display</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            max-width: 600px;
            margin: 0 auto;
            padding: 20px;
        }
        h1 {
            color: #333;
        }
        #array-container {
            margin: 20px 0;
            padding: 15px;
            border: 1px solid #ddd;
            border-radius: 5px;
            background-color: #f9f9f9;
        }
        .update-info {
            font-size: 0.9em;
            color: #666;
            margin-top: 10px;
        }
    </style>
</head>
<body>
    <h1>Real-Time Array Display</h1>
    <div id="array-container">
        <p>Current array: <span id="array-content">Loading...</span></p>
        <p class="update-info">Last updated: <span id="last-updated">-</span></p>
    </div>

    <script>
        const socket = new WebSocket(`ws://${window.location.hostname}/ws`);

        socket.onopen = function(e) {
            console.log("WebSocket connection established");
            document.getElementById('array-content').textContent = "Waiting for data...";
        };

        socket.onmessage = function(event) {
            const data = JSON.parse(event.data);
            document.getElementById('array-content').textContent = JSON.stringify(data.array);
            document.getElementById('last-updated').textContent = new Date().toLocaleTimeString();
        };

        socket.onerror = function(error) {
            console.error("WebSocket error:", error);
            document.getElementById('array-content').textContent = "Connection error";
        };

        socket.onclose = function(event) {
            if (event.wasClean) {
                console.log(`Connection closed cleanly, code=${event.code}, reason=${event.reason}`);
            } else {
                console.log('Connection died');
                document.getElementById('array-content').textContent = "Connection lost - trying to reconnect...";
                setTimeout(() => window.location.reload(), 2000);
            }
        };
    </script>
</body>
</html>
)rawliteral";

int arrSize = 30;
double* arr = new double[arrSize];

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


void notifyClients() {
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.createNestedArray("array");
    
    for(int i = 0; i< arrSize;i++) {
        array.add(arr[i]);
    }
    
    String json;
    serializeJson(doc, json);
    ws.textAll(json);
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if(type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        // ??? ??????????? ????? ?????????? ??????? ??????
        notifyClients();
    } else if(type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
    }
}

String outputState(int output){
    if(digitalRead(output)){
        return "checked";
    }
    else{
        return "";
    }
}

String processor(const String&var){
    if(var == "BUTTONPLACEHOLDER"){
        String buttons = "";
        buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) +"/><span class=\"slider\"></span></label>";
        return buttons;
    }
    return String();
}


const String RealUsername = "A", RealPassword = "123";

class CaptiveRequestHandler : public AsyncWebHandler{

    private:

    public:
    CaptiveRequestHandler() {
        for(int i =0; i< arrSize;i++){
            arr[i] = 0;
        }
    }

    virtual ~CaptiveRequestHandler() {}
    bool canHandle(AsyncWebServerRequest *request) {
        if(request->url() == "/ws") {
        return false;
        }
  
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request){
        Serial.println(1);
        if(request->url() == "/"){

            String Username, Password;

            char* PARAM_INPUT_1 = "username";
            char* PARAM_INPUT_2 = "password";
            Serial.println(2);
            if(request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)){
                Username = request->getParam(PARAM_INPUT_1)->value();
                Password = request->getParam(PARAM_INPUT_2)->value();
                Serial.println(Username);
                Serial.println(Password);
                if(Username.equals( RealUsername) && Password.equals( RealPassword)){
                    request->send_P(200, "text/html", index_html,processor);
                }
                else{
                        Serial.println(Username.equals( RealUsername) );
                        Serial.println(Password.equals( RealPassword));
                request->send_P(201, "text/html", aut_html);
                }

            }
            else{
                Serial.println(request->hasParam(PARAM_INPUT_1));
                Serial.println(request->hasParam(PARAM_INPUT_2));

                request->send_P(201, "text/html", aut_html);
            }
        }
        else if(request->url() == "/update"){

            char* PARAM_INPUT_1 = "output";
            char* PARAM_INPUT_2 = "state";

            Serial.println(request->url());
            String inputMessage1, inputMessage2;
            if(request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)){
                inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
                inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
                digitalWrite(inputMessage1.toInt(),inputMessage2.toInt());
            } else{
                inputMessage1 = "No message sent";
                inputMessage2 = "No message sent";
            }

            request-> send(200, "text/plain", "OK");
        }
        else{
            Serial.println(4);
            
            request-> send(404, "text/plain", "404");
        }


        Serial.println(request->url());
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  WiFi.softAP("esp-captive_Mikerin");

    

    ws.onEvent(onWsEvent);

    server.addHandler(&ws);

    
    server.addHandler(new CaptiveRequestHandler());
    server.on("/ws", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", arr_html);
});
    
    server.begin();
}

int counter = 0;
int arrPoint = 0;

void loop() {

    counter++;

    if(counter > 20){
        counter = 0;
        arr[arrPoint] ++;
        arrPoint = (arrPoint + 1) % arrSize;

        notifyClients();

    }
  
  delay(20);
}
