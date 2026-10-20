//Práctica 2. Configuración de entradas y salidas digitales.

#define BUTTON_PIN 21 //GPIO21 para el boton
#define ledPin 18 //GPIO18 para el LED

//Declaración de variable utilizadas en el proceso

//Estado del boton
int btn_currentState; //Estado actual del boton
int btn_lastState = HIGH; //Ultimo estado del boton
bool ledState = false;

//Variables para control de tiempo
unsigned long previousMillis = 0;

//Marcas de tiempos
unsigned long start_time=0;//Contador en milisegundos
unsigned long interval =150;

//Contador auxiliar
int flag = 0;

//Input serial para cambio de modos
int ledMode; // 1.Encendido/Apagado, 2.Off after 3 sec, 
//3.Toogle , 4.Destello, 5. 2sec light, 6. Christmas light

void setup(){
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop(){
  // Leer comandos desde el monitor serial
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "onoff") ledMode = 1;
    else if (command == "off after 3sec") ledMode = 2;
    else if (command == "Toogle") ledMode = 3;
    else if (command == "Destello") ledMode = 4;
    else if (command == "2sec light") ledMode = 5;
    else if(command == "Christmas") ledMode = 6;
    Serial.println("Modo cambiado a: " + command);
  }

//leer estado del boton
btn_currentState=digitalRead(BUTTON_PIN);

//cambio de modos 
switch(ledMode){
  case 1:   
  if (btn_lastState == HIGH && btn_currentState == LOW){
    Serial.print("LED Encendido\n");
    digitalWrite(ledPin, HIGH);
  }
  else if (btn_lastState == LOW && btn_currentState == HIGH){
    Serial.print("LED Apagado\n");
    digitalWrite(ledPin, LOW);
  }
  break;
  case 2: 
    if (btn_lastState == HIGH && btn_currentState == LOW){
    Serial.print("LED Encendido\n");
    digitalWrite(ledPin, HIGH);
    delay(3000);
    digitalWrite(ledPin, LOW);
  }
  break; 
  case 3:
   if (btn_lastState == HIGH && btn_currentState == LOW) {
    // alternar estado de LED
    ledState = !ledState;

    // controlar la codificación LED en el state  
    digitalWrite (ledPin, ledState);  }
  break;
  case 4:
    if (btn_currentState == LOW){
    digitalWrite(18, HIGH); // turn the LED on
    delay(150);             // wait for 500 milliseconds
    digitalWrite(18, LOW);  // turn the LED off
    delay(150);
  }else {
    digitalWrite(18, LOW);
  }
  break;
  case 5:
  if (btn_currentState == LOW){
     if (start_time == 0) {
      start_time = millis(); // marcar inicio de la presión
     }

  // calcular cuánto tiempo lleva presionado
  unsigned long elapsed = millis() - start_time;

  if (elapsed<2000){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(18 , ledState);
      }
    }else if (elapsed>2000){
      digitalWrite(18 , HIGH);
      }
  }else{
    start_time=0;
    digitalWrite(18, LOW);
  }
  break;
  case 6:
    // Detectar transición de botón presionado
  if (btn_lastState == HIGH && btn_currentState == LOW) {
    flag++;
    if (flag > 3) flag = 0; // reinicia ciclo
    Serial.print("Modo cambiado a: ");
    Serial.println(flag);
  }

  switch (flag){
    case 0:
    digitalWrite(18,LOW);
    break;
    case 1:
    digitalWrite(18, HIGH);//Led encendido
    break;
    case 2:
    if (millis() - previousMillis >= 500) {
      previousMillis = millis();
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
    break;
    case 3:
    if (millis() - previousMillis >= 50) {
      previousMillis = millis();
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
    break;
    case 4:
    digitalWrite(18, LOW);
    break;
    }
  break;
}
  btn_lastState = btn_currentState;
  delay(200);
}