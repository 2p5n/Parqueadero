#include <Servo.h>           // Biblioteca para controlar servomotores
#include <Wire.h>            // Biblioteca para comunicación I2C
#include <LiquidCrystal_I2C.h> // Biblioteca para LCD con interfaz I2C

// Configura LCD dirección I2C 0x27, 16 columnas y 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Definición de pines
int entrada_infraroja = 2;   // Pin del sensor infrarrojo de ENTRADA
int salida_infraroja = 3;    // Pin del sensor infrarrojo de SALIDA
int ledEntradaPin = 5;       // LED indicador de entrada
int ledSalidaPin = 6;        // LED indicador de salida
int servoPin_entrada = 9;    // Pin del servomotor de entrada
int servoPin_salida = 10;    // Pin del servomotor de salida

// Objetos para controlar los servomotores
Servo miServo_entrada;       // Servo de la barrera de entrada
Servo miServo_salida;        // Servo de la barrera de salida

// Variables de estado del parqueadero
int espaciosDisponibles = 12; // Capacidad total de espacios
int cuantosHayOcupados = 0;   // Contador de ocupados

// Variables para detección de flancos (cambios de estado)
bool estadoPrevioEntrada = LOW; // Estado anterior sensor entrada
bool estadoPrevioSalida = LOW;  // Estado anterior sensor salida

void setup() {
  Serial.begin(9600);         // Inicia comunicación serial para depuración

  lcd.init();                 // Inicializa la pantalla LCD
  lcd.backlight();            // Activa la retroiluminación del LCD

  // Configuración de pines
  pinMode(entrada_infraroja, INPUT_PULLUP); // Sensor con resistencia pull-up interna
  pinMode(salida_infraroja, INPUT_PULLUP);
  pinMode(ledEntradaPin, OUTPUT);     // LED de entrada como salida
  pinMode(ledSalidaPin, OUTPUT);      // LED de salida como salida

  // Vincula servomotores a sus pines correspondientes
  miServo_entrada.attach(servoPin_entrada);
  miServo_salida.attach(servoPin_salida);

  // Mensaje inicial en LCD
  lcd.print("Proyecto Bimestral"); // Línea 1: Texto estático
  lcd.setCursor(0, 1);             // Mueve cursor a segunda fila
  lcd.print("Parqueadero");        // Línea 2: Texto estático
  delay(2000);                     // Muestra mensaje por 2 segundos
  updateLCD();                     // Actualiza a información dinámica
}

void loop() {
  // Lectura de sensores en cada iteración
  int value_entrada = digitalRead(entrada_infraroja);
  int value_salida = digitalRead(salida_infraroja);

  // Lógica de ENTRADA de vehículos
  if (value_entrada == HIGH && estadoPrevioEntrada == LOW) { // Detección flanco ascendente
    digitalWrite(ledEntradaPin, HIGH);  // Enciende LED de entrada
    if (espaciosDisponibles > 0) {      // Si hay espacios disponibles
      miServo_entrada.write(0);         // Abre barrera (0 grados)
      delay(2500);                      // Espera 2.5 segundos
      miServo_entrada.write(90);        // Cierra barrera (90 grados)
      espaciosDisponibles--;            // Reduce espacios libres
      cuantosHayOcupados++;             // Aumenta ocupación
      updateLCD();                      // Actualiza display
    }
    estadoPrevioEntrada = HIGH;         // Actualiza estado previo
  } else if (value_entrada == LOW) {    // Cuando el sensor se libera
    digitalWrite(ledEntradaPin, LOW);   // Apaga LED
    estadoPrevioEntrada = LOW;          // Reinicia estado
  }

  // Lógica de SALIDA de vehículos
  if (value_salida == HIGH && estadoPrevioSalida == LOW) {  // Flanco ascendente
    digitalWrite(ledSalidaPin, HIGH);   // Enciende LED de salida
    if (cuantosHayOcupados > 0) {       // Si hay vehículos estacionados
      miServo_salida.write(0);          // Abre barrera
      delay(2500);                      
      miServo_salida.write(90);         // Cierra barrera
      espaciosDisponibles++;            // Libera espacio
      cuantosHayOcupados--;             // Reduce ocupación
      updateLCD();
    }
    estadoPrevioSalida = HIGH;          // Actualiza estado
  } else if (value_salida == LOW) {     // Sensor liberado
    digitalWrite(ledSalidaPin, LOW);    // Apaga LED
    estadoPrevioSalida = LOW;           // Reinicia estado
  }

  delay(200);  // Pequeña pausa para estabilidad en lecturas
}

// Función para actualizar la pantalla LCD
void updateLCD() {
  lcd.clear();                // Limpia la pantalla
  lcd.setCursor(0, 0);        // Posiciona en primera fila
  lcd.print("Libres: ");      // Texto estático
  lcd.print(espaciosDisponibles); // Valor dinámico
  lcd.setCursor(0, 1);        // Segunda fila
  lcd.print("Ocupados: ");    
  lcd.print(cuantosHayOcupados); 
}