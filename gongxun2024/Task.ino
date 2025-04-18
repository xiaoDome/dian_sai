#include <Arduino.h>
#define Kb 1024
// #if CONFIG_FREERTOS_UNICORE
// #define ARDUINO_RUNNING_CORE 0
// #else
// #define ARDUINO_RUNNING_CORE 1
// #endif

/*
线程初始化
*/
void Task_Init() {

  //    xTaskCreatePinnedToCore(
  //     Task_ESPNOW
  //     ,  "Task_ESPNOW" //任务名
  //     ,  32*Kb  // 栈大小
  //     ,  NULL
  //     ,  1  // 任务优先级
  //     ,  NULL
  //     ,  0);


  //   xTaskCreatePinnedToCore(
  //     Task_Led_Debug
  //     ,  "Task_Led_Debug"   // 任务名
  //     ,  16*Kb  // This stack size can be checked & adjusted by reading the Stack Highwater
  //     ,  NULL
  //     ,  0  // 任务优先级, with 3 (configMAX_PRIORITIES - 1) 是最高的，0是最低的.
  //     ,  NULL
  //     ,  0);

  xTaskCreatePinnedToCore(
    Task_Led_Logo, "Task_Led_Logo"  //任务名
    ,
    16 * Kb  // 栈大小
    ,
    NULL, 0  // 任务优先级
    ,
    NULL, 0);

  xTaskCreatePinnedToCore(
    Task_Led_System, "Task_Led_System"  //任务名
    ,
    16 * Kb  // 栈大小
    ,
    NULL, 0  // 任务优先级
    ,
    NULL, 0);

  //
  // strcpy(Esp_Data.DebugMes, "Task_Init OK");
}


/*---------------------- Tasks ---------------------*/

/*
ESPNOW任务
*/
void Task_ESPNOW(void *pvParameters) {
  while (1) {
    // Espnow_Loop();
    delay(10);
  }
}

void Task_Led_Debug(void *pvParameters) {
  while (1) {
    Light_Debug_Loop();
    //    Light_Logo_Loop();
    delay(50);
  }
}
void Task_Led_Logo(void *pvParameters) {
  while (1) {
    Light_Logo_Loop();

    delay(50);
  }
}
void Task_Led_System(void *pvParameters) {
  while (1) {
    Light_System_Loop();

    delay(50);
  }
}


// void TaskBlink1(void *pvParameters){  // This is a task.

// }
// void TaskBlink1(void *pvParameters){  // This is a task.

// }