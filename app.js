document.querySelector('#conectar').addEventListener('click', function(event) {
  console.log("Clicou em conectar");
  carBle.conectar();
})

document.querySelector('#ligarFarol').addEventListener("click", function(event) {
  carBle.ligarFarol();
})

document.querySelector('#virarEsquerda').addEventListener("click", function(event) {
  carBle.atualizaDirecao(-10);
})

document.querySelector('#virarDireita').addEventListener("click", function(event) {
  carBle.atualizaDirecao(10);
})