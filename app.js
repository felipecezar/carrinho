document.querySelector('#conectar').addEventListener('click', function(event) {
  console.log("Clicou em conectar");
  carBle.conectar();
})

document.querySelector('#ligarFarol').addEventListener("click", function(event) {
  carBle.ligarFarol();
})