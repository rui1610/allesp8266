#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "communication.h"
#include <ArduinoJson.h>

// Found this at https://4x5mg.net/2018/06/10/controlling-somfy-blinds-with-esp8266/

/*   This sketch allows you to emulate a Somfy RTS or Simu HZ remote.
   If you want to learn more about the Somfy RTS protocol, check out https://pushstack.wordpress.com/somfy-rts-protocol/
   
   The rolling code will be stored in EEPROM, so that you can power the Arduino off.
   
   Easiest way to make it work for you:
    - Choose a remote number
    - Choose a starting point for the rolling code. Any unsigned int works, 1 is a good start
    - Upload the sketch
    - Long-press the program button of YOUR ACTUAL REMOTE until your blind goes up and down slightly
    - send 'p' to the serial terminal
  To make a group command, just repeat the last two steps with another blind (one by one)
  
  Then:
    - m, u or h will make it to go up
    - s make it stop
    - b, or d will make it to go down
    - you can also send a HEX number directly for any weird command you (0x9 for the sun and wind detector for instance)
*/

String header;
const char *ssid = WIFI_SSD;
const char *password = WIFI_PASSWD;

// Create an instance of the server
// specify the port to listen on as an argument
ESP8266WebServer server(80);
String getSomfyCommandFromParameters()
{
  String command;
  if (server.arg("command") == "")
  { //Parameter not found
    command = "";
  }
  else
  {                                  //Parameter found
    command = server.arg("command"); 
  }
  
  return command;
}

somfyRemote getSomfyRemoteFromParameters()
{
  int remoteId;
  if (server.arg("remote") == "")
  { //Parameter not found
    remoteId = 0;
  }
  else
  { //Parameter found
    String remoteString = server.arg("remote");
    remoteId = remoteString.toInt();
  }
  somfyRemote myRemote;

  switch (remoteId)
  {
  case 1:
    myRemote = remote1;
    break;
  case 2:
    myRemote = remote2;
    break;
  case 3:
    myRemote = remote3;
    break;
  case 4:
    myRemote = remote4;
    break;
  case 5:
    myRemote = remote5;
    break;
  case 6:
    myRemote = remote6;
    break;
  case 7:
    myRemote = remote7;
    break;
  case 8:
    myRemote = remote8;
    break;
  case 9:
    myRemote = remote9;
    break;
  default:
    break;
  }

  return myRemote;
}
void handleSomfy()
{ //Handler for the rooth path

  String message = "Planning to send remote a command";
  Serial.println(message);
  message = "- step 1";
  Serial.println(message);
  somfyRemote remote = getSomfyRemoteFromParameters();

  message = "- step 2";
  Serial.println(message);
  String command = getSomfyCommandFromParameters();
  message = "- step 3";
  Serial.println(message);



  if (command == "U" or command == "u")
  {
    BuildFrame(frame, UP, remote);
    command = "up";
  }
  if (command == "D" or command == "d")
  {
    BuildFrame(frame, DOWN, remote);
    command = "down";
  }
  if (command == "S" or command == "s")
  {
    BuildFrame(frame, STOP, remote);
    command = "stop";
  }
  if (command == "P" or command == "p")
  {
    BuildFrame(frame, PROG, remote);
    command = "program";
  }

  SendCommand(frame, 2);
  for (int i = 0; i < 2; i++)
  {
    SendCommand(frame, 7);
  }
  message = "Sent remote " + String(remote.number) + " the command '" + command +"'";
  server.send(200, "text/plain", message);
  Serial.println(message);
  
}

void getCurrentEEPROMValues(somfyRemote myRemote)
{
  int myEEPROM_ADDRESS = getEEPromAddressRemote(myRemote);

  EEPROM.get(myEEPROM_ADDRESS, rollingCode);
  Serial.print("Current rolling code for remote >" + String(myRemote.number) + "< with id  >" + String(myRemote.id) + "<: ");
  Serial.println(rollingCode);
}

void setup()
{
  initializeRemotes();

  Serial.begin(115200);
  Serial.println(" ");
  Serial.println("Starting Heimfunker");

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.on("/somfy", handleSomfy);

  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

  pinMode(transmitPin, OUTPUT);
  digitalWrite(transmitPin, LOW);

  EEPROM.begin(4095);

  getCurrentEEPROMValues(remote1);
  getCurrentEEPROMValues(remote2);
  getCurrentEEPROMValues(remote3);
  getCurrentEEPROMValues(remote4);
  getCurrentEEPROMValues(remote5);
  getCurrentEEPROMValues(remote6);
  getCurrentEEPROMValues(remote7);
  getCurrentEEPROMValues(remote8);
  getCurrentEEPROMValues(remote9);
}

void loop(void)
{

  server.handleClient();
}
