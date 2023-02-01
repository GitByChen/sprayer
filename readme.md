2023年2月1日
修改人：陈嘉洛
新增：增加称重数据简单加密校验
    加密API:Weight_encryption(u16 weight)
    解密API:Weight_Decode(u8* weight)
优化：
    优化mqtt数据广播方式，将用于保持模块正常通讯的定时广播由15秒改为10秒发送一次，解决NB模块断开导致无法正常广播的问题
2022年12月29日
修改人：陈嘉洛
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
        3:功能相关API接口
            1、称重：Get_WeightBase():获取重量去皮值
                    Get_Weight():获取称重值
                    Write_Weight_To_Card():重量值写卡
            2： ParseStr():CJSON格式数据处理        
    功能说明：
    1、增加了看门狗任务，1S更新
    2、增加4G模块的相关功能操作
    3、增加了读卡的逻辑判断# sprayer

