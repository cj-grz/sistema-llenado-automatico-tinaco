#define TRIG_TINACO   2
#define ECHO_TINACO   3
#define TRIG_CISTERNA 4
#define ECHO_CISTERNA 5
#define RELE_BOMBA    6

#define LED_R         9
#define LED_G         10
#define LED_B         11

#define LED_LLENADO   12   

const float TINACO_D_MAX   = 150.0;
const float TINACO_D_MIN   = 2.0;
const float CISTERNA_D_MAX = 150.0;
const float CISTERNA_D_MIN = 2.0;

bool bombaEncendida = false;

float medirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(5);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracion = pulseIn(echo, HIGH, 40000);

  if (duracion == 0) return -1.0;

  return duracion * 0.0343 / 2.0;
}

float calcularPorcentaje(float distancia, float dMax, float dMin) {
  if (distancia < 0) return -1.0;

  float pct = (dMax - distancia) / (dMax - dMin) * 100.0;

  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;

  return pct;
}

void setLED(int r, int g, int b) {
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);
}

void ledVerde()    { setLED(0, 255, 0); }
void ledRojo()     { setLED(255, 0, 0); }
void ledAmarillo() { setLED(255, 180, 0); }
void ledMagenta()  { setLED(255, 0, 255); }

// Ajusta si tu relay está invertido
void bombaON()  { digitalWrite(RELE_BOMBA, HIGH); }
void bombaOFF() { digitalWrite(RELE_BOMBA, LOW);  }

// Si está invertido, usa esto:
// void bombaON()  { digitalWrite(RELE_BOMBA, LOW); }
// void bombaOFF() { digitalWrite(RELE_BOMBA, HIGH); }

void setup() {
  pinMode(TRIG_TINACO, OUTPUT);
  pinMode(ECHO_TINACO, INPUT);
  pinMode(TRIG_CISTERNA, OUTPUT);
  pinMode(ECHO_CISTERNA, INPUT);

  pinMode(RELE_BOMBA, OUTPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  pinMode(LED_LLENADO, OUTPUT);

  digitalWrite(TRIG_TINACO, LOW);
  digitalWrite(TRIG_CISTERNA, LOW);

  bombaOFF();
  digitalWrite(LED_LLENADO, LOW);

  Serial.begin(9600);
}

void loop() {
  float distTinaco = medirDistancia(TRIG_TINACO, ECHO_TINACO);
  delay(60);
  float distCisterna = medirDistancia(TRIG_CISTERNA, ECHO_CISTERNA);

  float pctTinaco = calcularPorcentaje(distTinaco, TINACO_D_MAX, TINACO_D_MIN);
  float pctCisterna = calcularPorcentaje(distCisterna, CISTERNA_D_MAX, CISTERNA_D_MIN);

  Serial.print("Tinaco: ");
  Serial.print(pctTinaco);
  Serial.print("% | Cisterna: ");
  Serial.print(pctCisterna);
  Serial.println("%");

  // Validación de sensores
  if (pctTinaco < 0 || pctCisterna < 0) {
    bombaEncendida = false;
    bombaOFF();
    digitalWrite(LED_LLENADO, LOW);
    ledMagenta();
    Serial.println("ERROR SENSOR");
    delay(1000);
    return;
  }

  // ---- CONTROL DE BOMBA ----

  if (pctCisterna < 10.0) {
    bombaEncendida = false;
  } 
  else {
    if (!bombaEncendida && pctTinaco < 30.0) {
      bombaEncendida = true;
    }

    if (bombaEncendida && pctTinaco > 90.0) {
      bombaEncendida = false;
    }
  }

  if (bombaEncendida) {
    bombaON();
    digitalWrite(LED_LLENADO, HIGH);  // LED azul ON
  } else {
    bombaOFF();
    digitalWrite(LED_LLENADO, LOW);   // LED azul OFF
  }

  // ---- LED RGB (PRIORIDAD CORRECTA) ----

  if (pctCisterna < 10.0) {
    ledMagenta();
    Serial.println("ESTADO: FALLA");
  }
  else if (pctTinaco > 90.0) {
    ledVerde();
    Serial.println("ESTADO: LLENO");
  }
  else if (pctTinaco < 30.0) {
    ledRojo();
    Serial.println("ESTADO: VACIO");
  }
  else {
    ledAmarillo();
    Serial.println("ESTADO: NORMAL");
  }

  delay(500);
}
