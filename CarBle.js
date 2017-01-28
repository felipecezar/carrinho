(function (){
  'use strict';

  let SERVIDOR_GATT_UUID =       0x5100;
  let SINALIZACAO_SERVICE_UUID = 0x5200;
  let FAROL_CHAR_UUID =          0x5201;

  class CarBle {

    constructor(){
      this.device = null;
      this.server = null;
    }

    conectar(){
      let options = {filters:[{services:[SERVIDOR_GATT_UUID]}]};
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

        let buffer = new ArrayBuffer(1);
        let view = new Int8Array(buffer);
        view[0] = 1;
        return characteristic.writeValue(buffer);
      })
    }

  }

  window.carBle = new CarBle();

}) ();