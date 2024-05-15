#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); // Configuração do LCD

String senha = "2222"; // Senha padrão
String buf = ""; // Buffer para entrada do teclado

// Configuração do teclado 3x4
const byte LINHAS = 4;
const byte COLUNAS = 3;
char KEYS[LINHAS][COLUNAS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pino_linha[LINHAS] = {2, 3, 4, 5}; // Pinos das linhas
byte pino_coluna[COLUNAS] = {6, 7, 8}; // Pinos das colunas

Keypad keypad = Keypad(makeKeymap(KEYS), pino_linha, pino_coluna, LINHAS, COLUNAS);

Servo servo_9; // Servo para controlar a fechadura

// Configuração dos LEDs e Buzzer
int greenLedPin = 10; // LED verde para acesso liberado
int redLedPin = 11; // LED vermelho para acesso negado
int buzzerPin = 12; // Buzzer para sinalizar acesso liberado

void setup() {
  lcd.begin(16, 2); // Inicializa o LCD
  lcd.setCursor(0, 0); // Define o cursor na primeira linha
  lcd.print("DIGITE A SENHA"); // Mensagem inicial
  
  servo_9.attach(9); // Conecta o servo ao pino 9
  servo_9.write(0); // Mantém a fechadura fechada

  pinMode(greenLedPin, OUTPUT); // Configura LED verde como saída
  pinMode(redLedPin, OUTPUT); // Configura LED vermelho como saída
  pinMode(buzzerPin, OUTPUT); // Configura buzzer como saída

  digitalWrite(greenLedPin, LOW); // Apagado no início
  digitalWrite(redLedPin, LOW); // Apagado no início
}

void loop() {
  char key = keypad.getKey(); // Captura a tecla pressionada
  if (key != NO_KEY) {
    switch (key) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        lcd.setCursor(buf.length(), 1); // Cursor na segunda linha
        lcd.print(key); // Mostra a tecla no LCD
        buf += key; // Adiciona ao buffer
        break;

      case '': // Limpa o buffer ao pressionar ''
        buf = "";
        lcd.setCursor(0, 1); // Cursor na segunda linha
        lcd.print("               "); // Limpa a segunda linha
        break;

      case '#': // Verifica a senha ao pressionar '#'
        if (buf == senha) { // Se a senha estiver correta
          lcd.setCursor(0, 1); // Define o cursor na segunda linha
          lcd.print("ACESSO LIBERADO"); // Exibe mensagem
          digitalWrite(greenLedPin, HIGH); // Acende LED verde
          digitalWrite(redLedPin, LOW); // Apaga LED vermelho
          digitalWrite(buzzerPin, HIGH); // Buzzer toca por 1 segundo
          delay(1000);
          digitalWrite(buzzerPin, LOW);
          servo_9.write(90); // Abre a fechadura
          delay(6000); // Mantém aberto por 6 segundos
          servo_9.write(0); // Fecha a fechadura
          digitalWrite(greenLedPin, LOW); // Apaga LED verde
        } else { // Se a senha estiver errada
          lcd.setCursor(0, 1); // Define o cursor na segunda linha
          lcd.print("ACESSO NEGADO");
          digitalWrite(greenLedPin, LOW); // Apaga LED verde
          digitalWrite(redLedPin, HIGH); // Acende LED vermelho
          delay(2000);
          digitalWrite(redLedPin, LOW);
          servo_9.write(0); // Mantém a fechadura fechada
        }
        buf = ""; // Limpa o buffer
        delay(2000); // Espera antes de limpar a mensagem
        lcd.setCursor(0, 1); // Define o cursor na segunda linha
        lcd.print("               "); // Limpa a segunda linha
        break;
    }
  }

  delay(100); // Pequena pausa para evitar sobrecarga do Arduino
}