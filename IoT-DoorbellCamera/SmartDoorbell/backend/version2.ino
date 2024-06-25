#include <FirebaseESP32.h>
#include "esp_camera.h"
#include <WiFi.h>


//#define FIREBASE_HOST "ENTER_YOUR_HOST_HERE"
//#define FIREBASE_AUTH "ENTER_YOUR_AUTH_HERE"

#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#include "camera_pins.h"

const char* ssid = "ENTER_YOUR_WIFI_SSID";
const char* password = "ENTER_YOUR_WIFI_PASSWORD";

//Peripherals - LED, BUZZER, BUTTON
const int buttonPin = 14;  
#define LED_PIN 13
#define BUZZER_PIN 2

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

FirebaseData fbdo;

FirebaseAuth auth;  //novo za firebase
FirebaseConfig configF; //novo za firebase


#define DATABASE_URL "DATABASE_URL"
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

#define API_KEY "API_KEY"

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // camera setup
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  #if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
  #endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV2640_PID) { // PROMJENJENO U MOJ BROJ KAMERE
    s->set_vflip(s, 0); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_SVGA);

  #if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
  #endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  //ZA FIREBASE (NOVO)
  Serial.printf("Firebase Client v%\n\n", FIREBASE_CLIENT_VERSION);

  configF.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  configF.database_url = DATABASE_URL;
  configF.token_status_callback = tokenStatusCallback;

  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  Firebase.begin(&configF, &auth);


}

void loop() {
  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  // Perform actions based on button state
  if (buttonState == LOW) {
    // Button is pressed
    
    Serial.println("nesto");
    digitalWrite(LED_PIN,HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN,LOW);

    //camera 
    camera_fb_t* camera_fb = esp_camera_fb_get();
    if (!camera_fb) {
      Serial.println("Failed to capture image.");
      return;
    }
  
    // Upload image to Firebase Realtime Database
    String path = "/images"; // Change the path as desired
  
    if (Firebase.setBlob(fbdo, path.c_str(), camera_fb->buf, camera_fb->len)) {
      Serial.println("Image uploaded");
    }   else {
      Serial.println("Failed to upload image.");
    }
  
    //Clean up
    esp_camera_fb_return(camera_fb);
  }
   
    delay(500);
    /*
    bool bVal;
    Firebase.getBool(fbdo, F("/ledControl"), &bVal);
 
    if(bVal?true:false) {
      digitalWrite(LED_PIN,HIGH);
      delay(1000);
      digitalWrite(LED_PIN,LOW);
    }
    */

}
