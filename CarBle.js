(function (){
  'use strict';

  let SERVIDOR_GATT_UUID =       0x5100;
  let SINALIZACAO_SERVICE_UUID = 0x5200;
  let FAROL_CHAR_UUID =          0x5201;
  let DIRECAO_SERVICE_UUID =     0x7200;
  let DIRECAO_CHAR_UUID =        0x7201;

  class CarBle {

    constructor(){
      this.device = null;
      this.server = null;
    }

    conectar(){
      let options = {filters:[{services:[SERVIDOR_GATT_UUID]}], optionalServices: [SINALIZACAO_SERVICE_UUID]};
      return navigator.bluetooth.requestDevice(options)
      .then(device => {
        this.device = device;
        return device.gatt.connect();
      })
      .then(server => {
        this.server = server;
      });
    }

    ligarFarol(){
      return this.server.getPrimaryService(SINALIZACAO_SERVICE_UUID)
      .then(service => service.getCharacteristic(FAROL_CHAR_UUID))
      .then(characteristic => {

        console.log("Ligar Farol");

        let data = new Uint8Array([1]);

        return characteristic.writeValue(data);
      })
    }
    
    atualizaDirecao(valor){
      return this.server.getPrimaryService(DIRECAO_SERVICE_UUID)
      .then(service => service.getCharacteristic(DIRECAO_CHAR_UUID))
      .then(characteristic => {

        console.log("Ligar Farol");

        let data = new Uint8Array([valor]);

        return characteristic.writeValue(data);
      })
    }

  }

  window.carBle = new CarBle();

}) ();