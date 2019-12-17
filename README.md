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
     ```
     #define osThread(name)  &os_thread_def_##name
     ```
     其中`osThreadId osThreadCreate (const osThreadDef_t *thread_def, void *argument)`函数调用FreeRTOS底层函数`xTaskCreate`创建Task

#### 2019\12\11
- void *   
  void指针，无类型指针（通用指针），可以指向任何类型
  ```
  static void vSenderTask( void *pvParameters ){
       long lValueToSend;
       portBASE_TYPE xStatus;
       /* 该任务会被创建两个实例，所以写入队列的值通过任务入口参数传递 – 这种方式使得每个实例使用不同的值。队列创建时指定其数据单元为long型，所以把入口参数强制转换为数据单元要求的类型 */
       lValueToSend = ( long ) pvParameters;
       }
  ```
#### 2019\12\17
- `osPriority`优先级有限  
  但是FreeRTOS最大优先级数量可以自己配置
     ```
     typedef enum  {
     osPriorityIdle          = -3,          ///< priority: idle (lowest)
     osPriorityLow           = -2,          ///< priority: low
     osPriorityBelowNormal   = -1,          ///< priority: below normal
     osPriorityNormal        =  0,          ///< priority: normal (default)
     osPriorityAboveNormal   = +1,          ///< priority: above normal
     osPriorityHigh          = +2,          ///< priority: high
     osPriorityRealtime      = +3,          ///< priority: realtime (highest)
     osPriorityError         =  0x84        ///< system cannot determine priority or thread has illegal priority
     } osPriority;
     ```
- `osSemaphoreDef(myBinarySem01);`
     ```
     #define osSemaphoreDef(name) const osSemaphoreDef_t os_semaphore_def_##name = { 0 }
     ```
     其中结构体
     ```
     typedef struct os_semaphore_def  {
     uint32_t                   dummy;    ///< dummy value.
     #if( configSUPPORT_STATIC_ALLOCATION == 1 )
     osStaticSemaphoreDef_t     *controlblock;      ///< control block for static allocation; NULL for dynamic allocation
     #endif
     } osSemaphoreDef_t;
     ```
- `myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);`  
  `count == 1`时调用 `vSemaphoreCreateBinary(sema);`生成二值信号量 否则
  调用`xSemaphoreCreateCounting(count, count);`生成计数信号量
- ```
  osThreadDef(Handle, HandleTask, osPriorityAboveNormal, 0, 128);
  HandleHandle = osThreadCreate(osThread(Handle), NULL);
  ```
  handler函数设置较高优先级时并且函数内写有
  `osSemaphoreWait(myBinarySem01Handle,osWaitForever);` 函数，该函数内调用`xSemaphoreTake`，任务被无超时阻塞，所以此函数调用也只会在成功获取信号量之后才会返回。
- ```
  osThreadDef(Periodic, PeriodicTask, osPriorityNormal, 0, 128);
  PeriodicHandle = osThreadCreate(osThread(Periodic), NULL);
  ```
  创建周期任务，任务内调用 `osSemaphoreRelease(myBinarySem01Handle);`,   
  `xSemaphoreGive`函数会给出信号量以使得等待此信号量的任务解除阻塞。如果解出阻塞的任务的优先级高于当前任务的优先级 – 强制进行一次任务切换，以确保中断直接返回到解出阻塞的任务(优选级更高)。
- **注** ：
  - 硬件ISR函数内只能发送信号量，然后在较高优先级的处理函数进行处理，或同步处理，若硬件中断种占用过多资源，会导致rtos系统崩溃。
  - 在原生的FreeRTOS中，信号量的发送和获取在ISR中要使用以FromISR结尾的特定函数。在STM32Cube生成的代码中，ST的工程师已经把接口进行了统一，在相应的函数中，通过查询程序状态寄存器判断当前是在ISR中还是在普通函数中。所以用户在使用时，统一使用osSemaphoreWait()和osSemaphoreRelease()函数操作即可。同样的，FreeRTOS的其他通信方式如队列、互斥量等，都进行了这样的处理。

  