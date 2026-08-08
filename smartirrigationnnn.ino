#define BLYNK_TEMPLATE_ID "YourTemplateIDHere"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation System"
#define BLYNK_AUTH_TOKEN "YourAuthTokenHere"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <WebServer.h>
#include <DHT.h>

// ================= WIFI =================
char ssid[] = "YourSSIDHere";
char pass[] = "YourPasswordHere";

// ================= HOTSPOT =================
const char* ap_ssid = "ESP32_Farm";
const char* ap_pass = "12345678";

// ================= PINS =================
#define SOIL_PIN1   34
#define SOIL_PIN2   35
#define DHT_PIN     4
#define RELAY_PIN   26

#define IR1_PIN     27
#define IR2_PIN     25

#define ALERT_LED   2

#define DHTTYPE DHT11

// ================= OBJECTS =================
DHT dht(DHT_PIN, DHTTYPE);

BlynkTimer timer;
WebServer server(80);

// ================= VARIABLES =================
bool autoMode = true;
bool pumpState = false;

int dryValue = 3500;
int wetValue = 1500;

int moistureThreshold = 30;

// ================= FLOW VARIABLES =================
volatile unsigned long irTime1 = 0;
volatile unsigned long irTime2 = 0;

float velocity = 0;
float flowRate = 0;

// Distance between IR sensors (meters)
const float distanceBetweenIR = 0.10;

// Pipe radius (meters)
const float pipeRadius = 0.00125;

// ================= FUNCTION PROTOTYPE =================
void handleRoot();

// =====================================================
// IR INTERRUPTS
// =====================================================
void IRAM_ATTR IR1_ISR()
{
  irTime1 = millis();
}

void IRAM_ATTR IR2_ISR()
{
  irTime2 = millis();
}

// =====================================================
// FLOW CALCULATION
// =====================================================
void calculateFlow()
{
  if (irTime1 != 0 && irTime2 != 0 && irTime2 > irTime1)
  {
    float timeDiff = (irTime2 - irTime1) / 1000.0;

    if (timeDiff > 0)
    {
      velocity = distanceBetweenIR / timeDiff;

      float area = 3.1416 * pipeRadius * pipeRadius;

      flowRate = area * velocity * 1000.0;

      irTime1 = 0;
      irTime2 = 0;
    }
  }
}

// =====================================================
// MANUAL CONTROL
// =====================================================
BLYNK_WRITE(V3)
{
  if (!autoMode)
  {
    pumpState = param.asInt();

    digitalWrite(RELAY_PIN, pumpState ? LOW : HIGH);

    Blynk.virtualWrite(V4, pumpState);
  }
}

// =====================================================
// AUTO / MANUAL MODE
// =====================================================
BLYNK_WRITE(V5)
{
  autoMode = param.asInt();
}

// =====================================================
// BEAUTIFUL RESPONSIVE WEBPAGE
// =====================================================
String getHTML(float t, float h, int s1, int s2)
{
  String html = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta charset="UTF-8">

<meta name="viewport" content="width=device-width, initial-scale=1">

<meta http-equiv="refresh" content="3">

<title>Smart Irrigation Dashboard</title>

<style>

body{
  font-family: Arial;
  background: #e8f5e9;
  margin:0;
  padding:20px;
  text-align:center;
}

h1{
  color:#1b5e20;
}

.container{
  display:flex;
  flex-wrap:wrap;
  justify-content:center;
}

.card{
  background:white;
  width:280px;
  margin:12px;
  padding:20px;
  border-radius:15px;
  box-shadow:0 4px 10px rgba(0,0,0,0.2);
}

.value{
  font-size:30px;
  font-weight:bold;
  color:#1565c0;
}

.on{
  color:green;
  font-weight:bold;
}

.off{
  color:red;
  font-weight:bold;
}

</style>

</head>

<body>

<h1>Smart Irrigation Dashboard</h1>

<div class="container">

<div class="card">
<h2>Soil Moisture 1</h2>
<div class="value">)rawliteral";

  html += String(s1);

  html += R"rawliteral(%</div>
</div>

<div class="card">
<h2>Soil Moisture 2</h2>
<div class="value">)rawliteral";

  html += String(s2);

  html += R"rawliteral(%</div>
</div>

<div class="card">
<h2>Temperature</h2>
<div class="value">)rawliteral";

  html += String(t, 1);

  html += R"rawliteral(&deg;C</div>
</div>

