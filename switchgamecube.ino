#include <USBComposite.h>
#include <GameControllers.h>

#define LED PC13
#define NUM_CONTROLLERS 1
// Facing GameCube socket (as on console), flat on top:
//    123
//    ===
//    456

// Connections for GameCube adapter:
// GameCube 2--PA6
// GameCube 2--1Kohm--3.3V
// GameCube 3--GND
// GameCube 4--GND
// GameCube 6--3.3V
const uint32_t gamecubePins[NUM_CONTROLLERS] = {PA6};


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

#define VENDOR_ID 0x057E 
#define PRODUCT_ID 0x0337
#define LED PB12

#define MAIN_REPORT_ID 33

USBHID HID;

uint8_t gameCubeAdapterReportDescriptor[] = {
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

#define GAMECUBE_CONTROLLER_ENABLED 0x10 // NORMAL (0x20=WAVEBIRD)

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
      for (int i=0;i<NUM_CONTROLLERS; i++)
        gccs[i]->begin();
    
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
          mainReport.payload[i].data.buttons = 0x8000;
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
  (void)extra;
  (void)buffer;
  (void)size;
}

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  usb_hid_setDedicatedRXEndpoint(buffer, sizeof(buffer), newData, NULL);
  adapter.begin();
  while (!USBComposite);
  delay(1000);
}

uint8_t fixDeadZone(uint8_t x) {
  return 255;
  if (128 - 40 < x && x < 128 + 40)
    return 128;
  else
    return x;
}

uint16_t buttonsToUSB(uint16_t controllerButtons) {
  uint16_t abxy = controllerButtons & 0xF;
  uint16_t dpad = (controllerButtons >> 8) & 0xF;
  uint16_t z = (controllerButtons & gcmaskZ) != 0;
  uint16_t s = (controllerButtons & gcmaskStart) != 0;
  uint16_t rl = (controllerButtons >> 13) & 0x3;
  return abxy | (dpad << 4) | (s << 8) | (z << 9) | (rl << 10);
}

void loop() {  
/*  adapter.mainReport.payload[0].data.joystickX = 255;
  adapter.mainReport.payload[0].data.buttons |= gcmaskA;
  adapter.sendReport();
  delay(300);
  adapter.mainReport.payload[0].data.joystickX = 0;
  adapter.mainReport.payload[0].data.buttons &= ~gcmaskA;
  adapter.sendReport();
  delay(300); */
  boolean haveOne = false;
  for (int i=0; i<NUM_CONTROLLERS; i++) {
    bool rumble = false; // TODO
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
}
