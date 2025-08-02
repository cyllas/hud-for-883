# Arquitetura do Projeto ESP32-S3 HUD

## Visão Geral

O projeto ESP32-S3 HUD é um sistema de navegação Head-Up Display desenvolvido em C para a placa ESP32-S3 Touch AMOLED 1.43 da Waveshare. O sistema recebe dados de navegação do aplicativo Sygic iOS via BLE e exibe informações de navegação em uma interface otimizada para tela circular.

## Arquitetura de Software

### Estrutura Modular

O projeto segue uma arquitetura modular com separação clara de responsabilidades:

```
ESP32-S3 HUD Architecture
├── Main Controller (main.cpp)
├── BLE Communication Module
├── Display Driver Module  
├── UI Management Module
├── Sensor Handler Module
└── Configuration Module
```

### Módulos Principais

#### 1. Main Controller (`main.cpp`)
- **Responsabilidade**: Coordenação geral do sistema
- **Funcionalidades**:
  - Inicialização de todos os módulos
  - Loop principal de atualização
  - Coordenação entre módulos

#### 2. BLE Communication (`ble/`)
- **Responsabilidade**: Comunicação Bluetooth Low Energy
- **Componentes**:
  - `ble_server.h/cpp`: Servidor BLE para recepção de dados
- **Protocolo**: Compatível com Sygic iOS HUD mode
- **Service UUID**: `DD3F0AD1-6239-4E1F-81F1-91F6C9F01D86`

#### 3. Display Driver (`display/amoled_driver.*`)
- **Responsabilidade**: Controle do display AMOLED
- **Funcionalidades**:
  - Inicialização do display 466x466
  - Operações de desenho (pixel, linha, círculo)
  - Gestão de rotação
  - Otimização para tela circular

#### 4. UI Management (`display/ui_manager.*`)
- **Responsabilidade**: Gerenciamento da interface de usuário
- **Estados da UI**:
  - `UI_STARTUP`: Tela de inicialização
  - `UI_CONNECTING`: Aguardando conexão BLE
  - `UI_NAVIGATION`: Exibindo dados de navegação
  - `UI_NO_DATA`: Conectado mas sem dados
  - `UI_ERROR`: Estado de erro
- **Temas**: Dia/Noite com cores otimizadas

#### 5. Sensor Handler (`sensors/imu_handler.*`)
- **Responsabilidade**: Manipulação do sensor IMU QMI8658
- **Funcionalidades**:
  - Leitura de acelerômetro e giroscópio
  - Cálculo de rotação automática
  - Calibração automática
  - Detecção de orientação

#### 6. Configuration (`config/settings.*`)
- **Responsabilidade**: Gerenciamento de configurações
- **Persistência**: Usando Preferences (flash)
- **Configurações**:
  - Display (brilho, rotação, tema)
  - Navegação (unidades, elementos visíveis)
  - BLE (reconexão, auto-connect)
  - Energia (timeout, wake-on-data)

## Fluxo de Dados

```
iPhone (Sygic) → BLE → ESP32-S3 → Processing → UI Display
                  ↑                     ↓
                Sensor (IMU) → Rotation → Auto-rotate
```

### 1. Recepção de Dados
- Sygic transmite dados via BLE
- `BLEServer` recebe e decodifica
- Dados estruturados em `NavigationData`

### 2. Processamento
- `UIManager` recebe dados processados
- Determina estado apropriado da UI
- Aplica configurações do usuário

### 3. Renderização
- `AmoledDriver` executa comandos de desenho
- Interface otimizada para tela circular
- Rotação automática baseada no IMU

## Protocolo de Comunicação BLE

### Estrutura dos Dados Sygic

```
Formato: 0x01320A3335306D
[0] = Basic data flag
[1][2] = Speed limit (hex)
[3] = Turn direction
[4][5][6][7] = Distance (hex string)
```

### Mapeamento de Direções
- `0x01`: Virar à esquerda
- `0x02`: Virar à direita  
- `0x03`: Seguir em frente
- `0x04`: Retorno (U-turn)

## Interface de Usuário

### Layout Circular Otimizado

```
    [Speed Limit]
         |
    [Navigation]
    [Instruction]
         |
      [Distance]
```

### Elementos Visuais
- **Limite de Velocidade**: Círculo vermelho no topo
- **Instrução**: Texto central
- **Distância**: Parte inferior
- **Seta Direcional**: Indicador visual da manobra

### Temas
- **Dia**: Fundo branco, texto preto, acentos azuis
- **Noite**: Fundo preto, texto branco, acentos verdes

## Gestão de Energia

### Estados de Energia
1. **Ativo**: Display ligado, BLE ativo
2. **Standby**: Display dimmed, BLE ativo  
3. **Sleep**: Display off, BLE ativo para wake-up

### Wake-up Triggers
- Dados de navegação recebidos
- Movimento detectado pelo IMU
- Timeout configurável

## Configurações Persistentes

### Categorias de Configuração

#### Display
- Brilho (0-255)
- Rotação manual (0-3)
- Auto-rotação (bool)
- Tema noturno (bool)

#### Navegação  
- Mostrar limite velocidade (bool)
- Mostrar distância (bool)
- Mostrar instruções (bool)
- Unidade de distância (metros/pés)

#### Conectividade
- Delay de reconexão (ms)
- Auto-conectar (bool)

#### Energia
- Timeout para sleep (segundos)
- Wake on data (bool)

## Tratamento de Erros

### Estratégias de Recuperação
1. **BLE Disconnection**: Auto-reconexão com delay configurável
2. **Display Error**: Reset e reinicialização
3. **Sensor Error**: Continuar sem auto-rotação
4. **Data Corruption**: Ignorar pacote e aguardar próximo

### Estados de Erro
- Tela específica para erros
- Log via Serial para debug
- Recuperação automática quando possível

## Performance

### Otimizações Implementadas
- **Frame Rate**: 20 FPS (50ms update cycle)
- **SPI Speed**: 40MHz para display
- **I2C Speed**: 400kHz para sensores
- **BLE Power**: ESP_PWR_LVL_P9 para máximo alcance

### Uso de Memória
- **Flash**: ~500KB código + bibliotecas
- **SRAM**: ~100KB heap usage
- **PSRAM**: Disponível para expansões futuras

## Extensibilidade

### Pontos de Extensão Identificados
1. **Novos Protocolos**: Interface BLE modular
2. **Diferentes Displays**: Driver abstrato
3. **Sensores Adicionais**: Framework de sensores
4. **UI Customizada**: Sistema de temas extensível

### Arquitetura para LVGL (Futuro)
- Driver já preparado para integração
- Interface de callback implementada
- Gestão de memória compatível