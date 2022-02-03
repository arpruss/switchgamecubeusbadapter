#include <USBComposite.h>
#include <GameControllers.h>

//#define TEST
//#define OFFICIAL_REPORT // if this is commented out, the first GC controller will work on PC without any special drives, but compatibility with other devices may be reduced

#define VENDOR_ID 0x057E
#define PRODUCT_ID 0x0337

#define LED PC13
#define NUM_CONTROLLERS 1
#define RUMBLE 
// Facing GameCube socket (as on console), flat on top:
//    123
//    ===
//    456

// Connections for GameCube adapter:
// Optional and untested (use at own risk): GameCube 1--5V for better rumbling
// GameCube 2--PA6
// GameCube 2--1Kohm--3.3V
// GameCube 3--GND
// GameCube 4--GND
// GameCube 6--3.3V

const uint32_t gamecubePins[NUM_CONTROLLERS] = {PA6};
#ifdef RUMBLE
uint8_t rumbles[NUM_CONTROLLERS] = {0};
uint32_t lastRumbleTime = 0;
#define RUMBLE_TIMEOUT 4000 // in case of some sort of disconnection, stop rumbling after this
#define MAX_SIMULTANEOUS_RUMBLES  2 // to avoid too much current draw
#define GAMECUBE_CONTROLLER_ENABLED 0x14 // NORMAL, POWERED
#else
#define GAMECUBE_CONTROLLER_ENABLED 0x10 // NORMAL (0x22=WAVEBIRD COMMUNICATING)
#endif

//const uint32_t indicatorLEDs[] = { PA0, PA1, PA2, PA3 };

const uint8_t MAX_MISSED = 8; // if 8 reads are missed, we count the controller disconnected
uint8_t missed[NUM_CONTROLLERS] = { 0 };

GameCubeController gcc0(gamecubePins[0]);
#if NUM_CONTROLLERS >= 2
GameCubeController gcc1(gamecubePins[1]);
#endif
#if NUM_CONTROLLERS >= 3
GameCubeController gcc2(gamecubePins[2]);
#endif
#if NUM_CONTROLLERS >= 4
GameCubeController gcc3(gamecubePins[3]);
#endif

GameCubeController* gccs[NUM_CONTROLLERS] = {
  &gcc0,
#if NUM_CONTROLLERS >= 2
  &gcc1,
#endif  
#if NUM_CONTROLLERS >= 3
  &gcc2,
#endif  
#if NUM_CONTROLLERS >= 4
  &gcc3,
#endif  
};


#define MAIN_REPORT_ID 0x21
#define RUMBLE_REPORT_ID 17

USBHID HID;

