/* INTRO
 * 
 * CAN Node Example: 
 * Created by Jesse Jongebloed for the SBM Monorepo
 * 
 * This file will be edited as questions come up from the team to demonstrate 
 * how certain things are done. 
 * Line by line comments are added to illustrate what's happening for beginners.
 * To ask how this is used or ask for a new concept to be added to the example
 * raise a github issue in the repository and @ faultfactory.
 * 
 *
 * Have fun. Drive Fast. Take Chances. Enjoy your bird. 
 * 
 * END INTRO
 */



/* Include Statements
 *  The # symbol at the start of a line defines a pre-processor directive. 
 *  These get evaluated at compile time and will add, replace or bypass code
 *  that the compiler encounters as it parses the file, depending on the 
 *  current variables in the compiler space. Note that the variables in the
 *  program and the preprocessor do not share the same space. 
 *  
 *  #include is fairly transparent in that we are including the code in the file
 *  listed in the file here.
 *  How this is done can be somewhat complex depending on the build scheme
 *  but for the moment, understand that Arduino libraries used are enclosed with
 *  the  angle brackets: < > 
 *  Local includes in your src directory, get quotes: " " 
 *  
 *  Typically this is done with files ending in .h which are headers and for
 *  the case of Arduino, this defines the names and interfaces for the library functions. 
 */
#include <CAN.h>
#include <millisDelay.h>
#include "src/can_conv/sbm_network_definition.h"


/*  Preprocessor Directives, Speed, and Serial. 
 *   
 *  We can leverage the preprocessor directives to help us with debugging
 *  or allow for different variants to be compiled.   
 *  A good example of this can be done with Serial library. 
 *  The Serial print functions in Arduino are rather slow and consumptive. 
 *  For this sketch I've wrapped them in pre-processor directives to demonstrate
 *  the time penalty for using them and allowed the CAN communications code to continue
 *  
 *  After you've read the entire file try running the code on the board with the default setting of:
 *  #define SERIAL_DEBUG_PRINTS_ON 1 
 *  You'll see the Serial console output the CAN communications as it 
 *  is by default to provide confidence that the CAN communciations operates as intended from the printouts. 
 *  Open the Serial Monitor and observe the loop count output on every iteration on the S
 *  with this configuration. 
 *  
 *  Now, try changing the code to say: 
 *  #define SERIAL_DEBUG_PRINTS_ON 0
 *  Save and upload the sketch again to the board. 
 *  The CAN code is not wrapped in these conditions so they will continue to function
 *  despite not providing Serial output. Additionally the loop count and timer output have
 *  been left to still broadcast to Serial so we can observe their output. 
 *  
 *  By comparing the loop count to the previous one, you can get a feel for the penalty of leaving this
 *  code in the sketch if you are not using it. The difference in this example is subtle but better
 *  gains are to be had by disabling the entirety of the Serial library if you are not using it.
 *  That's reccomended when the time comess. 
 *   *  
 *  Here's an example of the pre-processor directive wrapping so you know what this is referring to.
 *  
 *  #if SERIAL_DEBUG_PRINTS_ON  
 *  Serial.println("Userful but costly information sent to serial terminal").
 *  #endif    
*/
#define SERIAL_DEBUG_PRINTS_ON 1

/* Defining variables
 * Unlike Matlab and Python where you can just type a variable and it will come into existence
 * C/C++ is 'strongly typed' meaning that you need to define exactly what type of variable you 
 * are creating. 
 * 
 * In the lines below, we are creating storage for the can translation object, the IMU 
 * variables, loop counter and a timer object defined in the millsDelay header we included.
 * 
 * Limiting scope is generally best practice but is unavoidable with arduino, that can be a
 * bit difficult given that they have abstracted away the main function and only
 * provide setup() and loop(). We want these values to persist throughout each iteration
 * so they must be defined outside of the loop functions. 
 */
can_obj_sbm_network_definition_h_t can_obj;
float yaw = 0.0f;
float pitch = 0.0; 
float roll = 0.0f; 
unsigned long loopCount = 0; 
millisDelay CAN_Send_Delay; 

/* Function definition
 *  This defines a function for filling the IMU values with some nonsense data
 *  as you can see, given that the variables are in global scope, no arguments
 *  need to be passed in or out. The void keyword means that the function returns nothing.
 */
void makeRandomIMUValues()
{
  float millisFloat = (float)millis();
  yaw = 180.f + 180.0f*sin((HALF_PI+millisFloat)/TWO_PI);
  pitch = 180.f + 180.0f*sin((PI+millisFloat)/TWO_PI);
  roll = 180.f + 180.0f*sin((EULER+millisFloat)/TWO_PI);
}

/*  Unions
 *  This is complex concept in programming and actually a little dangerous but
 *  at a high level this is just looking at 64 bytes of data in two different ways
 *  to allow for two different librareis to use it. 
 *  
 *  For now, it is totally fine to look at it and say 'it works, don't touch it'.
 *  Even seasoned programmers get bit with unions. 
 *  Given the differing interfaces for our libraries, we are stuck with this for now 
 *  but can address this in other ways for the future. 
 */
union MsgData
{
  uint64_t a;   // dbcc output uses uint64_t
  uint8_t b[8]; // Adafruit CAN library wants uint8_t[8]
};



/*
 * This function will run once to set up your system. 
 */
