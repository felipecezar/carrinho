document.querySelector('#conectar').addEventListener('click', function(event) {
  carBle.conectar();
})

document.querySelector('#ligarFarol').addEventListener("click", function(event) {
  carBle.mudarFarol(1);
})

document.querySelector('#desligarFarol').addEventListener("click", function(event) {
  carBle.mudarFarol(0);
})
