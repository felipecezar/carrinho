document.querySelector('#conectar').addEventListener('click', function(event) {
  console.log("Clicou em conectar");
  carBle.connect()
  .then(() => {
      return
  })
})

document.querySelector('#ligarFarol').addEventListener("click", function(event) {
  carBle.ligarFarol()
})