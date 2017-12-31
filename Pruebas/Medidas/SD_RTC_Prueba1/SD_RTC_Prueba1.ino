#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>

RTC_DS3231 rtc;

String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
String monthsNames[12] = { "Enero", "Febrero", "Marzo", "Abril", "Mayo",  "Junio", "Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre" };
 
void setup() {
//Leo: inicialización
Serial.begin(9600);

//Configuración RCT



//Comprobar conexión con RTC
if (!rtc.begin()) {
  Serial.println(F("")); //Sustituir por patron reconocible en LED
  return;}

//Fijar fecha y hora si se pierde la corriente
if (rtc.lostPower()){
  // Fijar a fecha y hora de compilacion
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));}

//Comprobar conexión con SD
if (!SD.begin(4)){
  Serial.println(F("Error al iniciar SD"));
  return;
}

}//Fin del setup

// Fecha en formato String:
String getStringDate(DateTime date){

  String y = String(date.year());
  String m = monthsNames[date.month()];
  String d = String(date.day());
  String DoW = String(daysOfTheWeek[date.dayOfTheWeek()]);
  
  String fecha = DoW+" "+d+" "+m+" "+y;
  return fecha;
  }

String getStringDateNumber(DateTime date){
  String y = String(date.year());
  String m = String(date.month());
  String d = String(date.day());

 return y+m+d;
}

// Hora en formato String:
String getStringTime(DateTime date){
  String hh = String(date.hour());
  String mm = String(date.minute());
  String ss = String(date.second());

  String hora = hh+":"+mm+":"+ss;
  return hora;  
  }

void writeDataToSD(float dato, String nombre){
  //Provisional
  Serial.println(dato);
  DateTime hora = rtc.now();
  String linea = getStringTime(rtc.now()) + " " + String(dato)+" W";
  
  //Se abre el archivo. Si no existe, se crea:
  File archivo = SD.open(nombre, FILE_WRITE);
  //Se escribe la linea con la lectura
  archivo.println(linea);
  //Se cierra el archivo para garantizar el guardado de los datos
  archivo.close();
  Serial.println(linea);
 }

float getMeassurement(){
  long tiempo = millis(); 
  int contador = 0;
  float sum = 0;
  float valor = 0;

  while(millis() - tiempo < 15000){
    valor = 2 + random(0,5);
    sum += sq(valor);
    contador++;
    delay(1);
  }
  return sqrt(sum/contador);

}

float data;
String nombreArchivo;
void loop() {
//Leo: 
  //obtener medida
  data = getMeassurement();
  Serial.println(data);
  //Calcular fecha actual para determinar nombre de archivo
  nombreArchivo = getStringDateNumber(rtc.now())+".txt";
  Serial.println(nombreArchivo);
  //escribir medida
  writeDataToSD(data, nombreArchivo);


}// Fin del loop
