#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ======================================================
// CONFIGURAÇÃO DO ACCESS POINT
// ======================================================

const char* ssid = "FuteboT01";
const char* password = "87654321";

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// Servidor HTTP
ESP8266WebServer server(80);

// ======================================================
// PINOS DOS MOTORES
// ======================================================

// Motor A
const int MOTOR_A1 = D1;
const int MOTOR_A2 = D2;

// Motor B
const int MOTOR_B1 = D3;
const int MOTOR_B2 = D4;

// ======================================================
// CONTROLE DE CONEXÃO
// ======================================================

IPAddress clientIP;
unsigned long lastHeartbeat = 0;

// Tempo máximo sem receber heartbeat
const unsigned long HEARTBEAT_TIMEOUT = 4000;

// ======================================================
// FUNÇÕES DOS MOTORES
// ======================================================

void parar() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);

  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}

void frente() {

  // Motor A para frente
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);

  // Motor B para frente
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}

void tras() {

  // Motor A para trás
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);

  // Motor B para trás
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

void esquerda() {

  // Motor A para trás
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);

  // Motor B para frente
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}

void direita() {

  // Motor A para frente
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);

  // Motor B para trás
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

// ======================================================
// VERIFICA SE O CLIENTE É O CELULAR AUTORIZADO
// ======================================================

bool clienteAutorizado() {

  if (WiFi.softAPgetStationNum() == 0) {
    return false;
  }

  if (server.client().remoteIP() == clientIP) {
    return true;
  }

  return false;
}

// ======================================================
// PÁGINA HTML
// ======================================================

