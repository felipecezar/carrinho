(function (){
  'use strict';

  let SERVIDOR_GATT_UUID        = 0x5100;
  let SINALIZACAO_SERVICE_UUID  = 0x5200;
  let FAROL_CHAR_UUID           = 0x5201;
  let DIRECAO_SERVICE_UUID      = 0x7200;
  let DIRECAO_CHAR_UUID         = 0x7201;
  let MOTOR_SERVICE_UUID        = 0x6200;
  let VELOCIDADE_CHAR_UUID      = 0x6201;
  let RE_CHAR_UUID              = 0x6202;

  class CarBle {

    constructor(){

      this.dispositivo              = null;
      this.servidorGATT             = null;
      this.sinalizacaoServico       = null;
      this.direcaocaoServico        = null;
      this.motorServico             = null;
      this.farolCaracteristica      = null;
      this.anguloCaracteristica     = null;
      this.velocidadeCaracteristica = null;
      this.reCaracteristica         = null;

    }

    conectar(){

      console.log('Conectando ao dispositivo...');

      let options = { filters:[
          {services:[SERVIDOR_GATT_UUID]},
          {services:[SINALIZACAO_SERVICE_UUID]},
          {services:[DIRECAO_SERVICE_UUID]},
          {services:[MOTOR_SERVICE_UUID]}
      ]};

      return navigator.bluetooth.requestDevice(options)
      .then(device => {
          this.dispositivo = device;
          console.log('Dispositivo: ' + this.dispositivo.name);
          return device.gatt.connect();
      })
      .then(server => {
          this.servidorGATT = server;
          console.log('Servidor GATT > OK');
          return this.servidorGATT.getPrimaryService(SINALIZACAO_SERVICE_UUID);
      })
      .then(service => {
          console.log('Servico - Sinalizacao > OK');
          this.sinalizacaoServico = service;
          return this.sinalizacaoServico.getCharacteristic(FAROL_CHAR_UUID);
      })
      .then(characteristic => {
          console.log('Caracteristica - farol > OK');
          this.farolCaracteristica = characteristic;
      })
      .then(() => {
          return this.servidorGATT.getPrimaryService(DIRECAO_SERVICE_UUID)
      })
      .then(service => {
          this.direcaocaoServico = service;
          console.log('Servico - Direcao > OK');
          return this.direcaocaoServico.getCharacteristic(DIRECAO_CHAR_UUID);
      })
      .then(characteristic => {
        this.anguloCaracteristica = characteristic;
        console.log('Caracteristica - Angulo > OK');
      })

      .then(() => {
          return this.servidorGATT.getPrimaryService(MOTOR_SERVICE_UUID)
      })
      .then(service => {
          this.motorServico = service;
          console.log('Servico - Motor > OK');
          return this.motorServico.getCharacteristic(VELOCIDADE_CHAR_UUID);
      })
      .then(characteristic => {
        this.velocidadeCaracteristica = characteristic;
        console.log('Caracteristica - Velocidade > OK');
        return this.motorServico.getCharacteristic(RE_CHAR_UUID);
      })
        .then(characteristic => {
        this.reCaracteristica = characteristic;
        console.log('Caracteristica - Re > OK');
      });
    }

    mudarFarol(valor){
      console.log("Ligar Farol");
      let data = new Uint8Array([valor]);
      return this.farolCaracteristica.writeValue(data);
    }


  }

  window.carBle = new CarBle();

}) ();