// ================================================================
//  Encoders.ino — Enkoder Okuma ve Mesafe Hesaplama
// ================================================================

// ── Enkoder Başlatma ─────────────────────────────────────────────
void initPulseCounters() {
    pinMode(LEFT_PULSE_PIN,  INPUT);
    pinMode(RIGHT_PULSE_PIN, INPUT);
}

// ── Enkoder Okuma ────────────────────────────────────────────────
void readPulseCounters() {
    bool newLeftState  = digitalRead(LEFT_PULSE_PIN);
    bool newRightState = digitalRead(RIGHT_PULSE_PIN);

    if (newLeftState != leftPulseState) {
        leftPulseState = newLeftState;
        leftPulseCount++;
    }
    if (newRightState != rightPulseState) {
        rightPulseState = newRightState;
        rightPulseCount++;
    }
}

// ── Enkoder Sıfırlama ────────────────────────────────────────────
void resetPulseCounters() {
    leftPulseCount  = 0;
    rightPulseCount = 0;
}

// ── Mesafe Hesaplama ─────────────────────────────────────────────
float leftDistanceMM()      { return leftPulseCount  * MM_PER_PULSE; }
float rightDistanceMM()     { return rightPulseCount * MM_PER_PULSE; }
float calcAvgDistanceMM()   { return (leftDistanceMM() + rightDistanceMM()) / 2.0f; }
