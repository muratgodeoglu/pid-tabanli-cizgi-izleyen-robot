// ================================================================
//  ROBOT CONTROLLER FIRMWARE
//  Editör: Murat GÖDEOĞLU — ÇEAL Robotics
// ================================================================
//  Mikrodenetleyici : Arduino Nano
//  Motor Sürücü     : Ufuk Bengi özel MOSFET sürücü (30V 20A)
//  Motorlar         : Coreless 1324 DC motor
//  Sensörler        : Rize Robotik duvar takipli çizgi izleme sensörü
//  Kapı Sensörü     : IR alıcı-verici özel modül
//  Encoder          : 4 Pulse, QRE1113GR
//
//  Yazılım Açıklaması:
//  PD tabanlı yüksek hızlı çizgi izleme kontrol algoritması.
//  Encoder tabanlı mesafe hesaplama, dinamik hız profili,
//  çizgi kayıp kurtarma (spin / duvar takip) destekler.
//
//  Performans:
//  - Loop frekansı     : ~1200 Hz
//  - Ortalama hız      : 4.3 m/s
//  - Türkiye sıralaması: 9.
//  - 18. MEB Robot Yarışması 1. Kulvar — 10.73 sn
//
//  Tüm hakları ÇEAL Robotics'e aittir.
// ================================================================

// ── Pin Tanımları ────────────────────────────────────────────────
#define LEFT_DRIVE_A    10
#define LEFT_DRIVE_B     9
#define RIGHT_DRIVE_A    6
#define RIGHT_DRIVE_B    5
#define BLOWER_PIN      11
#define GATE_SENSOR     12
#define RIGHT_PULSE_PIN  3
#define LEFT_PULSE_PIN   2

// ── Encoder Yapılandırması ───────────────────────────────────────
#define WHEEL_DIAMETER_MM  25.0
#define ENCODER_PPR         4

const float WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER_MM * 3.14159f;
const float MM_PER_PULSE        = WHEEL_CIRCUMFERENCE / ENCODER_PPR;

// ── Encoder Durumu ───────────────────────────────────────────────
bool          leftPulseState  = false;
bool          rightPulseState = false;
volatile long leftPulseCount  = 0;
volatile long rightPulseCount = 0;

// ── Ayarlanabilir Parametreler ───────────────────────────────────
int   cruiseSpeed    = 190;
int   wallSpeed      = 250;
float gainP          = 0.022f;
float gainD          = 1.0f;
int   blackThreshold = 250;
int   readMin        = 40;
int   readMax        = 900;
int   sensorWeight[8] = { 0, -8, -5, -2, 2, 5, 8, 0 };

// ── Sensör Tamponları ────────────────────────────────────────────
int rawReading[8];
int normReading[8];

// ── Çalışma Zamanı Değişkenleri ──────────────────────────────────
long  timestamp;
long  lossTimestamp;
int   trackError;
int   prevTrackError = 0;
int   lastSide;
int   brakeBias;
byte  lineDetected;
byte  activeSensor;

// ── Motor PWM Çıkışları ──────────────────────────────────────────
int leftDrivePWM;
int rightDrivePWM;

// ── PD Ara Değerleri ─────────────────────────────────────────────
float steerRatio;
float steerDerivative;
float steerCorrection;

// ================================================================
//  SETUP
// ================================================================
void setup() {
    initPulseCounters();
    initDriveMotors();
    pinMode(GATE_SENSOR, INPUT_PULLUP);

    // Blower ısınma gecikmesi
    analogWrite(BLOWER_PIN, 240);
    delay(200);
    analogWrite(BLOWER_PIN, 250);

    timestamp = millis();
    resetPulseCounters();

    // Kapı açılana kadar bekle
    while (digitalRead(GATE_SENSOR) == LOW) {
        applyBrake(1);
        readLineSensors();
    }
}

// ================================================================
//  ANA DÖNGÜ
// ================================================================
void loop() {
    readPulseCounters();
    trackError = readLineSensors();

    int distanceMM = calcAvgDistanceMM();

    // Dinamik hız profili
    if (distanceMM < 9000) {
        cruiseSpeed = 230;
        gainD       = 0.8f;
    } else {
        cruiseSpeed = 185;
        gainD       = 1.0f;
    }

    // Fren biası — büyük hata varsa hızı kıs
    brakeBias = (abs(trackError) < 1000) ? 0 : -(abs(trackError) / 35);

    if (lineDetected == 1) {
        // PD çizgi takibi
        steerRatio      = gainP * (float)trackError;
        steerDerivative = gainD * (float)(trackError - prevTrackError);
        steerCorrection = constrain(steerRatio + steerDerivative, -510.0f, 510.0f);

        leftDrivePWM  = constrain(cruiseSpeed - (int)steerCorrection + brakeBias, -200, 255);
        rightDrivePWM = constrain(cruiseSpeed + (int)steerCorrection + brakeBias, -200, 255);

        setDriveMotors(leftDrivePWM, rightDrivePWM);
        prevTrackError = trackError;

    } else if (lastSide == 1) {
        // Çizgi sağda kayboldu — dönerek kurtar
        setDriveMotors(-200, 220);

    } else if (lastSide == -1) {
        // Çizgi solda kayboldu — dönerek kurtar
        setDriveMotors(220, -200);

    } else if ((millis() - timestamp) > 700) {
        // Çizgi tamamen kayboldu — duvar takibine geç
        followWall();
        analogWrite(BLOWER_PIN, 0);
    }
}
