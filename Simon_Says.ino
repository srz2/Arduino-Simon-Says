//Pins for the Display LEDs
#define LED_RED     2
#define LED_GREEN   3
#define LED_YELLOW  4
#define LED_BLUE    5

//Pins for the RGB Values for the Control LED
#define LED_CTRL_B  11
#define LED_CTRL_G  12
#define LED_CTRL_R  13

//Analog Pins Acting as Digital
#define dip_D_0   8
#define dip_1   15
#define dip_2   16
#define dip_3   17
#define dip_4   18
#define dip_5   19
#define dip_D_6  6
#define dip_D_7  7

//
#define kLED_UNK     0
#define kLED_RED     1
#define kLED_GREEN   2
#define kLED_YELLOW  3
#define kLED_BLUE    4

#define MAX_ROUNDS  32 //2 ^ 5

int maxRound;

int m_queue[MAX_ROUNDS];      //Queue Generated by Ardino
int m_index;

int i_queue[MAX_ROUNDS];      //Queue representing input from user
int i_index;

bool didStartup;              //Startup flag

unsigned int currentRound;    //Index for current round

long my_pow(int base, int power)
{
  if ( power <= 0 )
    return 1;
    
  return base * my_pow( base, power-1 );
}

void setup() 
{
  //Seed random number
  randomSeed(analogRead(0));

  //Init Variables
  didStartup = false;
  currentRound = 0;
  m_index = 0;
  i_index = 0;

  //Set User LEDS to OUTPUT
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN , OUTPUT);
  pinMode(LED_RED   , OUTPUT);
  pinMode(LED_BLUE  , OUTPUT);
  
  //Set Control LED to OUTPUT
  pinMode(LED_CTRL_R  , OUTPUT);
  pinMode(LED_CTRL_G  , OUTPUT);
  pinMode(LED_CTRL_B  , OUTPUT);

  //Set Dip Pins Position
  pinMode(dip_D_0, INPUT);
  pinMode(dip_1, INPUT);
  pinMode(dip_2, INPUT);
  pinMode(dip_3, INPUT);
  pinMode(dip_4, INPUT);
  pinMode(dip_5, INPUT);
  pinMode(dip_D_6, INPUT);
  pinMode(dip_D_7, INPUT);



  //Determine Level Number
  maxRound = 0;
  int bits[8];
  bits[0] = digitalRead(dip_D_0);
  bits[6] = digitalRead(dip_D_6);
  bits[7] = digitalRead(dip_D_7);

  bits[1] = analogRead(dip_1) > 1000 ? true : false;
  bits[2] = analogRead(dip_2) > 1000 ? true : false;
  bits[3] = analogRead(dip_3) > 1000 ? true : false;
  bits[4] = analogRead(dip_4) > 1000 ? true : false;
  bits[5] = analogRead(dip_5) > 1000 ? true : false;
  
  //Start Serial Port for Debug message
  Serial.begin(9600);
  Serial.print("Started Simon Says\n");
  Serial.print("------------------\n");
  delay(250);

    //Calculate Level
  for(int c = 0; c < 8; c++)
  {
    if(bits[c] == true)
    {
      int val = my_pow((double)2, (double)c);
      maxRound += val;
    }
  }

  Serial.print("Running Max Rounds: ");
  Serial.println(maxRound);
}

//Sets the RGB values of the Control LED
void setColor(int r, int g, int b)
{
  analogWrite(LED_CTRL_R, 255 - r);
  analogWrite(LED_CTRL_G, 255 - g);
  analogWrite(LED_CTRL_B, 255 - b);
}

//Sets Control LED to Red
void setControlRed()
{
  setColor(255,0,0);
}

//Sets Control LED to Green
void setControlGreen()
{
  setColor(0,0,255);
}

//Sets Control LED to Blue
void setControlBlue()
{
  setColor(0,255,0);
}

//Sets Control LED to Yellow
void setControlYellow()
{
  setColor(255,0,255);
}

//Sets Control LED to White
void setControlWhite()
{
  setColor(255,255,255);
}

//Sets Control LED to Off
void setControlOff()
{
  setColor(0,0,0);
}

