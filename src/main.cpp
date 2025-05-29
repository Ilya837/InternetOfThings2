#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>



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
            
            let username = form.Username.value;
            let password = form.Password.value;

            var xhr = new XMLHttpRequest();
            xhr.open("POST", "/?username="+username+"&password="+password,true);
            

            
        if (xhr.status === 200) {
        

            setTimeout(function() {
                window.location.href = 'http://example.com/dashboard';
            }, 1000);  // Перенаправление через 1 секунду
        }
        });
</script>

</body>
</html>
 


)rawliteral";

AsyncWebServer server(80);
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

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

const String RealUsername = "A", RealPassword = "123";

class CaptiveRequestHandler : public AsyncWebHandler{
    public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}
    bool canHandle(AsyncWebServerRequest *request) {
        
  
        return true;}
    void handleRequest(AsyncWebServerRequest *request){
        Serial.println(1);
        if(request->url() == "/"){

            String Username, Password;
            Serial.println(2);
            if(request->hasParam("username") && request->hasParam("password")){
                Username = request->getParam("username")->value();
                Password = request->getParam("password")->value();
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
                Serial.println(request->hasParam("username"));
                Serial.println(request->hasParam("password"));

                request->send_P(201, "text/html", aut_html);
            }
        }
        else if(request->url() == "/update"){

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
server.addHandler(new CaptiveRequestHandler());
server.begin();

    
}

void loop() {
  
  delay(20);
}
