let clienteWeb = null;

const clientId = 'Esp32' + Math.floor(Math.random() * 900) + 100;
clienteWeb = new Paho.MQTT.Client('broker.hivemq.com', 8884, clientId);

const temppage = document.getElementById('temperature');
const humipage = document.getElementById('humidity');

clienteWeb.onMessageArrived = function (message) {
    const payload = JSON.parse(message.payloadString);
    if (payload) {
        if (payload.temperatura !== undefined) {
            temppage.textContent = payload.temperatura + ' °C';
        }
        if (payload.umidade !== undefined) {
            humipage.textContent = payload.umidade + '%';
        }
    }
}

clienteWeb.connect({   
    useSSL: true, 
    onSuccess: function() {
        console.log('A conexão com Broker foi bem sucedida')
        clienteWeb.subscribe('senai928/temperatura');
    },
    onFailure: function() {
        console.log('A conexão com Broker falhou')
    }
});
