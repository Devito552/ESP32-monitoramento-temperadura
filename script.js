const temperatura = [
    { id: 1, temperatura: 23.9 },
    { id: 2, temperatura: 24.1 },
    { id: 3, temperatura: 24.5 },
    { id: 4, temperatura: 24.8 },
]

const umidade = [
    { id: 1, umidade: 60 },
    { id: 2, umidade: 62 },
    { id: 3, umidade: 65 },
    { id: 4, umidade: 67 },
]

function getRandomTemperatura() {
    const randomIndex = Math.floor(Math.random() * temperatura.length);
    return temperatura[randomIndex];
}

function getRandomUmidade() {
    const randomIndex = Math.floor(Math.random() * umidade.length);
    return umidade[randomIndex];
}
function updateData(){
    const temperaturaElement = document.getElementById('temperature');
    const randomTemperatura = getRandomTemperatura();
    temperaturaElement.innerHTML = `${randomTemperatura.temperatura} °C`;

    const umidadeElement = document.getElementById('humidity');
    const randomUmidade = getRandomUmidade();
    umidadeElement.innerHTML = `${randomUmidade.umidade} %`;
}