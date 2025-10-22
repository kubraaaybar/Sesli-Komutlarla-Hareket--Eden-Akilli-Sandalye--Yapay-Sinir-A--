import sounddevice as sd
import numpy as np
import librosa
import noisereduce as nr
import tensorflow as tf
import time
import serial
from scipy.io.wavfile import write

# 🎯 Model ve Arduino ayarları
model_path = "C:\\Users\\Lenovo\\OneDrive\\Masaüstü\\ses_tanima\\komut.h5"
model = tf.keras.models.load_model(model_path)

# 🎯 Arduino Seri Portu (Port ismini kendi sisteminize göre güncelleyin)
arduino_port = "COM5"  # ⛔️ PORT ismini kendi sisteminize göre değiştirin!
arduino = serial.Serial(arduino_port, 9600, timeout=1)  # Arduino ile bağlantı

# 🎯 Komut eşleştirme
label_mapping = {
    0: "geri",
    1: "sag",
    2: "dur",
    3: "sol",
    4: "ileri"
}

arduino_codes = {
    "geri": 4,    # Geri hareket için motor pinleri
    "sag": 5,     # Sağ dönüş için motor pinleri
    "dur": 6,     # Duruş için motor pinleri
    "sol": 7,     # Sol dönüş için motor pinleri
    "ileri": 8    # İleri hareket için motor pinleri
}

# 🎙️ Ses kayıt ayarları
duration = 2.5
sample_rate = 22050
temp_wav_path = "temp.wav"


def record_audio():
    print("🎤 Konuşun... Kayıt yapılıyor...")
    audio = sd.rec(int(duration * sample_rate), samplerate=sample_rate, channels=1, dtype='float32')
    sd.wait()
    audio_int16 = np.int16(audio * 32767)
    write(temp_wav_path, sample_rate, audio_int16)
    print("✅ Kayıt tamamlandı!")
    return temp_wav_path


def extract_features(file_path):
    try:
        # Ses dosyasını yükle ve örnekleme oranını 22050 Hz'e ayarla
        audio, sample_rate = librosa.load(file_path, sr=22050)
        
        # Gürültü azaltma işlemi uygula
        audio = nr.reduce_noise(y=audio, sr=sample_rate)
        
        # Baş ve sondaki sessiz kısımları kırp
        audio, _ = librosa.effects.trim(audio, top_db=20)
        
        # Ses verisini normalize et (değerleri -1 ile 1 arasına çeker)
        audio = librosa.util.normalize(audio)
        
        # MFCC (Mel Frekans Kepstral Katsayıları) çıkar, 40 adet
        mfccs = librosa.feature.mfcc(y=audio, sr=sample_rate, n_mfcc=40)
        
        # MFCC'lerin zaman uzunluğunu sabitlemek için hedef uzunluğu belirle
        max_pad_len = 100
        
        # Eksik zaman adımı sayısını hesapla
        pad_width = max_pad_len - mfccs.shape[1]
        
        if pad_width > 0:
            # Eğer MFCC uzunluğu 100'den kısaysa, eksik kısmı sıfırlarla doldur
            mfccs = np.pad(mfccs, pad_width=((0, 0), (0, pad_width)), mode='constant')
        else:
            # Eğer MFCC uzunluğu 100'den uzunsa, ilk 100 zaman adımını al
            mfccs = mfccs[:, :max_pad_len]
        
        # MFCC'yi modele uygun boyuta getir (1 örnek, 40 öznitelik, 100 zaman adımı)
        return mfccs.reshape(1, 40, 100)
    
    except Exception as e:
        # Hata oluşursa kullanıcıya bildir ve None döndür
        print(f"❌ Hata! Ses dosyası işlenemedi: {e}")
        return



def predict_command():
    # 1. Kullanıcıdan ses kaydı al (kayıt yap ve geçici .wav dosyasına kaydet)
    file_path = record_audio()

    # 2. Kaydedilen ses dosyasından özellik (MFCC) çıkar
    features = extract_features(file_path)

    if features is not None:
        # 3. Özellikleri kullanarak modeli çalıştır ve tahmin al
        prediction = model.predict(features)

        # 4. Tahmin sonucunda en yüksek olasılığa sahip komutu belirle
        command = label_mapping[np.argmax(prediction)]

        # 5. Komutu ekrana yazdır
        print(f'🟢 Tahmin Edilen Komut: {command}')

        # 6. Komutu Arduino'ya gönder (örneğin seri port ile)
        send_to_arduino(command)

        # 7. Komutu döndür (gerekirse başka yerde kullanmak için)
        return command
    else:
        # Özellik çıkarımı başarısız olduysa uyarı ver
        print("⚠️ Komut tahmin edilemedi.")
        return None


# 🎯 Arduino'ya komut gönderme fonksiyonu
def send_to_arduino(command):
    pin = arduino_codes.get(command)
    if pin:
        # Komutu Arduino'ya gönder
        arduino.write(str(pin).encode())  # Arduino'ya pin numarasını gönder
        print(f"📤 Komut Arduino'ya gönderildi: {command}")
    else:
        print("❌ Bilinmeyen komut.")


# 🔁 Sürekli dinleme döngüsü
while True:
    predict_command()
    time.sleep(5)  # Sürekli dinleme için kısa bir bekleme süresi
