// Watch video here: https://www.youtube.com/watch?v=M4mVDnlnzSA

// UIP is a proper library for Arduino Nano Ethernet shield
// NOTE: UIPEthernet library is not needed if you are using Arduino UNO/Duemilanove/Mega/etc.
// UIPEthernet library is used for Arduino Nano Ethernet Shield

#include <UIPEthernet.h> // Used for Ethernet

// **** ETHERNET SETTING ****
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x78, 0xEE  };                                       
IPAddress ip(192, 168, 1, 115);                        
EthernetServer server(80);
String readString;

void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client) 
  {  
    Serial.println("-> New Connection");

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply

        if (readString.length() < 20) {
          //store characters to string
          readString += c;
          Serial.println(readString); //print to serial monitor for debuging
        }
        
        if (c == '\n' && currentLineIsBlank) 
        {
          Serial.println(readString);
          
          if(readString.indexOf("?ac") >0)//checks for on
          {
            digitalWrite(7, HIGH);    // set pin 4 high
            Serial.println("Led On");
            client.println("<html><title>Lamba Yandı!</title><body><h3>Lamba yandı ! </h3></body>");
          }
          else{
            if(readString.indexOf("?kapat") > 0)//checks for off
            {
              digitalWrite(7, LOW);    // set pin 4 low
              Serial.println("Led Off");
              client.println("<html><title>Lamba Söndü!</title><body><h3>Lamba Söndü ! </h3></body>");
            }
          }
          readString="";
          break;
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      } 
    }

    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("   Disconnected\n");
  }
}
