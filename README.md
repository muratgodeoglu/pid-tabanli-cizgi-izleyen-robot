# pid-tabanli-cizgi-izleyen-robot
# Çizgi İzleyen Robot Sistemi

Bu proje, yüksek hızlı ve otonom çizgi izleme görevleri için geliştirilmiş bir mobil robot sistemidir. Piyasada yaygın olarak satılan bir robot için yazılan koddur. Sistem; mekanik tasarım, elektronik kontrol ve gömülü yazılım bileşenlerinden oluşmaktadır.

---

## 🚀 Proje Amacı

Bu projenin amacı, yarışma parkurunda yüksek hızda stabil şekilde çizgi takibi yapabilen, kayıp durumlarında kendini toparlayabilen otonom bir robot geliştirmektir.

---

## 🧠 Sistem Özeti

Robot sistemi şu bileşenlerden oluşmaktadır:

- **Kontrol Birimi:** Arduino Nano
- **Motorlar:** Coreless DC motorlar
- **Motor Sürücü:** Yüksek akım MOSFET tabanlı sürücü
- **Sensörler:** Çizgi izleme sensör dizisi + IR algılama modülü
- **Encoder:** Hız ve mesafe geri besleme sistemi
- **Mekanik Yapı:** 3D yazıcı ile üretilmiş şasi ve bağlantı parçaları

---

## 💻 Yazılım

Sistem yazılımı gerçek zamanlı kontrol prensibine göre geliştirilmiştir:

- PD tabanlı çizgi izleme algoritması
- Encoder geri beslemeli hız kontrolü
- Dinamik hız profili (viraj ve düz yol optimizasyonu)
- Çizgi kaybı durumunda kurtarma algoritması (spin / yeniden yakalama)

---

## ⚙️ Performans

- Kontrol döngüsü: ~1200 Hz
- Ortalama hız: 4.3 m/s
- Yarışma derecesi: 9.
- En iyi tur süresi: 10.73 saniye

---

## 🧩 Pist Performansı 
- https://photos.app.goo.gl/3YrWBbyk9Cvu7mGE9
- https://photos.app.goo.gl/VAHyJV4Du9QFfPHe9

