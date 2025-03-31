// Definiciones
#define NUM_SUM_ENTEROS 71932200 //con este valor, la suma de enteros tarda 10 segundos con 80mhz de freq
#define NUM_SUM_FLOTANTES 1139322

void sum_int(){
  volatile int resultado = 0;
  unsigned long tiempoInicio, tiempoFin;
  tiempoInicio = millis();  //empezamos a medir el tiempo
  for (int i = 0; i < NUM_SUM_ENTEROS; i++) {
    resultado += i;  // Operación de suma simple
  }
  tiempoFin = millis(); //terminamos de medir
  
  // Calcular tiempo transcurrido
  float tiempoTranscurrido = (tiempoFin - tiempoInicio)/1000.0;
  
  // Mostrar resultados
  Serial.print("Número de sumas de enteros: ");
  Serial.println(NUM_SUM_ENTEROS);
  Serial.print("Tiempo transcurrido (Segundos): ");
  Serial.println(tiempoTranscurrido);
  Serial.println("----------------------------------");
}

void sum_float(){
  volatile float resultado = 0;
  unsigned long tiempoInicio, tiempoFin;
  tiempoInicio = millis();  //empezamos a medir el tiempo
  for (float i = 0; i < NUM_SUM_FLOTANTES; i=i+0.2) {
    resultado += i;  // Operación de suma simple
  }
  tiempoFin = millis(); //terminamos de medir
  
  // Calcular tiempo transcurrido
  float tiempoTranscurrido = (tiempoFin - tiempoInicio)/1000.0;
  
  // Mostrar resultados
  Serial.print("Número de sumas de floats: ");
  Serial.println(NUM_SUM_FLOTANTES);
  Serial.print("Tiempo transcurrido (Segundos): ");
  Serial.println(tiempoTranscurrido);
  Serial.println("----------------------------------");
}
void test_freq(int freq){
  Serial.println();
  setCpuFrequencyMhz(freq); //Configuramos la frecuencia del procesador a "freq"
  Serial.print("Frecuencia configurada en: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" Mhz");
  Serial.println();
  //test suma de enteros
  
  sum_int();
 
  sum_float();
}


void setup() {
  Serial.begin(115200);
  while (!Serial);  // Esperar a que se inicie el puerto serial en algunas placas
  Serial.println("Iniciando Monitor serie...");
  Serial.println();
  test_freq(80);
  delay(2000);
  test_freq(160);
  delay(2000);
  test_freq(240);
}

void loop() {
 
}