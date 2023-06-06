[TOC]
## 设备绑定

- 设备绑定

##### 请求Topic
- `ljrh/nebulizer/{deviceSn}/bind`
- `QOS:0`
``` 
    deviceSn:当前设备sn号
``` 

##### 参数

|参数名|必选|类型|说明|
|:----    |:---|:----- |-----   |
|sn |是  |String |设备sn   |
|timestamp |是  |long |时间戳   ||

##### 请求示例
```json
{"sn":"MPN22E4040395690P","timestamp":1670393433838}
```



## 液重差异上报

- 液重差异上报

##### 请求Topic
- `ljrh/nebulizer/{deviceSn}/weightIncrease`
- `QOS:0`
``` 
    deviceSn:当前设备sn号
``` 

##### 参数

|参数名|必选|类型|说明|
|:----    |:---|:----- |-----   |
|sn |是  |String |设备sn   |
|timestamp |是  |long |时间戳   ||

##### 请求示例
```json
{"sn":"MPN22E4040395690P","timestamp":"1670393433838"}
```


## 同步时间

- 同步时间

##### 请求Topic
- ` ljrh/nebulizer/{deviceSn}/syncDate`
- `QOS:1`
``` 
    deviceSn:当前设备sn号
``` 

##### 参数

|参数名|必选|类型|说明|
|:----    |:---|:----- |-----   |
|week |是  |string |周   |
|date |是  |string |日期   ||

##### 请求示例
```json
{"week":"xxxx","date":"yyyy-MM-dd hh:mm:ss"}
```

##### 接口定义

- 删除设备定时任务,暂时不考虑弱信号的情况
- QOS:1 设备需要响应
-


## 清空设备定时任务

- 清空设备定时任务

##### 请求Topic
- ` ljrh/nebulizer/{deviceSn}/clearTask`
- `QOS:1`
``` 
    deviceSn:当前设备sn号
``` 

##### 参数

|参数名|必选|类型|说明|
|:----    |:---|:----- |-----   |
|sn |是  |string |设备SN   |
|timestamp |是  |string |时间戳   ||

##### 请求示例
```json
{"sn":"MPN22E4040395690P","timestamp":1670393433838}
```

##### 接口定义

- 删除设备定时任务,暂时不考虑弱信号的情况
- QOS:1 设备需要响应
- 








## 删除设备定时任务 

- 删除设备定时任务

##### 请求Topic
- ` ljrh/nebulizer/{deviceSn}/task`
- `QOS:1`
``` 
    deviceSn:当前设备sn号
    
``` 

##### 参数

|参数名|必选|类型|说明|
|:----    |:---|:----- |-----   |
|taskNo |是  |string |任务编号   ||

##### 请求示例
```json
{"imei":"860387066315086","sn":"MPN22E4040395690P","task":{"no":"T221207JKYDV1","week":"1,2,3","start":"14:10","end":"15:00","echo":30,"time":45,"level":2,"flag":3,"single":0,"status":1,"timestamp":1670393433838}}
 
```

##### 接口定义

- 删除设备定时任务,暂时不考虑弱信号的情况
- QOS:0 定时报告不必确定服务器必须收到










## 设备定时报告 

- 设备定时报告

##### 请求Topic
- ` ljrh/nebulizer/{deviceSn}/deviceStatus`
- `QOS:0`
``` 
    deviceSn:当前设备sn号
    
``` 

##### 参数

|参数名|必选|类型|说明|
|:----    |:---|:----- |-----   |
|sn |是  |string |设备SN号   |
|imei |是  |string | 设备IMEI号    |
|bind |是  |int | 设备是否绑定 1:是  0:否    |
|status |是  |int | 是否启用 1:是  0:否 |
|weight |是  |int |  液体重量 |
|timestamp |是  |long| 时间戳||

##### 请求示例
```json
 {
	"sn":"MPN22E4040395690P",
	"imei":"860387066315086",
	"bind":1,
	"status":1,
	"timestamp":1670384066
}
 
```

##### 接口定义

- 设备定时向服务器报送当前状态
- QOS:0 定时报告不必确定服务器必须收到




## 设备上传当前液体重量(新增的接口)

- 设备上传当前液体重量

##### 请求Topic
- ` ljrh/nebulizer/{deviceSn}/liquidWeight`
- `QOS:0`
``` 
    sn:当前设备sn号
    weight:当前液体重量
    timestamp:时间戳
```

##### 参数

|参数名|必选|类型|说明|
|:----    |:---|:----- |-----   |
|sn |是  |string |设备SN号   |
|weight |是  |int |  液体重量 |
|timestamp |是  |long| 时间戳||
##### 请求示例
```json
 {
	"sn":"MPN22E4040395690P",
	"timestamp":1670384066,
    "weight":480
}
 
```

##### 接口定义

- 设备上传当前液体重量
- QOS:0 定时报告不必确定服务器必须收到




## 执行喷雾动作

- 执行喷雾动作

##### 请求Topic
- ` ljrh/nebulizer/{deviceSn}/execute`
- `QOS:0`
``` 
    sn:当前设备sn号
    timestamp:时间戳
```

##### 参数

|参数名|必选|类型|说明|
|:----    |:---|:----- |-----   |
|sn |是  |string |设备SN号   |
|timestamp |是  |long| 时间戳||
##### 请求示例
```json
 {
	"sn":"MPN22E4040395690P",
	"timestamp":1670384066
}
 
```

##### 接口定义

- 设备上传当前液体重量
- QOS:0 定时报告不必确定服务器必须收到







## 下次动作执行时间

- 下次动作执行时间

##### 请求Topic
- ` ljrh/nebulizer/{deviceSn}/nextAction`
- `QOS:0`

##### 参数

|参数名|必选|类型|说明|
|:----    |:---|:----- |-----   |
|sn |是  |string |设备SN号   |
|week |是  |string |周   |
|start_time |是  |string |开始时间(mm:dd)  |
|timestamp |是  |long| 时间戳||
##### 请求示例
```json
 {
	"sn":"MPN22E4040395690P",
	"timestamp":1670384066,
     "start_time": "15:30",
     "week": 3
}
 
```

##### 下次动作执行时间

- 下次动作执行时间
- QOS:0 定时报告不必确定服务器必须收到
