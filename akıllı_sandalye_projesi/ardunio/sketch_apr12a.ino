#include <WiFi.h>

const char* ssid = "VODAFONE_8596";  // Wi-Fi ağ adı
const char* password = "H3esufckQSGTKtTN"; // Wi-Fi şifresi

WiFiServer server(80); // 80 portu üzerinden çalışacak

IPAddress local_IP(192, 168, 1, 6);  // ESP32 için sabit IP adresi
IPAddress gateway(192, 168, 1, 1);  // Gateway (router) IP
IPAddress subnet(255, 255, 255, 0);  // Subnet mask

void setup() {
  Serial.begin(115200);  // Seri port başlat (Arduino ile iletişim için)

  // Statik IP ayarlarını yapıyoruz
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("WiFi statik IP ayarı başarısız oldu.");
  }

  WiFi.begin(ssid, password);

  // Wi-Fi bağlantısı
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    Serial.print("...");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Wi-Fi bağlantısı tamam!");
    Serial.print("IP Adresi: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Wi-Fi'ye bağlanılamadı.");
    ESP.restart();  // Bağlantı sağlanamazsa yeniden başlat
  }

  server.begin();  // HTTP server başlat
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Yeni bir bağlantı var!");
    String request = client.readStringUntil('\r');
    Serial.println("Gelen istek: " + request);

    // Komut kontrolü
    if (request.indexOf("command=geri") != -1) {
      Serial.println("Komut: Geri");
      Serial.println("geri");  // Arduino'ya komut gönder
    } else if (request.indexOf("command=sag") != -1) {
      Serial.println("Komut: Sağ");
      Serial.println("sag");
    } else if (request.indexOf("command=dur") != -1) {
      Serial.println("Komut: Dur");
      Serial.println("dur");
    } else if (request.indexOf("command=sol") != -1) {
      Serial.println("Komut: Sol");
      Serial.println("sol");
    } else if (request.indexOf("command=ileri") != -1) {
      Serial.println("Komut: İleri");
      Serial.println("ileri");
    }

    // Yanıt gönder
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println();
    client.println("Komut alındı!");

    delay(1);
    client.stop();
  }
}
