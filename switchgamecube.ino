#include <USBComposite.h>
#include <GameControllers.h>


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
  adapter.mainReport.payload[0].state = GAMECUBE_CONTROLLER_ENABLED;
}

void loop() {  
  adapter.mainReport.payload[0].data.joystickX = 255;
  adapter.mainReport.payload[0].data.buttons |= gcmaskA;
  adapter.sendReport();
  delay(300);
  adapter.mainReport.payload[0].data.joystickX = 0;
  adapter.mainReport.payload[0].data.buttons &= ~gcmaskA;
  adapter.sendReport();
  delay(300);
}