uint8_t gameCubeAdapterReportDescriptor[] = {
#ifdef OFFICIAL_REPORT
0x05, 0x05,        // Usage Page (Game Ctrls)
0x09, 0x00,        // Usage (Undefined)
0xA1, 0x01,        // Collection (Application)
0x85, 0x11,        //   Report ID (17) RUMBLE
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x05,        //   Report Count (5)
0x91, 0x00,        //   Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              // End Collection
0xA1, 0x01,        // Collection (Application)
0x85, 0x21,        //   Report ID (33) MAIN
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x25,        //   Report Count (37)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
0xA1, 0x01,        // Collection (Application)
0x85, 0x12,        //   Report ID (18)
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x01,        //   Report Count (1)
0x91, 0x00,        //   Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              // End Collection
0xA1, 0x01,        // Collection (Application)
0x85, 0x22,        //   Report ID (34)
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x19,        //   Report Count (25)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
0xA1, 0x01,        // Collection (Application)
0x85, 0x13,        //   Report ID (19)
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x01,        //   Report Count (1)
0x91, 0x00,        //   Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              // End Collection
0xA1, 0x01,        // Collection (Application)
0x85, 0x23,        //   Report ID (35)
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x02,        //   Report Count (2)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
0xA1, 0x01,        // Collection (Application)
0x85, 0x14,        //   Report ID (20)
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x01,        //   Report Count (1)
0x91, 0x00,        //   Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              // End Collection
0xA1, 0x01,        // Collection (Application)
0x85, 0x24,        //   Report ID (36)
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x02,        //   Report Count (2)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
0xA1, 0x01,        // Collection (Application)
0x85, 0x15,        //   Report ID (21)
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x01,        //   Report Count (1)
0x91, 0x00,        //   Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              // End Collection
0xA1, 0x01,        // Collection (Application)
0x85, 0x25,        //   Report ID (37)
0x19, 0x00,        //   Usage Minimum (Undefined)
0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x75, 0x08,        //   Report Size (8)
0x95, 0x02,        //   Report Count (2)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
#else
0x05, 0x01,           /*  Usage Page (Generic Desktop) */ 
0x09, 0x05,           /*  Usage (Joystick) */ 
0xA1, 0x01,           /*  Collection (Application) */ 
0x85, 0x21,           //   Report ID (33)
0x75, 0x08,           //   Report Size (8)
0x95, 0x01,           //   Report Count (1)
0x81, 0x03,           //     INPUT = Cnst,Var,Abs
/* buttons */ 
0x15, 0x00,           /*  Logical Minimum (0) */ 
0x25, 0x01,           /*  Logical Maximum (1) */ 
0x75, 0x01,           /*  Report Size (1) */ 
0x95, 0x0C,           /*  Report Count (12) */ 
0x05, 0x09,           /*  Usage Page (9) */ 
0x19, 0x01,           /*  Usage Minimum (1) */ 
0x29, 0x0C,           /*  Usage Maximum (12) */ 
0x81, 0x02,           /*  Input (Data,Var,Abs) */ 
0x95, 0x04,           /*  Report Count (4) */ 
0x81, 0x03,          //     INPUT = Cnst,Var,Abs
  /* sticks */ 
0x05, 0x01,                     /* Usage Page (Generic Desktop) */ \
0x26, 0xFF, 0x00,        /*  Logical Maximum (255) */ 
0x09, 0x30,             /*  Usage (48) */ 
0x09, 0x31,             /*  Usage (49) */ 
0x09, 0x32,             /*  Usage (50) */ 
0x09, 0x35,             /*  Usage (53) */ 
0x09, 0x36,           /*  Usage (Slider) */ 
0x09, 0x36,           /*  Usage (Slider) */ 
0x75, 0x08,           /*  Report Size (8) */ 
0x95, 0x06,           /*  Report Count (6) */ 
0x81, 0x02,           /*  Input (Data,Var,Abs) */ 
0x75, 0x08,        //   Report Size (8)
0x95, 0x1B,        //   Report Count (36-9)
0x81, 0x03,          //     INPUT = Cnst,Var,Abs
0xC0,                 /*  End Collection */
#endif
};

/*
const uint16_t gcmaskA = 0x01;
const uint16_t gcmaskB = 0x02;
const uint16_t gcmaskX = 0x04;
const uint16_t gcmaskY = 0x08;
const uint16_t gcmaskStart = 0x10;
const uint16_t gcmaskDLeft = 0x100;
const uint16_t gcmaskDRight = 0x200;
const uint16_t gcmaskDDown = 0x400;
const uint16_t gcmaskDUp = 0x800;
const uint16_t gcmaskZ = 0x1000;
const uint16_t gcmaskR = 0x2000;
const uint16_t gcmaskL = 0x4000;

typedef struct {
  uint16_t buttons;
  uint8_t joystickX;
  uint8_t joystickY;
  uint8_t cX;
  uint8_t cY;
  uint8_t shoulderLeft;
  uint8_t shoulderRight;
} __packed GameCubeData_t;
*/

// useful reference: https://github.com/FIX94/Nintendont/blob/fd5e85c4fe4c4015936e21b16242fa0f15449e99/loader/source/ppc/PADReadGC/source/PADReadGC.c

typedef struct {
  uint8_t state;
  GameCubeData_t data;
} __packed GameCubeSingleController_t;

typedef struct {
  uint8_t reportId;
  GameCubeSingleController_t payload[4];
} __packed MainGameCubeReport_t;

static_assert(sizeof(MainGameCubeReport_t)==37, "Wrong packing!");

class HIDGameCubeAdapter : public HIDReporter {
public:
  MainGameCubeReport_t mainReport; 
  
  void begin() {
      USBComposite.setVendorId(VENDOR_ID);
      USBComposite.setProductId(PRODUCT_ID);
      USBComposite.setProductString("Gamecube Adapter");
      USBComposite.setManufacturerString("Omega Centauri Software");
      HID.begin(gameCubeAdapterReportDescriptor, sizeof(gameCubeAdapterReportDescriptor));
  }
  
