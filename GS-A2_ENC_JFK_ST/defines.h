#ifndef DEFINES_H
#define	DEFINES_H

	// Bit Masken zur Programmierung der GPIO Register; Coding Style
#define MODER_MASK_PIN_I(i) (0x03U << (2*(i))) //löschen 00 Input oder 01 Output
#define OUTPUT_MASK_PIN_I(i) (0x01U << (2*(i))) //setzen
#define IDR_MASK_PIN_I(i) (0x01U << (i))
#define ODR_MASK_PIN_I(i) (0x01U << (i))
#define BSRRH_MASK_PIN_I(i) (0x01U << (i))
#define BSRRL_MASK_PIN_I(i) (0x01U << (i))
#define NO_OF_PINS_OF_PORT (16) // Anzahl der Pins pro Port

// Konstanten zur Konfiguration der Ports und Pins (i) festlegen
#define PORT GPIOE
#define SWITCH_1_PIN 0 // Schiebeweiten
#define SWITCH_2_PIN 1
#define LED_1_PIN 15
#define LED_2_PIN 14
#define PERIPH_BASE ((uint32_t)0x40000000)
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000)
#define GPIOG ((GPIO_TypeDef *) GPIOG_BASE)

#define OUTPORT_PIN_CHANNEL_A      0
#define OUTPORT_PIN_CHANNEL_B      1

#endif /* DEFINES */
