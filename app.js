document.querySelector('#conectar').addEventListener('click', function(event) {
  carBle.conectar();
})

document.querySelector('#ligarFarol').addEventListener("click", function(event) {
  carBle.mudarFarol(1);
})

document.querySelector('#deliegarFarol').addEventListener("click", function(event) {
  carBle.mudarFarol(0);
})
