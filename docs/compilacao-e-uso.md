# Compilação e Uso - ESP32-S3 HUD

## Pré-requisitos

### Hardware Necessário
- ESP32-S3 Touch AMOLED 1.43 (Waveshare)
- Cabo USB-C para programação
- iPhone com Sygic GPS Navigation instalado
- Fonte de alimentação 3.7V (opcional para uso portátil)

### Software Necessário
- PlatformIO IDE ou PlatformIO Core
- VS Code (recomendado) com extensão PlatformIO
- Git para clone do repositório

## Instalação do Ambiente

### 1. Instalar PlatformIO

#### Via VS Code (Recomendado)
```bash
# 1. Instalar VS Code
# 2. Abrir VS Code
# 3. Ir em Extensions (Ctrl+Shift+X)
# 4. Buscar "PlatformIO IDE"
# 5. Instalar a extensão
```

#### Via CLI
```bash
# Instalar Python pip primeiro
pip install platformio
```

### 2. Clonar o Projeto
```bash
git clone <repository-url>
cd esp32-s3-hud
```

### 3. Verificar Dependências
O arquivo `platformio.ini` já inclui todas as dependências necessárias:
- NimBLE-Arduino (comunicação BLE)
- LVGL (GUI library - para futuras implementações)
- Adafruit Sensor Libraries

## Compilação

### 1. Abrir Projeto no PlatformIO
```bash
# Via VS Code
code esp32-s3-hud/

# Via CLI
cd esp32-s3-hud
pio run
```

### 2. Configurar Target
O projeto está configurado para `esp32-s3-devkitc-1`. Verificar no `platformio.ini`:
```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
```

### 3. Compilar
```bash
# Via PlatformIO IDE
# Clicar no ícone "Build" na barra inferior

# Via CLI
pio run
```

### 4. Upload para ESP32-S3
```bash
# Conectar ESP32-S3 via USB-C
# Via PlatformIO IDE
# Clicar no ícone "Upload" na barra inferior

# Via CLI
pio run --target upload
```

### 5. Monitor Serial (Opcional)
```bash
# Via PlatformIO IDE
# Clicar no ícone "Serial Monitor"

# Via CLI
pio device monitor
```

## Configuração do Hardware

### Verificação de Conexões

#### Display AMOLED
As conexões são fixas na placa Waveshare:
- CS: GPIO 10
- DC: GPIO 11  
- RST: GPIO 12
- SCK: GPIO 14
- MOSI: GPIO 13

#### Touch Screen + Sensores I2C
- SDA: GPIO 6
- SCL: GPIO 7
- Touch INT: GPIO 5
- Touch RST: GPIO 4

#### Verificação Visual
1. Display deve ligar e mostrar "ESP32-S3 HUD Starting..."
2. Texto deve aparecer em tela circular preta
3. Monitor serial deve mostrar logs de inicialização

## Configuração do Sygic iOS

### Passo a Passo Detalhado

#### 1. Habilitar BLE HUD Mode
1. Abrir Sygic GPS Navigation & Maps no iPhone
2. Ir para **Menu** → **Settings** → **Info** → **About**
3. **Tocar 3 vezes em qualquer item** da lista About
4. Nova linha **"About"** aparece no topo da lista
5. Tocar na nova linha **About** → **BLE HUD** → **Start**

#### 2. Verificar Ativação
- Ícone BLE deve aparecer na interface do Sygic
- Aplicativo começará a escanear por dispositivos BLE
- ESP32-S3 deve aparecer como "ESP32-S3-HUD"

#### 3. Conexão Automática
- Sygic conecta automaticamente ao ESP32-S3
- Display do HUD muda para "Connected"
- Status BLE no Sygic mostra "Connected"

## Uso do Sistema

### Fluxo Normal de Uso

#### 1. Ligar o Sistema
```
ESP32-S3 HUD Starting...
↓
Display Shows: "HUD Ready"
↓  
"Waiting for Sygic Connection"
```

#### 2. Conectar Sygic
```
Sygic Scanning...
↓
Connection Established
↓
Display Shows: "Connected"
```

#### 3. Navegação Ativa
```
Set Route in Sygic
↓
Navigation Data Transmitted
↓
HUD Shows: Instructions + Distance + Speed Limit
```

### Interface do HUD

#### Elementos na Tela
```
     [Speed Limit Circle]
            |
    [Turn Direction Arrow]
       [Instruction Text]
            |
        [Distance]
```

