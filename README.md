# FreeRTOS learning notes  
## Based on H743 & STM32Cube

#### 2019\12\8 

- 使用stm32cubemx生成FreeRTOS的代码，自动生成的代码都是cmsis_rtos对FreeRTOS的二次封装。封装后的函数接口在 ``cmsis_os.h`` 中
- main函数主体分为 硬件配置，创建任务，启动调度器
- `MX_FREERTOS_Init()`   函数进行初始化; 包含 `mutex,smaphores,timers,queues,threads.`
- 其中在threads中定义并创建线程
- eg.1  
     `osThreadDef(Task_LED0, Func_LED0, osPriorityNormal, 0, 128);`  
     `osThreadDef` 是宏定义:
     ``` 
     #define osThreadDef(name, thread, priority, instances, stacksz)  \
       	const  osThreadDef_t  os_thread_def_##name = \
        	{ #name, (thread), (priority), (instances), (stacksz)}
     ```
     其中结构体：
     ```
     typedef struct os_thread_def  {
     char                   *name;        ///< Thread name 
     os_pthread             pthread;      ///< start address of thread function
     osPriority             tpriority;    ///< initial thread priority
     uint32_t               instances;    ///< maximum number of instances of that thread function
     uint32_t               stacksize;    ///< stack size requirements in bytes; 0 is default stack size
     } osThreadDef_t;
     ```
     ( vscode ctrl + ] 代码左移 )
- eg.2  
     `Task_LED0Handle = osThreadCreate(osThread(Task_LED0), NULL);`
     `osThread` 是宏定义:  
     其中`osThreadId osThreadCreate (const osThreadDef_t *thread_def, void *argument)`函数调用FreeRTOS底层函数`xTaskCreate`创建Task

