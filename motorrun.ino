

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


const unsigned long REST_TIME = 1200; // rest time between motor runs (in milliseconds)
const unsigned long MOTOR_RUN_TIME = 9000; // motor run time (in milliseconds)



// Our task: blink an LED at one rate
void motorController(void *parameter) {
  while (1) {
    int randomval = analogRead(4);
    if (randomval > 100) {
      Serial.println("Motor is on water not detected");
    }
    if (randomval < 10) {
      Serial.println("Motor is off  water is detected");
    }
     vTaskDelay(50/ portTICK_PERIOD_MS);
  }
}

// Our task: blink an LED at another rate
void Motorrun(void *parameter) {
  while (1) {

    int randomval = analogRead(4);
    if (randomval > 100) {
      digitalWrite(2, HIGH);
      Serial.println("Motor is running");
      vTaskDelay(MOTOR_RUN_TIME / portTICK_PERIOD_MS);
      digitalWrite(2, LOW);
      Serial.println("Motor is not running");
      vTaskDelay(REST_TIME / portTICK_PERIOD_MS);
    }
  }
}

void setup() {

  // Configure pin
  pinMode(4, OUTPUT);
  Serial.begin(9600);
  // Task to run forever
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
    motorController,  // Function to be called
    "Toggle 1",   // Name of task
    1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
    NULL,         // Parameter to pass to function
    1,            // Task priority (0 to configMAX_PRIORITIES - 1)
    NULL,         // Task handle
    app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // Task to run forever
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
    Motorrun,  // Function to be called
    "Toggle 2",   // Name of task
    1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
    NULL,         // Parameter to pass to function
    2,            // Task priority (0 to configMAX_PRIORITIES - 1)
    NULL,         // Task handle
    app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
}

void loop() {
  // Do nothing
  // setup() and loop() run in their own task with priority 1 in core 1
  // on ESP32
}
