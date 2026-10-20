/*
  ESP32 DevKit V1 - 3 LED con PWM
  Rojo, verde y azul
  Arduino-ESP32 version 3.x

  Conexiones:
  GPIO 18 -> LED rojo
  GPIO 19 -> LED verde
  GPIO 21 -> LED azul

  Para cátodo común (cátodos a GND): invert = false
  Para ánodo conectado a 3.3V: invert = true
*/

const int ledR = 18;
const int ledG = 19;
const int ledB = 21;

// Cambia según tus conexiones:
const bool invert = false;

const int frecuencia = 5000;
const int resolucion = 8;

void setup() {
  Serial.begin(115200);

  ledcAttach(ledR, frecuencia, resolucion);
  ledcAttach(ledG, frecuencia, resolucion);
  ledcAttach(ledB, frecuencia, resolucion);

  apagarLeds();

  Serial.println("Nyaa~ Sistema RGB iniciado!");
}

void escribirLED(int pin, int brillo) {
  brillo = constrain(brillo, 0, 255);

  if (invert) {
    brillo = 255 - brillo;
  }

  ledcWrite(pin, brillo);
}

void mostrarColor(int rojo, int verde, int azul) {
  escribirLED(ledR, rojo);
  escribirLED(ledG, verde);
  escribirLED(ledB, azul);
}

void apagarLeds() {
  mostrarColor(0, 0, 0);
}

void loop() {
  // 1. Todos encendidos
  Serial.println("Todos encendidos");
  mostrarColor(255, 255, 255);
  delay(2000);

  // 2. Todos apagados
  Serial.println("Todos apagados");
  apagarLeds();
  delay(1000);

  // 3. Transición suave entre colores
  Serial.println("Iniciando arcoiris");

  for (int hue = 0; hue < 1536; hue++) {
    int sector = hue / 256;
    int paso = hue % 256;

    int inverso = 255 - paso;

    switch (sector) {
      case 0:
        mostrarColor(255, paso, 0);
        break;
      case 1:
        mostrarColor(inverso, 255, 0);
        break;
      case 2:
        mostrarColor(0, 255, paso);
        break;
      case 3:
        mostrarColor(0, inverso, 255);
        break;
      case 4:
        mostrarColor(paso, 0, 255);
        break;
      default:
        mostrarColor(255, 0, inverso);
        break;
    }

    delay(10);
  }

  // 4. Pausa antes de repetir
  apagarLeds();
  delay(1000);
}