const char paginaHTML[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="pt-BR">

<head>

<meta charset="UTF-8">

<meta name="viewport"
      content="width=device-width,
               initial-scale=1.0,
               maximum-scale=1.0,
               user-scalable=no">

<title>Projeto FuteboT</title>

<style>

* {
    box-sizing: border-box;
    -webkit-tap-highlight-color: transparent;
}

html, body {
    margin: 0;
    padding: 0;
    width: 100%;
    min-height: 100%;
    font-family: Arial, Helvetica, sans-serif;
    background: #10131a;
    color: white;
    user-select: none;
    touch-action: manipulation;
}

body {
    display: flex;
    justify-content: center;
    align-items: center;
    min-height: 100vh;
    padding: 20px;
}

.container {
    width: 100%;
    max-width: 500px;
    text-align: center;
}

.header {
    margin-bottom: 18px;
}

.title {
    font-size: clamp(22px, 6vw, 32px);
    font-weight: bold;
    margin-bottom: 8px;
}

.subtitle {
    font-size: 14px;
    color: #9ca3af;
}

/* ==================================================
   INDICADOR DE CONEXÃO
   ================================================== */

.status {
    display: inline-flex;
    align-items: center;
    gap: 8px;
    padding: 8px 15px;
    border-radius: 30px;
    margin: 15px 0 25px 0;
    font-size: 14px;
    font-weight: bold;
    background: #252a34;
}

.status-dot {
    width: 11px;
    height: 11px;
    border-radius: 50%;
    background: #ef4444;
}

.status.connected .status-dot {
    background: #22c55e;
    box-shadow: 0 0 10px #22c55e;
}

.status.connected {
    color: #86efac;
}

/* ==================================================
   CONTROLE
   ================================================== */

.controls {
    width: 100%;
    display: grid;

    grid-template-columns:
        repeat(3, minmax(80px, 1fr));

    grid-template-rows:
        repeat(3, minmax(80px, 100px));

    gap: 12px;

    max-width: 420px;
    margin: auto;
}

.btn {
    border: none;
    border-radius: 22px;

    background: #242a35;
    color: white;

    font-size: 35px;

    cursor: pointer;

    box-shadow:
        0 7px 0 #151922,
        0 10px 20px rgba(0,0,0,0.25);

    transition:
        transform 0.08s,
        box-shadow 0.08s,
        background 0.08s;

    display: flex;
    justify-content: center;
    align-items: center;

    touch-action: none;
}

.btn:active,
.btn.pressed {

    transform: translateY(6px);

    box-shadow:
        0 1px 0 #151922,
        0 4px 10px rgba(0,0,0,0.25);

    background: #2563eb;
}

.btn.stop {
    background: #3a1f24;
}

.btn.stop:active,
.btn.stop.pressed {
    background: #dc2626;
}

.forward {
    grid-column: 2;
    grid-row: 1;
}

.left {
    grid-column: 1;
    grid-row: 2;
}

.stop {
    grid-column: 2;
    grid-row: 2;
}

.right {
    grid-column: 3;
    grid-row: 2;
}

.backward {
    grid-column: 2;
    grid-row: 3;
}

/* ==================================================
   RODAPÉ
   ================================================== */

.footer {
    margin-top: 30px;
    color: #6b7280;
    font-size: 12px;
}

.ip {
    color: #60a5fa;
    font-weight: bold;
}

/* ==================================================
   RESPONSIVIDADE
   ================================================== */

@media (max-width: 380px) {

    .controls {
        grid-template-rows:
            repeat(3, 75px);
    }

    .btn {
        font-size: 28px;
        border-radius: 17px;
    }

}

</style>

</head>

<body>

<div class="container">

    <div class="header">

        <div class="title">
            projeto FuteboT
        </div>

        <div class="subtitle">
            prof. Izaias Neri
        </div>

        <div id="status" class="status">

            <span class="status-dot"></span>

            <span id="statusText">
                Conectando...
            </span>

        </div>

    </div>


    <div class="controls">

        <button
            class="btn forward"
            data-command="frente">
            ▲
        </button>

        <button
            class="btn left"
            data-command="esquerda">
            ◀
        </button>

        <button
            class="btn stop"
            data-command="parar">
            ■
        </button>

        <button
            class="btn right"
            data-command="direita">
            ▶
        </button>

        <button
            class="btn backward"
            data-command="tras">
            ▼
        </button>

    </div>


    <div class="footer">

        Access Point:
        <span class="ip">
            192.168.4.1
        </span>

    </div>

</div>


<script>

// ======================================================
// ENVIA COMANDO
// ======================================================

function enviarComando(comando) {

    fetch("/cmd?acao=" + comando, {
        method: "GET",
        cache: "no-store"
    }).catch(() => {});

}


// ======================================================
// BOTÕES
// ======================================================

const botoes = document.querySelectorAll(".btn");


botoes.forEach(botao => {

    const comando = botao.dataset.command;


    // --------------------------------------------------
    // TOUCH
    // --------------------------------------------------

    botao.addEventListener("touchstart", function(e) {

        e.preventDefault();

        botao.classList.add("pressed");

        enviarComando(comando);

    }, { passive: false });


    botao.addEventListener("touchend", function(e) {

        e.preventDefault();

        botao.classList.remove("pressed");

        enviarComando("parar");

    }, { passive: false });


    botao.addEventListener("touchcancel", function(e) {

        e.preventDefault();

        botao.classList.remove("pressed");

        enviarComando("parar");

    }, { passive: false });


    // --------------------------------------------------
    // MOUSE
    // --------------------------------------------------

    botao.addEventListener("mousedown", function(e) {

        e.preventDefault();

        botao.classList.add("pressed");

        enviarComando(comando);

    });


    botao.addEventListener("mouseup", function(e) {

        e.preventDefault();

        botao.classList.remove("pressed");

        enviarComando("parar");

    });


    botao.addEventListener("mouseleave", function(e) {

        botao.classList.remove("pressed");

        enviarComando("parar");

    });

});


// ======================================================
// HEARTBEAT
// ======================================================

function heartbeat() {

    fetch("/heartbeat", {
        method: "GET",
        cache: "no-store"
    })
    .then(response => response.text())
    .then(data => {

        if (data === "OK") {

            document
                .getElementById("status")
                .classList.add("connected");

            document
                .getElementById("statusText")
                .innerText =
                "Celular conectado";

        }

    })
    .catch(() => {

        document
            .getElementById("status")
            .classList.remove("connected");

        document
            .getElementById("statusText")
            .innerText =
            "Conexão perdida";

    });

}


// Atualiza a cada 1 segundo
setInterval(heartbeat, 1000);

heartbeat();


// ======================================================
// AO SAIR DA PÁGINA, PARA O CARRINHO
// ======================================================

window.addEventListener("pagehide", function() {

    enviarComando("parar");

});

</script>

</body>

</html>

)rawliteral";


// ======================================================
// PÁGINA PRINCIPAL
// ======================================================

void handleRoot() {

  // Só permite o primeiro dispositivo conectado
  if (WiFi.softAPgetStationNum() > 0) {

    if (clientIP == IPAddress(0, 0, 0, 0)) {
      clientIP = server.client().remoteIP();
    }

    if (server.client().remoteIP() != clientIP) {

      server.send(
        403,
        "text/plain",
        "Carrinho ocupado por outro dispositivo."
      );

      return;
    }
  }

  server.send_P(
    200,
    "text/html",
    paginaHTML
  );
}


