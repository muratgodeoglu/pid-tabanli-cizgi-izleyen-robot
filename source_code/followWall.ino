// ================================================================
//  followWall.ino — Duvar Takip Algoritması
// ================================================================

void followWall() {
    // Kısa ileri atış yaparak referans değerleri al
    setDriveMotors(100, 100);
    analogWrite(BLOWER_PIN, 0);
    delay(40);

    readLineSensors();
    prevTrackError = 0;

    // Referans okumaları al (dummy read + gerçek okuma)
    analogRead(A0); analogRead(A7);
    int rightIdeal = analogRead(A0);
    int leftIdeal  = analogRead(A7);

    gainD = 1.0f;

    while (lineDetected == 0) {
        analogWrite(BLOWER_PIN, 0);

        // Sensör değerlerini referansa göre kırp
        int rightRaw = constrain(analogRead(A0), 0, rightIdeal);
        int leftRaw  = constrain(analogRead(A7), 0, leftIdeal);

        // Hata hesaplama
        int rightErr = map(rightRaw, 0, rightIdeal, 0, 4000);
        int leftErr  = map(leftRaw,  0, leftIdeal,  0, 4000);
        trackError   = rightErr - leftErr;

        // PD düzeltme
        steerRatio      = gainP * (float)trackError;
        steerDerivative = gainD * (float)(trackError - prevTrackError);
        steerCorrection = constrain(steerRatio + steerDerivative, -510.0f, 510.0f);

        rightDrivePWM = constrain(wallSpeed - (int)steerCorrection, -100, 255);
        leftDrivePWM  = constrain(wallSpeed + (int)steerCorrection, -100, 255);

        setDriveMotors(leftDrivePWM, rightDrivePWM);
        prevTrackError = trackError;

        readLineSensors();
    }

    // Çizgi tekrar bulundu — hızı artır
    cruiseSpeed = 250;
    brakeBias   = 35;
    analogWrite(BLOWER_PIN, 0);
}
