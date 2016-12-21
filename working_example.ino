// Example sketch to read the ID from an Addicore 13.56MHz RFID tag
// as found in the RFID AddiKit found at: 
// http://www.addicore.com/RFID-AddiKit-with-RC522-MIFARE-Module-RFID-Cards-p/126.htm
#include <AddicoreRFID.h>
#include <SPI.h>
#define  uchar unsigned char
#define uint  unsigned int
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
int i;

void LightUpHigh(int port)
{
  for(i = 0; i < 4; i++){
   digitalWrite(port,HIGH);
   delay(200);
   digitalWrite(port,LOW);
   delay(200);
   digitalWrite(port,HIGH);
  }
}

void LightUpScoreOne(int score)
{
   for(i = 0; i < 4; i++){
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    delay(200);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    delay(200);
   }
    if(playerOneScore == 1)
      digitalWrite(2,HIGH);
    if(playerOneScore == 2){
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
    } 
    if(playerOneScore >= 3){
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
      digitalWrite(4,HIGH);
   }
}

void LightUpScoreTwo(int score)
{
   for(i = 0; i < 4; i++){
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
    delay(200);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    delay(200);
   }
    if(playerTwoScore == 1)
      digitalWrite(5,HIGH);
    if(playerTwoScore == 2){
      digitalWrite(5,HIGH);
      digitalWrite(6,HIGH);
    }  
    if(playerTwoScore >= 3){
      digitalWrite(5,HIGH);
      digitalWrite(6,HIGH);
      digitalWrite(7,HIGH);
   }
}

void LightUpTie()
{
  for(i = 0; i < 4; i++){
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
    delay(200);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    delay(200);
   }
   if(playerOneScore == 1)
      digitalWrite(2,HIGH);
   if(playerOneScore == 2){
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
   } 
   if(playerOneScore >= 3){
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
      digitalWrite(4,HIGH);
   }
   if(playerTwoScore == 1)
      digitalWrite(5,HIGH);
   if(playerTwoScore == 2){
      digitalWrite(5,HIGH);
      digitalWrite(6,HIGH);
    }  
   if(playerTwoScore >= 3){
      digitalWrite(5,HIGH);
      digitalWrite(6,HIGH);
      digitalWrite(7,HIGH);
   }
}


void setup() {                
   Serial.begin(9600);                        // RFID reader SOUT pin connected to Serial RX pin at 9600bps 
// start the SPI library:
  SPI.begin();
pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
digitalWrite(chipSelectPin, LOW);           // Activate the RFID reader
pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
digitalWrite(NRSTPD, HIGH);
  myRFID.AddicoreRFID_Init();      
pinMode(7,OUTPUT);
//digitalWrite(7,HIGH);
pinMode(6,OUTPUT);
//digitalWrite(6,HIGH);
pinMode(5,OUTPUT);
digitalWrite(5,LOW); 
pinMode(4,OUTPUT);
//digitalWrite(4,HIGH);
pinMode(3,OUTPUT);
//digitalWrite(3,HIGH);
pinMode(2,OUTPUT);
//digitalWrite(2,HIGH); 
}

void loop()
{
  
  uchar i, tmp, checksum1;
  uchar status;
        uchar str[MAX_LEN];
        uchar RC_size;
        uchar blockAddr;  //Selection operation block address 0 to 63
        String mynum = "";
        //str[1] = 0x4400;
//Find tags, return tag type
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str); 
if (status == MI_OK)
  {
          Serial.println("RFID tag detected");
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
if (checksum1 == 209) { //reset  //155 card, 54 fob
      Serial.println("RESETTING");
// val = 0;
      playerOneCard = "";
      playerTwoCard = "";
      playerOneScore = 0;
      playerTwoScore = 0;
//winner = "";
      result = "reset";
      gameOver = false;
      LightUpTie(); 
    } else if (!gameOver){ 
if (checksum1 == 87){
        Serial.println("Player one selects rock.");
        LightUpScoreOne(playerOneScore);
        playerOneCard = "rock";
      } else if (checksum1 == 253){
        Serial.println("Player one selects paper.");
        LightUpScoreOne(playerOneScore);
        playerOneCard = "paper";
      } else if (checksum1 == 60){
        Serial.println("Player one selects scissors.");
        LightUpScoreOne(playerOneScore);
        playerOneCard = "scissors";
      } else if (checksum1 == 25){
        Serial.println("Player two selects rock.");
        LightUpScoreTwo(playerTwoScore);
        playerTwoCard = "rock";
      } else if (checksum1 == 250){
        Serial.println("Player two selects paper.");
        LightUpScoreTwo(playerTwoScore);
        playerTwoCard = "paper";
      } else if (checksum1 == 217){
        Serial.println("Player two selects scissors.");
        LightUpScoreTwo(playerTwoScore);
        playerTwoCard = "scissors";
      }  
    } 
  }
//val = 0;
if (playerOneCard != "" && playerTwoCard != ""){
startGame();
  }
        myRFID.AddicoreRFID_Halt();      //Command tag into hibernation              
}
void startGame(){
if (playerOneCard == "rock" && playerTwoCard == "rock"){
    Serial.println("tie");
    LightUpTie();
    result = "tie";
    playerOneCard = "";
    playerTwoCard = "";
  } else if (playerOneCard == "rock" && playerTwoCard == "paper"){
    Serial.println("Player Two Wins!");
    playerTwoScore++;
    LightUpScoreTwo(playerTwoScore);
    result = "playerTwo";
    playerOneCard = "";
    playerTwoCard = "";
  } else if (playerOneCard == "rock" && playerTwoCard == "scissors"){
    Serial.println("Player One Wins!");
    playerOneScore++;
    LightUpScoreOne(playerOneScore);
    result = "playerOne";
    playerOneCard = "";
    playerTwoCard = "";
  } else if (playerOneCard == "paper" && playerTwoCard == "rock"){
    Serial.println("Player One Wins!");
    playerOneScore++;
    LightUpScoreOne(playerOneScore);
    result = "playerOne";
    playerOneCard = "";
    playerTwoCard = "";
  } else if (playerOneCard == "paper" && playerTwoCard == "paper"){
    Serial.println("tie");
    LightUpTie();
    result = "tie";
    playerOneCard = "";
    playerTwoCard = "";
  } else if (playerOneCard == "paper" && playerTwoCard == "scissors"){
    Serial.println("Player Two Wins!");
    playerTwoScore++;
    LightUpScoreTwo(playerTwoScore);
    result = "playerTwo";
    playerOneCard = "";
    playerTwoCard = "";
  } else if (playerOneCard == "scissors" && playerTwoCard == "rock"){
    Serial.println("Player Two Wins!");
    playerTwoScore++;
    LightUpScoreTwo(playerTwoScore);
    result = "playerTwo";
    playerOneCard = "";
    playerTwoCard = "";
  } else if (playerOneCard == "scissors" && playerTwoCard == "paper"){
    Serial.println("Player One Wins!");
    playerOneScore++;
    LightUpScoreOne(playerOneScore);
    result = "playerOne";
    playerOneCard = "";
    playerTwoCard = "";
  } else if (playerOneCard == "scissors" && playerTwoCard == "scissors"){
    Serial.println("tie");
    LightUpTie();
    result = "tie";
    playerOneCard = "";
    playerTwoCard = "";
  }
  playerOneCard = "";
  playerTwoCard = "";
//displayResults();
}


