// ================================================================
//  Motors.ino — Motor Sürücü Fonksiyonları
// ================================================================

// ── Motor Sürücü ─────────────────────────────────────────────────
void setDriveMotors(int leftPWM, int rightPWM) {
    if (leftPWM <= 0) {
        digitalWrite(LEFT_DRIVE_A, LOW);
        analogWrite( LEFT_DRIVE_B, -leftPWM);
    } else {
        digitalWrite(LEFT_DRIVE_B, LOW);
        analogWrite( LEFT_DRIVE_A,  leftPWM);
    }

    if (rightPWM <= 0) {
        digitalWrite(RIGHT_DRIVE_A, LOW);
        analogWrite( RIGHT_DRIVE_B, -rightPWM);
    } else {
        digitalWrite(RIGHT_DRIVE_B, LOW);
        analogWrite( RIGHT_DRIVE_A,  rightPWM);
    }
}

// ── Fren ─────────────────────────────────────────────────────────
void applyBrake(int durationMs) {
    setDriveMotors(0, 0);
    delay(durationMs);
}

// ── Motor Başlatma ───────────────────────────────────────────────
void initDriveMotors() {
    // Timer0 & Timer1 — 4 kHz PWM
    TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);
    TCCR0B = _BV(CS01);
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
    TCCR1B = _BV(CS11);

    // ADC hızlandır (prescaler = 4)
    ADCSRA = 0b00000100;
    ADCSRA |= (1 << ADEN);

    // Pin modları
    pinMode(RIGHT_DRIVE_A, OUTPUT);
    pinMode(RIGHT_DRIVE_B, OUTPUT);
    pinMode(LEFT_DRIVE_A,  OUTPUT);
    pinMode(LEFT_DRIVE_B,  OUTPUT);

    // Başlangıçta tüm pinleri LOW yap
    digitalWrite(RIGHT_DRIVE_A, LOW);
    digitalWrite(RIGHT_DRIVE_B, LOW);
    digitalWrite(LEFT_DRIVE_A,  LOW);
    digitalWrite(LEFT_DRIVE_B,  LOW);

    setDriveMotors(0, 0);
    delay(2000);
}