#### Exemplo de Tela de Navegação
```
┌─────────────────────┐
│       [50]          │  ← Speed Limit (km/h)
│                     │
│         ↱           │  ← Turn Arrow
│    Turn Right       │  ← Instruction
│                     │
│       350m          │  ← Distance
└─────────────────────┘
```

### Configurações Disponíveis

#### Via Código (settings.h)
```cpp
// Exemplo de configurações personalizáveis
config.setBrightness(200);        // 0-255
config.setNightMode(true);        // true/false
config.setAutoRotation(true);     // true/false
config.setShowSpeedLimit(false);  // true/false
```

## Troubleshooting

### Problemas Comuns

#### 1. Display Não Liga
**Sintomas**: Tela permanece apagada
**Soluções**:
```bash
# Verificar conexão USB
# Verificar logs no monitor serial
pio device monitor

# Recompilar e re-upload
pio run --target upload
```

#### 2. Sygic Não Conecta
**Sintomas**: HUD fica em "Waiting for Connection"
**Soluções**:
1. Verificar se BLE HUD está ativado no Sygic
2. Reiniciar Sygic app
3. Reiniciar ESP32-S3
4. Verificar distância (máximo ~10m)

#### 3. Dados de Navegação Não Aparecem
**Sintomas**: Conectado mas sem instruções
**Soluções**:
1. Configurar rota no Sygic
2. Iniciar navegação no Sygic
3. Verificar se Sygic está em primeiro plano

#### 4. Display com Orientação Errada
**Sintomas**: Interface aparece rotacionada
**Soluções**:
```cpp
// Desabilitar auto-rotação
config.setAutoRotation(false);
config.setRotation(0); // 0, 1, 2, ou 3
```

### Logs de Debug

#### Monitor Serial Output Normal
```
ESP32-S3 HUD Navigation Starting...
Initializing settings...
Settings initialized successfully
Initializing AMOLED display...
AMOLED display initialized successfully
Initializing IMU sensor...
IMU calibration complete. Gyro Z offset: 0.123
IMU sensor initialized successfully
Initializing BLE server...
BLE server initialized successfully
BLE advertising started
ESP32-S3 HUD initialized successfully!
```

#### Logs de Conexão BLE
```
BLE Client connected
Navigation Update: Turn Right, 350m, 50km/h
Navigation Update: Turn Left, 150m, 30km/h
```

#### Logs de Erro
```
Failed to initialize display!
IMU WHO_AM_I failed: 0xFF (expected 0x05)
BLE Client disconnected
```

### Performance Esperada

#### Métricas Normais
- **Tempo de boot**: ~3-5 segundos
- **Conexão BLE**: ~2-5 segundos primeira vez
- **Reconexão**: ~1-3 segundos
- **Update rate**: 20 FPS
- **BLE latency**: ~100-300ms

#### Indicadores de Problemas
- Boot > 10 segundos: problema de hardware
- Reconexão > 10 segundos: interferência BLE
- Update rate < 10 FPS: sobrecarga de CPU

## Expansões e Modificações

### Adicionando Novos Recursos

#### 1. Modificar UI
```cpp
// Em ui_manager.cpp
void UIManager::drawCustomElement() {
    // Implementar novo elemento visual
}
```

#### 2. Adicionar Configurações
```cpp
// Em settings.h
bool customFeature;

// Em settings.cpp
void Settings::setCustomFeature(bool enabled) {
    currentSettings.customFeature = enabled;
    save();
}
```

#### 3. Modificar Protocolo BLE
```cpp
// Em ble_server.cpp
void BLEServer::parseCustomData(uint8_t* data, size_t length) {
    // Implementar parsing de novos dados
}
```

### Building para Produção

#### Configurações de Release
```ini
# Em platformio.ini
build_flags = 
    -DCORE_DEBUG_LEVEL=0  ; Sem debug logs
    -O2                   ; Otimização máxima
    -DNDEBUG             ; Release mode
```

#### Otimizações de Tamanho
```cpp
// Remover Serial.println em produção
#ifdef DEBUG_MODE
    Serial.println("Debug message");
#endif
```

## Suporte e Manutenção

### Documentação de Referência
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [Waveshare ESP32-S3 Touch AMOLED](https://www.waveshare.com/wiki/ESP32-S3-Touch-AMOLED-1.43)
- [Sygic BLE HUD Original Project](https://github.com/alexanderlavrushko/BLE-HUD-navigation-ESP32)

### Contato e Suporte
- Issues no GitHub para bugs
- Documentação em `docs/` para referência técnica
- Comments no código para implementação específica