  void end() {
      HID.end();
  }

  HIDGameCubeAdapter(USBHID& HID) 
            : HIDReporter(HID, (uint8_t*)&mainReport, sizeof(mainReport), MAIN_REPORT_ID) {
        memset(&mainReport, 0, sizeof(mainReport));
        mainReport.reportId = MAIN_REPORT_ID;
        for (int i=0;i<4;i++) {
          mainReport.payload[i].data.buttons = 0; // 0x8000;
          mainReport.payload[i].data.joystickX = 128;
          mainReport.payload[i].data.joystickY = 128;
          mainReport.payload[i].data.cX = 128;
          mainReport.payload[i].data.cY = 128;
        }
    }
};
HIDGameCubeAdapter adapter(HID);

uint8_t buffer[64];

void newData(void* extra, volatile void* buffer, uint16_t size) {
  
#ifdef RUMBLE  
  if (size == 5 && ((uint8_t*)buffer)[0] == 0x11) {
    memcpy(rumbles,((uint8_t*)buffer)+1,NUM_CONTROLLERS);
    lastRumbleTime = millis();    
  }
#endif  
}

void setup() {
  pinMode(LED, OUTPUT);

  for (int i=0;i<NUM_CONTROLLERS; i++)
    gccs[i]->begin();

  usb_hid_setDedicatedRXEndpoint(buffer, sizeof(buffer), newData, NULL);
  adapter.begin();
  while (!USBComposite);
  delay(1000);
}

uint16_t buttonsToUSB(uint16_t controllerButtons) {
  uint16_t abxy = controllerButtons & 0xF;
  uint16_t dpad = (controllerButtons >> 8) & 0xF;
  uint16_t z = (controllerButtons & gcmaskZ) != 0;
  uint16_t s = (controllerButtons & gcmaskStart) != 0;
  uint16_t rl = (controllerButtons >> 13) & 0x3;
  return abxy | (dpad << 4) | (s << 8) | (z << 9) | (rl << 10);
}

void xloop() {
  digitalWrite(LED,0);
}

void loop() {  
#ifdef TEST
  adapter.mainReport.payload[0].state = GAMECUBE_CONTROLLER_ENABLED;
  adapter.mainReport.payload[0].data.joystickX = 128;
  adapter.mainReport.payload[0].data.joystickY = 128;
  adapter.mainReport.payload[0].data.buttons = 0;
  adapter.sendReport();
  digitalWrite(LED,1);
  delay(300);
  adapter.mainReport.payload[0].data.joystickX = 255;
  adapter.mainReport.payload[0].data.buttons = buttonsToUSB(gcmaskA);
  adapter.sendReport();
  digitalWrite(LED,0);
  delay(300);
  adapter.mainReport.payload[0].data.joystickX = 0;
  adapter.mainReport.payload[0].data.buttons = 0;
  adapter.sendReport();
  digitalWrite(LED,1);
  delay(300); 
#else  
  boolean haveOne = false;
  uint32 rumbleCount = 0;
  for (int i=0; i<NUM_CONTROLLERS; i++) {
    bool rumble = false;
#ifdef RUMBLE    
    if (rumbles[i] && millis() < lastRumbleTime+RUMBLE_TIMEOUT && rumbleCount < MAX_SIMULTANEOUS_RUMBLES) {
      rumble = true;
      rumbleCount++;
    }
#else
    rumble = false;
#endif    
    GameCubeData_t data;
    bool success = gccs[i]->readWithRumble(&data, rumble);
    if (success) {
      adapter.mainReport.payload[i].state = GAMECUBE_CONTROLLER_ENABLED;
      data.buttons = buttonsToUSB(data.buttons);
      adapter.mainReport.payload[i].data = data;
      missed[i] = 0;
      haveOne = true;
    }
    else {
      if (adapter.mainReport.payload[i].state) {
        if (missed[i] > MAX_MISSED)
          adapter.mainReport.payload[i].state = 0;
        else
          missed[i]++;
      }
    }
  }
  if (haveOne) {
    adapter.sendReport();
    digitalWrite(LED, !haveOne);
  }
#endif  
}

