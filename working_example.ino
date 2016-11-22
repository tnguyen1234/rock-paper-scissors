// Example sketch to read the ID from an Addicore 13.56MHz RFID tag
// as found in the RFID AddiKit found at: 
// http://www.addicore.com/RFID-AddiKit-with-RC522-MIFARE-Module-RFID-Cards-p/126.htm

#include <AddicoreRFID.h>
#include <SPI.h>

#define	uchar	unsigned char
#define	uint	unsigned int

uchar fifobytes;
uchar fifoValue;

AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module

/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 10;
const int NRSTPD = 9;
String card = "";
String ucard0 = "";
String ucard1 = "";
String ucard2 = "";
String ucard3 = "";
uint32_t myCard;
int currentCard;


String playerOneCard = "";
String playerTwoCard = "";
int playerOneScore = 0;
int playerTwoScore = 0;
String result = "";
bool gameOver = false;



//Maximum length of the array
#define MAX_LEN 16

void setup() {                
   Serial.begin(9600);                        // RFID reader SOUT pin connected to Serial RX pin at 9600bps 
 
  // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
    digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
    digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();  
}

void loop()
{
  	uchar i, tmp, checksum1;
	uchar status;
        uchar str[MAX_LEN];
        uchar RC_size;
        uchar blockAddr;	//Selection operation block address 0 to 63
        String mynum = "";

        str[1] = 0x4400;
	//Find tags, return tag type
	status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);	
	if (status == MI_OK)
	{
          Serial.println("RFID tag detected");
          /*Serial.print("Tag Type:\t\t");
          uint tagType = str[0] << 8;
          tagType = tagType + str[1];
          switch (tagType) {
            case 0x4400:
              Serial.println("Mifare UltraLight");
              break;
            case 0x400:
              Serial.println("Mifare One (S50)");
              break;
            case 0x200:
              Serial.println("Mifare One (S70)");
              break;
            case 0x800:
              Serial.println("Mifare Pro (X)");
              break;
            case 0x4403:
              Serial.println("Mifare DESFire");
              break;
            default:
              Serial.println("Unknown");
              break;
          } */
	}

	//Anti-collision, return tag serial number 4 bytes
	status = myRFID.AddicoreRFID_Anticoll(str);
	if (status == MI_OK)
	{
          card = str[0] + str[1] + str[2] + str[3];
          checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];

          
          myCard = (uint32_t)str[0] << 24 | (uint32_t)str[1] << 16 | (uint32_t)str[2] << 8 | (uint32_t)str[3];
          currentCard = (int)myCard;
          
          Serial.print("The tag's number is:\t");
    	    Serial.print(str[0]);
            Serial.print(" , ");
    	    Serial.print(str[1]);
            Serial.print(" , ");
    	    Serial.print(str[2]);
            Serial.print(" , ");
    	    Serial.println(str[3]);
         
         Serial.println(currentCard);

          Serial.print("Read Checksum:\t\t");
    	      Serial.println(str[4]);
          Serial.print("Calculated Checksum:\t");
            Serial.println(checksum1);

            
            // Should really check all pairs, but for now we'll just use the first
            if(str[0] == 197)                      //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
            {
                Serial.println("\nHello Craig!\n");
            } else if(str[0] == 244) {             //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
                Serial.println("\nHello Erin!\n");
            }
            Serial.println();
            delay(1000);

    if (str[0] == 1556) { //reset  //155 card, 54 fob
      Serial.println("RESETTING");
     // val = 0;
      playerOneCard = "";
      playerTwoCard = "";
      playerOneScore = 0;
      playerTwoScore = 0;
      //winner = "";
      result = "reset";
      gameOver = false;
      displayResults();
    } else if (!gameOver){
      if (str[0] == 155){
        Serial.println("Player one selects rock.");
        playerOneCard = "rock";
      } else if (str[0] == "IDpaper1"){
         Serial.println("Player one selects paper.");
        playerOneCard = "paper";
      } else if (str[0] == "IDscissors1"){
         Serial.println("Player one selects scissors.");
        playerOneCard = "scissors";
      } else if (str[0] == "IDrock2"){
         Serial.println("Player two selects rock.");
        playerTwoCard = "rock";
      } else if (str[0] == 54){
         Serial.println("Player two selects paper.");
        playerTwoCard = "paper";
      } else if (str[0] == "IDscissors2"){
         Serial.println("Player two selects scissors.");
        playerTwoCard = "scissors";
      } 
    }
  
    
	}
   //val = 0;
  if (playerOneCard != "" && playerTwoCard != ""){
    startGame();
  }
		
        myRFID.AddicoreRFID_Halt();		   //Command tag into hibernation              

}

void startGame(){
  if (playerOneCard == "rock" && playerTwoCard == "rock"){
     Serial.println("tie");
    result = "tie";
  } else if (playerOneCard == "rock" && playerTwoCard == "paper"){
    Serial.println("Player Two Wins!");
    result = "playerTwo";
  } else if (playerOneCard == "rock" && playerTwoCard == "scissors"){
    Serial.println("Player One Wins!");
    result = "playerOne";
  } else if (playerOneCard == "paper" && playerTwoCard == "rock"){
    Serial.println("Player One Wins!");
    result = "playerOne";
  } else if (playerOneCard == "paper" && playerTwoCard == "paper"){
    Serial.println("tie");
    result = "tie";
  } else if (playerOneCard == "paper" && playerTwoCard == "scissors"){
      Serial.println("Player Two Wins!");
    result = "playerTwo";
  } else if (playerOneCard == "scissors" && playerTwoCard == "rock"){
    Serial.println("Player Two Wins!");
    result = "playerTwo";
  } else if (playerOneCard == "scissors" && playerTwoCard == "paper"){
    Serial.println("Player One Wins!");
    result = "playerOne";
  } else if (playerOneCard == "scissors" && playerTwoCard == "scissors"){
    Serial.println("tie");
    result = "tie";
  }
  playerOneCard = "";
  playerTwoCard = "";
  displayResults();
}

void displayResults(){
  if(result == "playerOne") {
    playerOneScore++;
    //light current Result LED
    if (playerOneScore == 1){
      //light LED1
    } else if (playerOneScore == 2){
      //light LED1
      //light LED2
    } else if (playerOneScore == 3){
      //light LED1
      //light LED2
      //light LED3
      gameOver = true;
    }
  } else if (result == "playerTwo"){
    playerTwoScore++;
     //light current Result LED
     if (playerTwoScore == 1){
      //light LED1
    } else if (playerTwoScore == 2){
      //light LED1
      //light LED2
    } else if (playerTwoScore == 3){
      //light LED1
      //light LED2
      //light LED3
      gameOver = true;
    }
  } else if (result == "tie"){
    //light currentResult middle LED
  } else if (result == "reset"){
    //handle all resetting, all LEDS off
    //game was reset
  }
  
}


