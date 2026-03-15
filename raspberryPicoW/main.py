# -------------------------------------------
# Raspberry Pi Pico W - Carrinho WiFi
# Autor: exemplo educacional
# -------------------------------------------

import network
import socket
from machine import Pin
import time

# ==============================
# CONFIGURAÇÃO DOS PINOS MOTOR
# ==============================

motorA1 = Pin(2, Pin.OUT)
motorA2 = Pin(3, Pin.OUT)
motorB1 = Pin(4, Pin.OUT)
motorB2 = Pin(5, Pin.OUT)

# ==============================
# FUNÇÕES DE MOVIMENTO
# ==============================

def frente():
    motorA1.high()
    motorA2.low()
    motorB1.high()
    motorB2.low()

def tras():
    motorA1.low()
    motorA2.high()
    motorB1.low()
    motorB2.high()

def esquerda():
    motorA1.low()
    motorA2.high()
    motorB1.high()
    motorB2.low()

def direita():
    motorA1.high()
    motorA2.low()
    motorB1.low()
    motorB2.high()

def parar():
    motorA1.low()
    motorA2.low()
    motorB1.low()
    motorB2.low()

# ==============================
# CRIA ACCESS POINT
# ==============================

ssid = "CarrinhoPico"
password = "12345678"

ap = network.WLAN(network.AP_IF)
ap.active(True)
ap.config(essid=ssid, password=password)

print("Criando rede WiFi...")

while not ap.active():
    pass

print("Rede criada!")
print("IP:", ap.ifconfig()[0])

# ==============================
# HTML DA INTERFACE
# ==============================

html = """<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>

body{
font-family: Arial;
text-align:center;
}

button{
width:120px;
height:60px;
font-size:20px;
margin:10px;
}

</style>
</head>

<body>

<h2>Carrinho WiFi</h2>

<p>
<a href="/frente"><button>Frente</button></a>
</p>

<p>
<a href="/esquerda"><button>Esquerda</button></a>
<a href="/parar"><button>Parar</button></a>
<a href="/direita"><button>Direita</button></a>
</p>

<p>
<a href="/tras"><button>Trás</button></a>
</p>

</body>
</html>
"""

# ==============================
# SERVIDOR WEB
# ==============================

addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]

server = socket.socket()
server.bind(addr)
server.listen(1)

print("Servidor ativo")

while True:

    cl, addr = server.accept()
    print("Cliente conectado:", addr)

    request = cl.recv(1024)
    request = str(request)

    if '/frente' in request:
        frente()

    if '/tras' in request:
        tras()

    if '/esquerda' in request:
        esquerda()

    if '/direita' in request:
        direita()

    if '/parar' in request:
        parar()

    cl.send("HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n")
    cl.send(html)
    cl.close()