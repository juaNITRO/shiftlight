
//Este proyecto es un contador de rpm para fabricar unas luces de cambio con una tira LED direccionable
//Se ha realizado para una tira de 18 LEDs RGB WS2812B
//La señal de RPM en este caso se cogerá de la señal del tacómetro que envia la ECU al cuadro de instrumentos
//En el caso del Megane esta señal es una onda cuadrada que varia su frecuencia 1Hz son 30 rpm ----> a 3000 rpm la señal es de 100Hz (2 Pulsos por revolucion del cigüeñal)
//PetrolheadGarage ni ninguno de sus integrantes será responsable de cualquier daño producido por este código, tutorial y/o vídeo relacionado

#define RPM1 3000                     //rpm para encender el 1er LED
#define RPM2 4000                     //rpm para encender el 2do LED
#define RPM3 5000                     //rpm para encender el 4to LED
#define RPM4 6500                     //rpm para encender el 6to LED
#define PARPADEO 55                   //ms de duracion del parpadeo 

#include <Adafruit_NeoPixel.h>        //Incluimos libreria Neopixel de Adafruit
#ifdef __AVR__                        //Necesario para la tira de led con Arduino
#include <avr/power.h>
#endif

# define PIN 6                        //Pin de datos al que conectamos la tira led
# define NUMPIXELS 20                 //numero de leds que vamos a controlar (1er led sera el numero 0 y ultimo sera NUMPIXELS-1)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); //Creamos el objeto "pixels" para la tira de led WS2812B

volatile int rpm = 0;                 //Variable donde se guardarán las rpm
unsigned long previousmicros = 0;     //Variable donde guardaremos el tiempo anterior para contabilizar la frecuencia
unsigned long currentmicros = 0;      //Variable donde guardaremos el tiempo actual para contabilizar la frecuencia
unsigned long periodo = 30000000;     //Variable donde guardaremos la duracion entre interrupciones. Iniciamos en 30000000 para que el primer calculo de rpm no sea infinito
unsigned long millis_anteriores = 0;  //Variable donde guardaremos el tiempo anterior para el parpadeo
boolean onoff = false;                //Variable para el parpadeo de LEDs. Se utilizará para cambiar de estado la tira de LEDs entre encendido y apagado

//unsigned long contadorloop = 0;           //Variable utilizada para saber los loops/s que realiza el microcontrolador. (Solo utilizar para observar el funcionamiento del codigo)
//unsigned long millis_anterioresloop = 0;  //Variable utilizada para saber los loops/s que realiza el microcontrolador. (Solo utilizar para observar el funcionamiento del codigo)

void setup() {
  pixels.begin();                   //Inicializamos la tira led
  pixels.clear();                   //Apagamos todos los LED
  //Serial.begin(115200);             //Iniciamos puerto serie.(Solo utilizar para observar el funcionamiento del codigo por puerto serie)
  attachInterrupt(0, RPM, FALLING); //Activamos la interrupcion 0 (pin 2 en el Arduino)
}

void RPM() {                        //Funcion a realizar en cada interrupcion externa del pin 2
  currentmicros = micros();         //Almacenamos los us transcurridos en la variable currentmicros
  periodo = currentmicros - previousmicros; //El periodo de la señal serán los us actuales menos los us anteriores
  previousmicros = currentmicros;   //Igualamos us actuales a us anteriores para calcular el proximo periodo
}

void loop() {
  unsigned long millis_actuales = millis(); //Almacenamos los ms actuales para el parpadeo de los leds
  rpm = 30000000 / periodo;                 //Calculamos las rpm. La señal en el Megane es ralenti~750rpm = 25Hz, 3000rpm = 100Hz ---> 1Hz = 30rpm. "periodo" estará en us
  //Serial.print(periodo);                  //Mandamos al puerto serie el valor del periodo entre señales en us
  //Serial.print(" us    ");
  //Serial.print(rpm);                      //Mandamos al puerto serie el valor de rpm
  //Serial.println(" rpm");

  /*Contador de loops por segundo del codigo. (solo usar para observar el funcionamiento por puerto serie)*/

  /*unsigned long millis_actualesloop = millis();
    contadorloop++;                             //Contador de loops/s que realiza el microcontrolador. Solo para ver la velocidad de la que disponemos
    if(millis_actualesloop-millis_anterioresloop>1000)  //Muestra los loops/s por el puerto serie
    {
    Serial.print(contadorloop);               //Enviamos al puerto serie los loops/s que está realizando el microcontrolador
    Serial.println("  loops/s");
    millis_anterioresloop = millis_actualesloop;      //Igualamos millisactuales a millisanteriores para la proxima medicion de loops
    contadorloop = 0;                         //Reseteamos el contador para la próxima medición de loops
    }*/

  /*Empezamos con el encendido de leds dependiendo de las rpm*/

  if (rpm < RPM1) {                   //Apagamos todos los LEDs con rpm menor que RPM1
    pixels.clear();
    pixels.show();
  }

  // como las rpm y los leds son directamente proporcionales, 
  // osea a mayor rpm mas leds encendidos. el valor de rpm de puede mapear oara obtener la posicion del led.
  // mapeamos de 3000 a 4000 rpm entre los led del 1 al 6. Entonces si las rpm estan entre 3000 y 3200 se mapea el led 1,
  // si las rpm estan entre 3200 y 3400 es el led 2. y asi sucesivamente.   
  // Además entre los colores rgb tambien hay cierta proporción que se puede mapear

/*---------------------------------------------------------------------------------------------------------------------------------------------*/

  // RGB verde comienza en 0,255,0 y termina en 160,255,0 - aqui se ve que sólo se mueve el valor de R. Tambien es mapeable
  // Amararillo y naranjo son valores intermedios entre R y G tambien se pueden mapear. 
  // RGB Rojo comienza en 255,100,0 aproximadamente y termina en 255,0,0 - aqui se ve que sólo se mueve el valor de G. Tambien es mapeable

  // pixels.Color(map(rpm, RPM1, RPM2, 0, 190), 255, 0) - Usando la función map se puede comenzar con un color verde y terminar con un color llegando al amarillo
  // Para el parpadeo se puede utilizar la función .fill para darle el mismo color a todos los leds. Ahorrando varias lineas de código  
   
/*---------------------------------------------------------------------------------------------------------------------------------------------*/

  //entre 3000 y 4000
  if (rpm >= RPM1 && rpm < RPM2) {
    pixels.setPixelColor(map(rpm, RPM1, RPM2, 1, 6), pixels.Color(map(rpm, RPM1, RPM2, 0, 190), 255, 0));
    pixels.show();
  }

  //entre 4000 y 5000
  if (rpm >= RPM2 && rpm < RPM3) {
    pixels.setPixelColor(map(rpm, RPM2, RPM3, 7, 12), pixels.Color(map(rpm, RPM2, RPM3, 190, 255), 255, 0));
    pixels.show();
  }

  //entre 5000 y 6400
  if (rpm >= RPM3 && rpm < (RPM4 - 100)) {
    pixels.setPixelColor(map(rpm, RPM3, (RPM4 - 100), 13, 19), pixels.Color(255, map(rpm, RPM3, (RPM4 - 100), 255, 0), 0));
    pixels.show();
  }

  if (rpm >= RPM4) {
    if (millis_actuales - millis_anteriores > PARPADEO) {
      onoff = !onoff;
      millis_anteriores = millis_actuales;
      if (onoff == false) {
        pixels.fill(pixels.Color(255, 0, 0), 1, 19);
        pixels.show();
      } else {
        pixels.clear();
        pixels.show();
      }
    }
  }
  delay(20);
}
