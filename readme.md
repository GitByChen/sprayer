2022年12月29日
功能说明：
    1：框架
        1、使用stm32cube 快速创建
        2、使用FREERTOS 系统
        3、IC：stm32L431RCT6
    2：功能：
        1、任务
            work_task：运行时间相关，判断是否工作的任务
            RC522Stask：读卡任务
            BC260Y_task：4G模块任务
            StartDefaultTask：调试使用的任务，用于串口3的接收判断，并进行看门口更新
        功能说明：
        1、增加了看门狗任务，1S更新
        2、增加4G模块的相关功能操作
        3、增加了读卡的逻辑判断# sprayer
