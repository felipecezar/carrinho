(function (){
  'use strict';

  let SERVIDOR_GATT_UUID =       0x5100;
  let SINALIZACAO_SERVICE_UUID = 0x5200;
  let FAROL_CHAR_UUID =          0x5201;
  let DIRECAO_SERVICE_UUID =     0x7200;
  let DIRECAO_CHAR_UUID =        0x7201;

  class CarBle {

    constructor(){

      this.dispositivo         = null;
      this.servidorGATT        = null;
      this.sinalizacaoServico  = null;
      this.direcaocaoServico   = null;
      this.farolCaracteristica = null;
      this.anguloCaracteristica = null;

      console.log('Objeto CarBle criado!');


    }

    conectar(){

      console.log('Conectando ao dispositivo...');

      let options = { filters:[
            {services:[SERVIDOR_GATT_UUID]},
            {services:[SINALIZACAO_SERVICE_UUID]},
            {services:[DIRECAO_SERVICE_UUID]}
      ]};

      return navigator.bluetooth.requestDevice(options)
      .then(device => {
          this.dispositivo = device;
          console.log('> Encontrado ' + this.dispositivo.name);
          console.log('Conectando no servidor GATT...');
          return device.gatt.connect();
      })
      .then(server => {
          this.servidorGATT = server;
          return this.servidorGATT.getPrimaryService(SINALIZACAO_SERVICE_UUID);
      })
      .then(service => {
          console.log('> Encontrado o servico de sinalizacao.');
          this.sinalizacaoServico = service;
          return this.sinalizacaoServico.getCharacteristic(FAROL_CHAR_UUID);
      })
      .then(characteristic => {
          console.log('> Encontrado a caracteristica do farol.');
          this.farolCaracteristica = characteristic;
      })
      .then(() => {
          return this.servidorGATT.getPrimaryService(DIRECAO_SERVICE_UUID)
      })
      .then(service => {
          this.direcaocaoServico = service;
          console.log('> Encontrado o servico de sinalizacao.');
          return this.direcaocaoServico.getCharacteristic(DIRECAO_CHAR_UUID);
      })
      .then(characteristic => {
        console.log('> Encontrada a caracteristica do angulo');
        this.anguloCaracteristica = characteristic;
      });
    }


    ligarFarol(){
      console.log("Ligar Farol");
      let data = new Uint8Array([1]);
      return this.farolCaracteristica.writeValue(data);
    }


  }

  window.carBle = new CarBle();

}) ();