<div class="card">
<h2>Humidity</h2>
<div class="value">)rawliteral";

  html += String(h, 1);

  html += R"rawliteral(%</div>
</div>

<div class="card">
<h2>Flow Rate</h2>
<div class="value">)rawliteral";

  html += String(flowRate, 2);

  html += R"rawliteral(L/s</div>
</div>

<div class="card">
<h2>Pump Status</h2>
<div class="value">)rawliteral";

  if (pumpState)
  {
    html += "<span class='on'>ON</span>";
  }
  else
  {
    html += "<span class='off'>OFF</span>";
  }

  html += R"rawliteral(

</div>
</div>

</div>

</body>

</html>

)rawliteral";

  return html;
}


// =====================================================
// WEB SERVER
// =====================================================
void handleRoot()
{
  int raw1 = analogRead(SOIL_PIN1);
  int raw2 = analogRead(SOIL_PIN2);

  int soil1 = constrain(map(raw1, dryValue, wetValue, 0, 100), 0, 100);
  int soil2 = constrain(map(raw2, dryValue, wetValue, 0, 100), 0, 100);

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h))
  {
    t = 0;
    h = 0;
  }

  server.send(200, "text/html", getHTML(t, h, soil1, soil2));
}

// =====================================================
// SENSOR DATA
// =====================================================
void sendSensorData()
{
  int raw1 = analogRead(SOIL_PIN1);
  int raw2 = analogRead(SOIL_PIN2);

  int soil1 = constrain(map(raw1, dryValue, wetValue, 0, 100), 0, 100);
  int soil2 = constrain(map(raw2, dryValue, wetValue, 0, 100), 0, 100);

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h))
  {
    Serial.println("DHT Error!");
    return;
  }

  // ================= AUTO MODE =================
  if (autoMode)
  {
    if (soil1 < moistureThreshold || soil2 < moistureThreshold)
    {
      pumpState = true;

      digitalWrite(RELAY_PIN, LOW);

      digitalWrite(ALERT_LED, HIGH);
    }
    else
    {
      pumpState = false;

      digitalWrite(RELAY_PIN, HIGH);

      digitalWrite(ALERT_LED, LOW);
    }
  }

  // ================= FLOW =================
  calculateFlow();

  // ================= BLYNK =================
  Blynk.virtualWrite(V0, soil1);
  Blynk.virtualWrite(V6, soil2);

  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);

  Blynk.virtualWrite(V4, pumpState);

  Blynk.virtualWrite(V7, flowRate);

  // ================= SERIAL =================
  Serial.println("\n========== SMART IRRIGATION ==========");

  Serial.print("Soil Moisture 1: ");
  Serial.print(soil1);
  Serial.println("%");

  Serial.print("Soil Moisture 2: ");
  Serial.print(soil2);
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");

  Serial.print("Flow Rate: ");
  Serial.print(flowRate);
  Serial.println(" L/s");

  Serial.print("Pump Status: ");
  Serial.println(pumpState ? "ON" : "OFF");

  Serial.println("======================================");
}

// =====================================================
// SETUP
// =====================================================
void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  pinMode(ALERT_LED, OUTPUT);

  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);

  digitalWrite(RELAY_PIN, HIGH);

  attachInterrupt(
    digitalPinToInterrupt(IR1_PIN),
    IR1_ISR,
    FALLING
  );

  attachInterrupt(
    digitalPinToInterrupt(IR2_PIN),
    IR2_ISR,
    FALLING
  );

  dht.begin();

  analogReadResolution(12);

  // ================= WIFI MODE =================
  WiFi.mode(WIFI_AP_STA);

  // ================= HOTSPOT =================
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println();
  Serial.println("ESP32 Hotspot Started");

  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  // ================= WIFI CONNECT =================
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");

  int count = 0;

  while (WiFi.status() != WL_CONNECTED && count < 20)
  {
    delay(500);

    Serial.print(".");

    count++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println();
    Serial.println("WiFi Connected!");

    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println();
    Serial.println("WiFi Connection Failed!");
  }

  // ================= BLYNK =================
  Blynk.config(BLYNK_AUTH_TOKEN);

  Blynk.connect();

  // ================= WEB SERVER =================
  server.on("/", handleRoot);

  server.begin();

  Serial.println("Web Server Started");

  // ================= TIMER =================
  timer.setInterval(3000L, sendSensorData);
}

// =====================================================
// LOOP
// =====================================================
void loop()
{
  Blynk.run();

  timer.run();

  server.handleClient();

  delay(1);
}