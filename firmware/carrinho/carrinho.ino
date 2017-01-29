#include <BLE_API.h>
#include <Servo.h>


#define DEVICE_NAME        "CarroBLE"
#define BATTERY_LEVEL_PIN  A5
#define FAROL_PIN          D13
#define INPUT1_MOTOR_PIN   D2
#define INPUT2_MOTOR_PIN   D1
#define ENABLE_MOTOR_PIN   A3
#define SERVO_PIN          D3



// Physical Web
#define EDDYSTONE_SERVICE_UUID 0xFEAA
uint8_t eddystoneData[]  = {
  0xAA, 0xFE,  // Eddystone Service Id
  0x10,        // Frame type: URL
  0xF8,        // Power
  0x03,        // https://
  'g', 'o', 'o', '.', 'g', 'l', '/', 'l', 'p', 'a', 'G','z','W'
};


BLE    ble;
static const uint16_t advertisedServices[] = { GattService::UUID_BATTERY_SERVICE, 0x5100, EDDYSTONE_SERVICE_UUID };

// Battery
static uint8_t levelCounter     = 100;
Ticker             batteryTask;
static uint8_t     batteryLevel[1];
GattCharacteristic batteryLevelChar(GattCharacteristic::UUID_BATTERY_LEVEL_CHAR, batteryLevel, sizeof(batteryLevel), sizeof(batteryLevel), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);
GattCharacteristic *batteryServiceChars[] = {&batteryLevelChar };
GattService        batteryService(GattService::UUID_BATTERY_SERVICE, batteryServiceChars, sizeof(batteryServiceChars) / sizeof(GattCharacteristic *));


// Sinalizacao Farol, setas
#define SINALIZACAO_SERVICE_UUID 0x5200
#define FAROL_CHAR_UUID          0x5201

static uint8_t     farolStatus[1];
GattCharacteristic farolChar(FAROL_CHAR_UUID, farolStatus, sizeof(farolStatus), sizeof(farolStatus), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);
GattCharacteristic *sinalizacaoServiceChars[] = {&farolChar};
GattService        sinalizacaoService(SINALIZACAO_SERVICE_UUID, sinalizacaoServiceChars, sizeof(sinalizacaoServiceChars) / sizeof(GattCharacteristic *));


// Motor
#define MOTOR_SERVICE_UUID      0x6200
#define VELOCIDADE_CHAR_UUID    0x6201
#define RE_CHAR_UUID            0x6202


// -- Caracteristica Velocidade do Motor
static uint8_t     velocidade[1];
static uint8_t     velocidadeAtual;
GattCharacteristic velocidadeChar(VELOCIDADE_CHAR_UUID, velocidade, sizeof(velocidade), sizeof(velocidade), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);

// -- Caracteristica Re
static uint8_t     reStatus[1];
GattCharacteristic reChar(RE_CHAR_UUID, reStatus, sizeof(reStatus), sizeof(reStatus), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);


GattCharacteristic *motorServiceChars[] = {&velocidadeChar,&reChar};
GattService        motorService(MOTOR_SERVICE_UUID, motorServiceChars, sizeof(motorServiceChars) / sizeof(GattCharacteristic *));


// Direcao
#define DIRECAO_SERVICE_UUID 0x7200
#define DIRECAO_CHAR_UUID    0x7201

Servo direcao;

static uint8_t     angulo[1];
static uint8_t     anguloAtual;
GattCharacteristic direcaoChar(DIRECAO_CHAR_UUID, angulo, sizeof(angulo), sizeof(angulo), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);
GattCharacteristic *direcaoServiceChars[] = {&direcaoChar};
GattService        direcaoService(DIRECAO_SERVICE_UUID, direcaoServiceChars, sizeof(direcaoServiceChars) / sizeof(GattCharacteristic *));


void disconnectionCallBack(const Gap::DisconnectionCallbackParams_t *params) {
  ble.startAdvertising();
  Serial.println("Desconectado :-(");
}

void connectionCallBack( const Gap::ConnectionCallbackParams_t *params ) {
  uint8_t index;
  Serial.print("Conectado ao endereco: ");
  for(index=0; index<6; index++) {
    Serial.print(params->peerAddr[index], HEX);
    Serial.print(" ");
  }
}