//Turns Yellow Display LED On/Off
void ledYellow(bool isOn)
{
  pinMode(LED_YELLOW, OUTPUT);
  if(isOn)
    digitalWrite(LED_YELLOW, HIGH);
  else
    digitalWrite(LED_YELLOW, LOW);
}

//Turns Green Display LED On/Off
void ledGreen(bool isOn)
{
  pinMode(LED_GREEN, OUTPUT);
  if(isOn)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);
}

//Turns Red Display LED On/Off
void ledRed(bool isOn)
{
  pinMode(LED_RED, OUTPUT);
  if(isOn)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);
}

//Turns Blue Display LED On/Off
void ledBlue(bool isOn)
{
  pinMode(LED_BLUE, OUTPUT);
  if(isOn)
    digitalWrite(LED_BLUE, HIGH);
  else
    digitalWrite(LED_BLUE, LOW);
}

//Turns All Displays LEDs Off
void allDisplayLEDOff()
{
  ledYellow(false);
  ledGreen(false);
  ledRed(false);
  ledBlue(false);
}

//Turns All Displays LEDs On
void allDisplayLEDOn()
{
  ledYellow(true);
  ledGreen(true);
  ledRed(true);
  ledBlue(true);
}

//Turns all LEDs Off
void allLEDOff()
{
  allDisplayLEDOff();
  setControlOff();
}

//Start up sequence for a new game
void startUp()
{
  //Turn off all Leds
  allLEDOff();
    
  //Turn Control White
  setColor(255,255,255);
}

//Game Lost Sequence
void lostGame()
{
  Serial.print("You lost the game!\n");
  
  setControlRed();
  allDisplayLEDOn();

  while(1)
    ;
}

//Game Won Sequence
void wonGame()
{
  Serial.print("You won the game!\n");
  
  setControlGreen();
  allDisplayLEDOn();

  while(1)
    ;
}

//Gets a new randomly generated LED
int getNewLED()
{
  int index = random(1,5);
  int newLED = kLED_UNK;
  switch(index)
  {
    case 1:
      newLED = kLED_RED;
      break;
    case 2:
      newLED = kLED_GREEN;
      break;
    case 3:
      newLED = kLED_YELLOW;
      break;
    case 4:
      newLED = kLED_BLUE;
      break;
    default:
      newLED = kLED_UNK;
      break;
  }

  return newLED;
}

//Outputs an LED Color to the Serial Terminal
void printLED(int led)
{
  switch(led)
  {
    case kLED_RED:
      Serial.print("Red\n");
      break;
    case kLED_GREEN:
      Serial.print("Green\n");
      break;
    case kLED_YELLOW:
      Serial.print("Yellow\n");
      break;
    case kLED_BLUE:
      Serial.print("Blue\n");
      break;
    default:
      Serial.print("Unknown\n");
      break;
  }
}

//Turns on off all display leds and then turns on the one led passed
void displayLED(int led)
{
  allDisplayLEDOff();
  switch(led)
  {
    case kLED_RED:
      ledRed(true);
      break;
    case kLED_GREEN:
      ledGreen(true);
      break;
    case kLED_YELLOW:
      ledYellow(true);
      break;
    case kLED_BLUE:
      ledBlue(true);
      break;
    default:
      Serial.print("Display LED Unknown\n");
      break;
  }
}

//Sets the I/O of the Display LEDs
void setDisplayLEDS(int io)
{
  if(io == OUTPUT)
    setControlYellow();
  else
    setControlBlue();

  pinMode(LED_YELLOW, io);
  pinMode(LED_GREEN , io);
  pinMode(LED_RED   , io);
  pinMode(LED_BLUE  , io);
}

//Display the contents of a passed queue
void displayQueue(int queue[], int q_size)
{
  Serial.print("Queue Size: ");
  Serial.print(q_size);
  Serial.print("\n");
  
  for(int c = 0; c < q_size; c++)
  {
    Serial.print("Displaying ");
    int topLED = queue[c];
    printLED(topLED);
    displayLED(topLED);
    delay(850);
    allDisplayLEDOff();
    delay(250);
  }
}