// ======================================================
// COMANDOS
// ======================================================

void handleCommand() {

  IPAddress ip = server.client().remoteIP();

  // Se ainda não existe cliente registrado,
  // registra o primeiro.
  if (clientIP == IPAddress(0, 0, 0, 0)) {
    clientIP = ip;
  }

  // Impede outro cliente
  if (ip != clientIP) {

    parar();

    server.send(
      403,
      "text/plain",
      "Dispositivo nao autorizado."
    );

    return;
  }


  if (!server.hasArg("acao")) {

    parar();

    server.send(
      400,
      "text/plain",
      "Comando invalido."
    );

    return;
  }


  String comando = server.arg("acao");


  if (comando == "frente") {

    frente();

  }

  else if (comando == "tras") {

    tras();

  }

  else if (comando == "esquerda") {

    esquerda();

  }

  else if (comando == "direita") {

    direita();

  }

  else {

    parar();

  }


  lastHeartbeat = millis();

  server.send(
    200,
    "text/plain",
    "OK"
  );
}


// ======================================================
// HEARTBEAT
// ======================================================

void handleHeartbeat() {

  IPAddress ip = server.client().remoteIP();


  if (clientIP == IPAddress(0, 0, 0, 0)) {
    clientIP = ip;
  }


  if (ip != clientIP) {

    server.send(
      403,
      "text/plain",
      "Ocupado"
    );

    return;
  }


  lastHeartbeat = millis();

  server.send(
    200,
    "text/plain",
    "OK"
  );
}


// ======================================================
// NOT FOUND
// ======================================================

void handleNotFound() {

  server.send(
    404,
    "text/plain",
    "Pagina nao encontrada."
  );
}


// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(115200);

  delay(100);


  // ----------------------------------------------------
  // CONFIGURAÇÃO DOS PINOS
  // ----------------------------------------------------

  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);


  // Segurança:
  // motores começam desligados
  parar();


  // ----------------------------------------------------
  // CONFIGURAÇÃO DO WI-FI
  // ----------------------------------------------------

  WiFi.mode(WIFI_AP);

  WiFi.softAPConfig(
    local_IP,
    gateway,
    subnet
  );


  // Último parâmetro = 1
  // permite somente UM dispositivo conectado
  //WiFi.softAP(
 //   ssid,
  //  password,
   // 1,
  //  false,
  //  1
 // );

WiFi.softAP(
    ssid,
    password
    //1,
    //false,
   // 1
  );

  // ----------------------------------------------------
  // SERVIDOR
  // ----------------------------------------------------

  server.on(
    "/",
    HTTP_GET,
    handleRoot
  );


  server.on(
    "/cmd",
    HTTP_GET,
    handleCommand
  );


  server.on(
    "/heartbeat",
    HTTP_GET,
    handleHeartbeat
  );


  server.onNotFound(
    handleNotFound
  );


  server.begin();


  // ----------------------------------------------------
  // INFORMAÇÕES NO MONITOR SERIAL
  // ----------------------------------------------------

  Serial.println();
  Serial.println("==============================");
  Serial.println("       PROJETO FUTEBOT");
  Serial.println("==============================");

  Serial.print("SSID: ");
  Serial.println(ssid);

  Serial.print("Senha: ");
  Serial.println(password);

  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  Serial.println("Servidor HTTP iniciado.");
  Serial.println("Aguardando celular...");
  Serial.println("==============================");
}


// ======================================================
// LOOP
// ======================================================

void loop() {

  server.handleClient();


  // ----------------------------------------------------
  // DETECTA CONEXÃO/DESCONEXÃO
  // ----------------------------------------------------

  int dispositivos =
      WiFi.softAPgetStationNum();


  // Se não existe ninguém conectado,
  // libera o próximo celular.
  if (dispositivos == 0) {

    parar();

    clientIP = IPAddress(0, 0, 0, 0);

    lastHeartbeat = 0;

  }


  // ----------------------------------------------------
  // WATCHDOG DO CONTROLE
  // ----------------------------------------------------

  // Se o celular estiver conectado ao Wi-Fi,
  // mas deixar de enviar heartbeat,
  // o carrinho para automaticamente.

  if (dispositivos > 0) {

    if (lastHeartbeat > 0) {

      if (
        millis() - lastHeartbeat >
        HEARTBEAT_TIMEOUT
      ) {

        parar();

      }

    }

  }


  delay(2);
}