void setup()
{
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster


  Serial.begin(115200);
  // NOTE: You'll need to get rid of this eventually
  while (!Serial); // THIS STOPS YOU FROM RUNNING UNTIL YOUR USB IS PLUGGED IN!
  Serial.println("CAN Loopback Demonstration");
  // start the CAN bus at 500 kbps
    CAN.begin(500000);

  ///////////EXAMPLE ONLY, DELETE ME FOR USE ON VEHICLE //////////
  //CAN Loopback is for validating code with a single board. Remove this for a multi-node network
  CAN.loopback();
  ///////////EXAMPLE ONLY, DELETE ME FOR USE ON VEHICLE //////////

  /* Delays 
   *  This is starting the millsDelay from the library included by <millsDelay.h>
   *  This provides non-blocking delays that don't obstruct execution
   *  like using the standard delay() function would. 
   *  
   *  When you call the standard delay() function, you will hold up all
   *  execution. By using a timer such as millisDelay, you are allowing other
   *  functionality to take place while checking for timer expiration on
   *  every loop. 
   */
  CAN_Send_Delay.start(1000); // We want to execute after every 1000 milliseconds
}

void loop()
{
  loopCount++;
  if(CAN_Send_Delay.justFinished()) // Checking the delay on each loops
  {
    // If we're in this function, that means the delay has expired and we should send a CAN message
    // This library does not allow drift and logs the actual expiration time. 
    CAN_Send_Delay.repeat(); // Call this function to set the reset the timer again. 
    Serial.println(millis());
    Serial.print("Loops Completed:");    
    Serial.println(loopCount);
    loopCount = 0;
    #if SERIAL_DEBUG_PRINTS_ON
    Serial.println("The millis() output is printed above because to demonstrate that the internal time is right, despite the serial timestamps being wrong.");
    #endif
    makeRandomIMUValues();
    #if SERIAL_DEBUG_PRINTS_ON
    Serial.println("Sending the following values through CAN");
    Serial.print("Roll:" ); Serial.println(roll);
    Serial.print("Pitch:"); Serial.println(pitch);
    Serial.print("Yaw:"  ); Serial.println(yaw);
    #endif

    /* CAN Encoding
     *  The 3 function calls below handle encoding each value into the 
     *  64 bit can message per the .dbc using the generated code from
     *  .dbcc. 
     *  
     *  It respects the bit structure, scaling and offset defined in the .dbc file
     *  and stores the data into the can_obj variable, which is also an object defined
     *  the automatically generated code. 
     *  Its datatype was created by dbcc and encompaseses the whole .dbc
     */
    encode_can_0x100_Body_Roll_deg(&can_obj,roll); 
    encode_can_0x100_Body_Pitch_deg(&can_obj,pitch);
    encode_can_0x100_Body_Yaw_deg(&can_obj,yaw);
    
    // This is the union datatype that allows us to read in uint64_t or uint8_t[8]
    MsgData msg;  

    /* This command transfer the stored message from our can_obj to the
     * local varaible (msg) using the 'a' data view in the union. 
     * the '&' symbol is actually passing the memory address because the 
     * function signature of pack_message asks for a pointer to a uint64_t
     * 0x100 is the frame id of the can message we're working with. 
    */
    pack_message(&can_obj,0x100,&msg.a);
    CAN.beginPacket(0x100,8,false);
    CAN.write(msg.b,8);
    CAN.endPacket();  

    /* After sending the message we can go ahead and encode zeros across
    * the entire storage structure to give ourselves more confidence that
    * the code is doing what we expect. This is not required. just a demonstration
    * to guaranteed we're not looking at our old values. 
    */
    encode_can_0x100_Body_Roll_deg(&can_obj,0.0f); 
    encode_can_0x100_Body_Pitch_deg(&can_obj,0.0f);
    encode_can_0x100_Body_Yaw_deg(&can_obj,0.0f);
  }

  /* CAN Message Receipt
   *  The inline comments below illustrates what's happening pretty well. 
   */
  size_t packetSize = (size_t)CAN.parsePacket();
  if (packetSize)    // if this is not zero, we have incoming data
  {
    MsgData msg; // Create a union object to hold the incoming data.
    long canID = CAN.packetId(); // Get the frame ID of the packet

    // Illustrative Serial Prints
    #if SERIAL_DEBUG_PRINTS_ON
    Serial.print("Received ");
    Serial.print("packet with id 0x");
    Serial.print(canID, HEX);
    Serial.print(" and length ");
    Serial.println((int)packetSize);
    #endif
    for (size_t i = 0; i < packetSize; i++)
    {
      if (!CAN.available())
      {
        Serial.println("ERROR: Packet shorter than specified"); // just some print outs if shit hits the fan.
        break;                                                  // something went wrong so leave this loop.
      }
      msg.b[i] = (uint8_t)CAN.read();
    }
    unpack_message(&can_obj,canID,msg.a,(uint8_t)8,millis());
    
    double yaw_in, roll_in, pitch_in; 
    decode_can_0x100_Body_Pitch_deg(&can_obj,&pitch_in);
    decode_can_0x100_Body_Roll_deg(&can_obj,&roll_in);
    decode_can_0x100_Body_Yaw_deg(&can_obj,&yaw_in);
    #if SERIAL_DEBUG_PRINTS_ON
    Serial.print("Roll Recieved: ");
    Serial.println(roll_in);
    Serial.print("Pitch Recieved: ");
    Serial.println(pitch_in);
    Serial.print("Yaw Recieved: ");
    Serial.println(yaw_in);
    Serial.println();
    #endif
  }
    
  
}
