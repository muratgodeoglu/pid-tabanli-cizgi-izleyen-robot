// ================================================================
//  readLineSensors.ino — Çizgi Sensörü Okuma
// ================================================================

int readLineSensors() {
    long weightedSum = 0;
    long sensorTotal = 0;
    bool lineVisible = false;

    lineDetected = 0;
    activeSensor = 0;

    // Sensör 1–6 arası oku (0 ve 7 kenarlarda kullanılmıyor)
    for (int i = 1; i < 7; i++) {
        // Doğrudan ADC register erişimi (hız için)
        ADMUX   = (1 << REFS0) | (i & 0x07);
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC));

        int raw        = constrain(ADC, readMin, readMax);
        normReading[i] = 1000 - ((raw - readMin) * 1000L) / 860;

        if (normReading[i] > blackThreshold) {
            lineDetected = 1;
            activeSensor++;
            lineVisible  = true;
        }

        weightedSum += (long)normReading[i] * sensorWeight[i];
        sensorTotal +=       normReading[i];
    }

    if (lineVisible) lossTimestamp = millis();

    // Son görülen tarafı güncelle
    if      (normReading[6] > blackThreshold && normReading[1] < blackThreshold) lastSide =  1;
    else if (normReading[6] < blackThreshold && normReading[1] > blackThreshold) lastSide = -1;
    else if (normReading[3] > blackThreshold || normReading[4] > blackThreshold) lastSide =  0;

    // Çizgi yoksa son bilinen tarafa göre sabit hata döndür
    if (lineDetected == 0) {
        if (lastSide ==  1) return  5000;
        if (lastSide == -1) return -5000;
    }

    if (sensorTotal == 0) return 0;

    return (int)((weightedSum * 1000L) / sensorTotal);
}
