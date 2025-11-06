# -------------------------------------------
# 🧩 Raspberry Pi Pico W - Access Point (Wi-Fi)
# Autor: Izaias Neri
# Data: Novembro de 2025
# -------------------------------------------

import network
import time

# 🔧 Configurações da rede Wi-Fi (Access Point)
SSID = "PicoW_AP"       # Nome da rede Wi-Fi que será criada
PASSWORD = "12345678"   # Senha (mínimo 8 caracteres)

# 🔹 Cria o objeto do tipo Access Point
ap = network.WLAN(network.AP_IF)

# 🔹 Ativa o modo Access Point
ap.active(True)

# 🔹 Define o nome e senha da rede
ap.config(essid=SSID, password=PASSWORD)

# 🔹 Aguarda ativação
print("Ativando Access Point...")
while ap.active() == False:
    time.sleep(0.5)

print("✅ Access Point ativo!")
print("📶 SSID:", SSID)
print("🔑 Senha:", PASSWORD)
print("🌐 IP:", ap.ifconfig()[0])
