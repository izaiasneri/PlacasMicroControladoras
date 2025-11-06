
Cada diretório possui seus respectivos códigos-fonte, esquemas de ligação e instruções específicas de uso.

---

## 🧰 Configuração do ambiente de desenvolvimento

### 🔹 Arduino IDE
1. Baixe e instale a [Arduino IDE](https://www.arduino.cc/en/software).
2. No menu **Ferramentas → Placa → Gerenciador de Placas**, adicione:
   - **Arduino AVR Boards** (para Nano, Uno e Mega)
   - **esp32** (adicionando a URL: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`)
   - **esp8266** (adicionando a URL: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`)
3. Selecione a placa correta antes de compilar e enviar o código.

---

### 🔹 Thonny IDE (para Raspberry Pi Pico W)
1. Instale a [Thonny IDE](https://thonny.org/).
2. Conecte a placa **Raspberry Pi Pico W** via cabo USB.
3. Vá em **Executar → Selecionar Interpretador** e escolha:
   - **MicroPython (Raspberry Pi Pico)**.
4. Se necessário, instale o firmware MicroPython clicando em **Instalar ou atualizar firmware no Pico**.

---

### 🔹 VS Code (opcional, avançado)
1. Instale o [Visual Studio Code](https://code.visualstudio.com/).
2. Adicione as extensões:
   - **Arduino** (oficial da Microsoft)
   - **PlatformIO IDE** (para projetos mais avançados)
   - **PyMakr** (para ESP32/ESP8266 com MicroPython)
3. Configure o ambiente conforme a placa utilizada.

---

## 🧩 Dicas práticas

- Sempre verifique a **porta COM** usada pela placa.
- Utilize **fontes de alimentação adequadas** para motores e periféricos.
- Em projetos com Wi-Fi, evite senhas de rede com caracteres especiais (para facilitar o teste).
- Teste códigos simples (como *Blink*) antes de carregar projetos mais complexos.

---

## 📜 Licença

Este repositório é disponibilizado para fins **educacionais e experimentais**.  
O uso e modificação do conteúdo são livres mediante referência ao autor original.

---

## ✍️ Autor

**Prof. Me. Izaias Neri**  
Projetos com foco em aprendizado, experimentação e aplicação prática de microcontroladores.