//Get Input from user buttons
int getInputFromUser()
{
  //Holders for each input
  bool input_red    = 0;
  bool input_green  = 0;
  bool input_yellow = 0;
  bool input_blue   = 0;

  setDisplayLEDS(INPUT);

  //Query the switch status and hold here until one is pressed
  do
  {
    input_red = digitalRead(LED_RED);
    input_green = digitalRead(LED_GREEN);
    input_yellow = digitalRead(LED_YELLOW);
    input_blue = digitalRead(LED_BLUE);
  }while(!input_red && !input_green && !input_yellow && !input_blue);

  //Determine which button was pressed
  int returnColor = kLED_UNK;
  if(input_red)
    returnColor =  kLED_RED;
  else if(input_green)
    returnColor =  kLED_GREEN;
  else if(input_yellow)
    returnColor =  kLED_YELLOW;
  else if(input_blue)
    returnColor =  kLED_BLUE;
  else
    returnColor =  kLED_UNK;

  //Query the switch status until all is released
  do
  {
    //Serial.print("Waiting for release\n");
    input_red = digitalRead(LED_RED);
    input_green = digitalRead(LED_GREEN);
    input_yellow = digitalRead(LED_YELLOW);
    input_blue = digitalRead(LED_BLUE);
  }while(input_red || input_green || input_yellow || input_blue);

  return returnColor;
}

//Compare the last value of the queue
bool compareLast(int internal[], int index_internal, int inputted[], int index_inputted)
{
  Serial.print("Indexes ");
  Serial.print(index_internal);
  Serial.print(" ---- ");
  Serial.print(index_inputted);
  Serial.print("\n");
  
  int i1 = index_internal - 1;
  int i2 = index_inputted - 1;
  
  int led1 = internal[i1];
  int led2 = inputted[i2];

  Serial.println("Comparing");
  Serial.print("LED 1-");
  printLED(led1);
  Serial.print(" at index ");
  Serial.println(i1);
  Serial.print("LED 2-");
  printLED(led2);
  Serial.print(" at index ");
  Serial.println(i2);
  
  if(led1 == led2)
    return true;
  else
    return false;
}

void loop() 
{ 
  //Do Start Up
  if(!didStartup)
  {
    Serial.print("Start up...");
    didStartup = true;
    startUp();
    Serial.print("Done\n");
  }
  
  //Display Round information
  Serial.print("\n\nStarting new round...Round ");
  currentRound++;
  Serial.println(currentRound);

  if(currentRound > MAX_ROUNDS)
  {
    Serial.print("Overflown Max Rounds-");
    Serial.print(currentRound);
    Serial.print("/");
    Serial.println(MAX_ROUNDS);
    lostGame();
  }

  if(currentRound > maxRound)
  {
    Serial.print("Overflown Calculated Rounds-");
    Serial.print(currentRound);
    Serial.print("/");
    Serial.println(maxRound);
    lostGame();
  }

  //Get new led, add it to queue, and display the queue
  Serial.print("Getting new LED-");
  setDisplayLEDS(OUTPUT);
  delay(1000);
  int newLED = getNewLED();
  printLED(newLED);
  m_queue[m_index++] = newLED;
  displayQueue(m_queue, m_index);

  //Clear Leds and input queue
  allDisplayLEDOff();
  while(i_index > 0)
    i_queue[i_index--] = kLED_UNK;

  //Get input from user
  Serial.print("Getting input from user\n");
  bool lastIsSame = false;
  setDisplayLEDS(INPUT);
  for(int index = 1; index < currentRound + 1; index++)
  {
    Serial.print("Wait for input ");
    Serial.print(index);
    Serial.print("/");
    Serial.print(currentRound);
    Serial.print(" ---> ");
    
    int inputLED = getInputFromUser();
    printLED(inputLED);
    
    i_queue[i_index++] = inputLED;
    
    lastIsSame = compareLast(m_queue, index, i_queue, index);
    if(!lastIsSame)
      break;
    else
      lastIsSame = true;
  }

  if(!lastIsSame)
  {
    lostGame();
  }

  if(currentRound >= maxRound || currentRound >= MAX_ROUNDS)
  {
    wonGame();
  }
}



