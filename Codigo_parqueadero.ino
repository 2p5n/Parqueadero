//Librerias
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C del módulo LCD

int entrada_infraroja = 2;
int entrada_infraroja_3 = 3;  // Nuevo pin del sensor infrarrojo
int servoPin = 9;
int servoPin_10 = 10;  // Nuevo pin del servo motor

Servo miServo;
Servo miServo_10;  // Nueva instancia del servo motor

int espaciosDisponibles = 12;
int cuantosHayOcupados = 0;

void setup() {
  lcd.init();                      // Inicializar la pantalla LCD
  lcd.backlight();                 // Encender la retroiluminación

  pinMode(entrada_infraroja, INPUT);
  pinMode(entrada_infraroja_3, INPUT);  // Establecer el nuevo pin del sensor infrarrojo como entrada
  miServo.attach(servoPin);
  miServo_10.attach(servoPin_10);  // Inicializar el nuevo servo motor en el nuevo pin

  lcd.print("Proyecto bimestral");
  lcd.setCursor(0, 1);
  lcd.print("Parqueadero");
  delay(2000);
  lcd.clear();
  updateLCD();  // Mostrar el estado inicial en la pantalla LCD
}

void loop() {
  int value_entrada = digitalRead(entrada_infraroja);
  int value_entrada_3 = digitalRead(entrada_infraroja_3);

  // Operar el servo de entrada solo si hay espacios disponibles
  if (value_entrada == HIGH && espaciosDisponibles > 0) {
    miServo.write(90);  // Mover el servo de entrada a 90 grados (barrera arriba)
    delay(2500);        // Esperar 2.5 segundos
    miServo.write(0);   // Regresar el servo a 0 grados (barrera abajo)
    espaciosDisponibles--; // Decrementar los espacios disponibles
    cuantosHayOcupados++;  // Incrementar los espacios ocupados
    updateLCD();           // Actualizar la pantalla LCD
  }

  // Operar el servo de salida solo si hay espacios ocupados
  if (value_entrada_3 == HIGH && cuantosHayOcupados > 0) {
    miServo_10.write(90);  // Mover el servo de salida a 90 grados (barrera arriba)
    delay(2500);           // Esperar 2.5 segundos
    miServo_10.write(0);   // Regresar el servo a 0 grados (barrera abajo)
    espaciosDisponibles++; // Incrementar los espacios disponibles
    cuantosHayOcupados--;  // Decrementar los espacios ocupados
    updateLCD();           // Actualizar la pantalla LCD
  }
}

void updateLCD() {
  lcd.clear();             // Borrar la pantalla LCD
  lcd.setCursor(0, 0);     // Establecer el cursor en la primera fila
  lcd.print("Libres: ");   // Mostrar el texto "Libres: "
  lcd.print(espaciosDisponibles); // Mostrar la cantidad de espacios disponibles
  lcd.setCursor(0, 1);     // Establecer el cursor en la segunda fila
  lcd.print("Ocupados: "); // Mostrar el texto "Ocupados: "
  lcd.print(cuantosHayOcupados);  // Mostrar la cantidad de espacios ocupados
}