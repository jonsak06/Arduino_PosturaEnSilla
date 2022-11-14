#define BOTON D1 //asigna el nombre BOTON a la entrada/salida digital D1 de la placa
#define LED_ROJO D2 //asigna el nombre LED_ROJO a la entrada/salida digital D2 de la placa
#define LED_VERDE D3 //asigna el nombre LED_VERDE a la entrada/salida digital D3 de la placa

enum Estado {A, B, C};
////// Descripción de estados //////
// A = LED_ROJO OFF, LED_VERDE OFF
// B = LED_ROJO OFF, LED_VERDE ON
// C = LED_ROJO ON, LED_VERDE OFF
////////////////////////////////////

///////////////////////////////////////////////////////// Variables a utilizarse /////////////////////////////////////////////////////////
Estado estadoActual; //Estado en el que se encuentra el sistema

//variables para controlar el tiempo necesario para la transición de etapa B a C (5 segundos) y de etapa C a A (10 segundos)
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

bool tiempoRegistrado = false; //variable booleana para controlar si tiempo1 fue asignado
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////// Funciones a ejecutarse en los distintos estados //////////////////////////////////////////////
void fnEstadoA() {
  if(digitalRead(BOTON) == LOW) {
    digitalWrite(LED_VERDE, HIGH);
    estadoActual = B;
  }
}

void fnEstadoB() {
  if(digitalRead(BOTON) == HIGH) {
    if(!tiempoRegistrado) {
      tiempo1 = millis();
      tiempoRegistrado = true; 
    }

    tiempo2 = millis();

    if(tiempo2 >= tiempo1+5000) {
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_ROJO, HIGH);
      tiempoRegistrado = false;
      estadoActual = C;
    }
  }

  if(digitalRead(BOTON) == LOW) {
    tiempoRegistrado = false;
  }
}

void fnEstadoC() {
  if(digitalRead(BOTON) == LOW) {
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_VERDE, HIGH);
    tiempoRegistrado = false;
    estadoActual = B;
  }

  if(digitalRead(BOTON) == HIGH) {
    if(!tiempoRegistrado) {
      tiempo1 = millis();
      tiempoRegistrado = true;
    }

    tiempo2 = millis();

    if(tiempo2 >= tiempo1+10000) {
      digitalWrite(LED_ROJO, LOW);
      estadoActual = A;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(9600);
  pinMode(BOTON, INPUT);
  pinMode(LED_ROJO,OUTPUT);
  pinMode(LED_VERDE,OUTPUT);

  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
  estadoActual = A;
}

void loop() {
  switch(estadoActual) {
    case A:
      fnEstadoA();
      break;
    case B:
      fnEstadoB();
      break;
    case C:
      fnEstadoC();
      break;
  }
}