void atualizaSinalizacao() {
  if (farolStatus[0] == 1) {
     Serial.println("Farol Ligado");
     digitalWrite(FAROL_PIN, LOW);
  } else {
     Serial.println("Farol Desligado");
     digitalWrite(FAROL_PIN, HIGH);
  }
}

void atualizaVelocidadeMotor() {

  velocidadeAtual = velocidadeAtual + velocidade[0];
  analogWrite(ENABLE_MOTOR_PIN, velocidadeAtual);
  Serial.println("Velocidade:");
  Serial.println(velocidadeAtual);
}

void atualizaReMotor() {
  
  if (reStatus[0] == 1) {
     digitalWrite(INPUT1_MOTOR_PIN, LOW);
     digitalWrite(INPUT2_MOTOR_PIN, HIGH);
     Serial.println("Andando de re");
  } else {
     digitalWrite(INPUT1_MOTOR_PIN, HIGH);
     digitalWrite(INPUT2_MOTOR_PIN, LOW);
     Serial.println("Andando para frente");
  }
}

void atualizaDirecao() {
  anguloAtual = anguloAtual + angulo[0];
  direcao.write(anguloAtual);
  Serial.println("Angulo da direcao");
  Serial.println(anguloAtual);
}

void gattServerWriteCallBack(const GattWriteCallbackParams *params) {
  
  if (params->handle == farolChar.getValueAttribute().getHandle()) {
    memcpy(farolStatus, params->data, params->len);
    atualizaSinalizacao();
  }
  
  if (params->handle == reChar.getValueAttribute().getHandle()) {
    memcpy(reStatus, params->data, params->len);
    atualizaReMotor();
  }
  
  if (params->handle == velocidadeChar.getValueAttribute().getHandle()) {
    memcpy(velocidade, params->data, params->len);
    atualizaVelocidadeMotor();
  }

  if (params->handle == direcaoChar.getValueAttribute().getHandle()) {
    memcpy(angulo, params->data, params->len);
    atualizaDirecao();
  }
}


void sensorsCallback() {
  if (ble.getGapState().connected) {
    
    // Nivel de Baterial

    levelCounter--;
    if (levelCounter == 10)
        levelCounter = 100;

    batteryLevel[0] = levelCounter;
    
    ble.updateCharacteristicValue(batteryLevelChar.getValueAttribute().getHandle(), batteryLevel, sizeof(batteryLevel));
  }
}




void setup() {
  
  velocidadeAtual = 0;
  anguloAtual = 90;

  direcao.attach(SERVO_PIN);
  
  Serial.begin(9600);

  pinMode(BATTERY_LEVEL_PIN, INPUT);
  pinMode(FAROL_PIN, OUTPUT);
  pinMode(INPUT1_MOTOR_PIN, OUTPUT);
  pinMode(INPUT2_MOTOR_PIN, OUTPUT);
  pinMode(ENABLE_MOTOR_PIN, OUTPUT);

  digitalWrite(INPUT1_MOTOR_PIN, HIGH);
  digitalWrite(INPUT2_MOTOR_PIN, LOW);
  direcao.write(anguloAtual);
  analogWrite(ENABLE_MOTOR_PIN, velocidadeAtual);
  digitalWrite(FAROL_PIN, HIGH);

  ble.init();
  ble.onConnection(connectionCallBack);
  ble.onDisconnection(disconnectionCallBack);
  ble.onDataWritten(gattServerWriteCallBack);


  // Setup advertising payload, including Eddystone data
  ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
  ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t*)advertisedServices, sizeof(advertisedServices));
  ble.accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, (uint8_t*)eddystoneData, sizeof(eddystoneData));
  ble.accumulateScanResponse(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME) - 1);
  ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);

  ble.addService(batteryService);
  ble.addService(sinalizacaoService);
  ble.addService(motorService);
  ble.addService(direcaoService);

  ble.setDeviceName((const uint8_t *)DEVICE_NAME);
  ble.setTxPower(4);
  ble.setAdvertisingInterval(160); // (100 ms = 160 * 0.625ms.)
  ble.setAdvertisingTimeout(0);
  ble.startAdvertising();


  // Battery level task
  batteryTask.attach(sensorsCallback,1);

}

void loop() {
  ble.waitForEvent();
}
