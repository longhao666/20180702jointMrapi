文件修订记录：

| **版本号** | **修订人** | **时间** | **备注** |
| --- | --- | --- | --- |
| V1.0 | 崔元洋 | 2013-10-09 | 根据陈中元的通信协议制定，将关节与手抓等的通信协议分开，使用时更加清晰。 |
| V1.1 | 娄威 | 2016-05-06 | ·删除RS232通讯部分<br>·扩展内存控制表，主要增加了两组PID参数<br>·删除增量编码器相关控制字 |
| V1.2 | 娄威 | 2018-01-17 | ·增加伺服控制模式<br>·删减一些无用的指令 |
|   |   |   |   |

 <a href="#head1.1">`1.1 通信协议概要`</a>

 <a href="#head1.2">`1.2 指令类型`</a>

 <a href="#head1.3">`1.3 CAN总线协议`</a>

 <a href="#head1.4">`1.4 内存控制表`</a>

 <a href="#head1.5">`1.5 示例`</a>

 <a href="#headx">`附录`</a>

<a id="head1.1"/>
## 1.1 通信协议概要

　　主控制器和关节模块之间采用问答方式通信，控制器发出指令包，模块返回应答包。

　　一个CAN总线网络中允许有多个模块，所以每个模块都分配有一个ID号。控制器发出的控制指令中包含ID信息，只有匹配上ID号的模块才能完整接收这条指令，并返回应答信息。

　　所有的查询、控制和参数修改指令均通过对一张储存于模块主控芯片内部的 **内存控制表** 的读写操作完成。内存控制表的基本储存单位为2字节的有符号整形数据。

　　所有长度大于1字节的数据均采用低位在先的传输和储存方式，在协议解析中可配合使用c语音中的memcpy函数实现字节数据到整形数据或整形数据到字节数据的转换。

【注：`void *memcpy(void *dest, const void *src, size_t n)`;从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中】

<a id="head1.2"/>
## 1.2 指令类型

可用指令类型如下：

| **指令** | **功能** | **值** | **数据段长度** |
| --- | --- | --- | --- |
| CMD\_RD（读） | 查询控制表里的数据 | 0x01 | 1 |
| CMD\_WR（写） | 往控制表里写入数据 | 0x02 | 不小于2 |
| CMD\_WR\_NR（写） | 往控制表里写入数据(无返回) | 0x03 | 不小于2 |
| CMDTYPE\_SCP | 示波器数据通讯 | 0x05 | 6 |

<a id="head1.3"/>
## 1.3 CAN总线协议

　　使用CAN2.0A的标准帧格式，只使用数据帧，远程帧无应答。

### 1.3.1  指令包

指令包格式如下：

<table><tr><td colspan="2"> <b>仲裁域</b></td><td colspan="3"> <b>控制域</b></td><td colspan="8"> <b>数据域</b></td><td colspan="3"> <b>CRC及其他域</b></td></tr><tr><td>STID/11</td><td>RTR</td><td>IDE</td><td>R0</td><td>DLC/4</td><td colspan="8">Data (8Byte)</td><td>CRC</td><td>ACK</td><td>EOF</td></tr><tr><td>ID</td><td>0</td><td>0</td><td>0</td><td>Length</td><td>CMD</td><td>Index</td><td>D1</td><td>D2</td><td>D3</td><td>D4</td><td>D5</td><td>D6</td><td></td><td></td><td></td></tr></table>

**仲裁域** ：控制器给模块发送的指令，仲裁域ID即为模块ID，只用到11位中的低8位，模块ID为1~254（0x01~0xFE）。

模块返回的指令中，仲裁域ID为0x100+模块ID。

**控制域** ：DLC长度，即数据域字节长度。

**数据域** : 数据域第一个字节为指令类型，第二个字节为控制表索引，第三到第八个字节为随指令发送的数据。读指令只有一个字节数据，表示所要读取的内存表字节长度；写指令数据段为要写入内存表的内容。

CMD\_RD指令包：

<table><tr><td colspan="2"> <b>仲裁域</b></td><td colspan="3"> <b>控制域</b></td><td colspan="8"> <b>数据域</b></td><td colspan="3"> <b>CRC及其他域</b></td></tr><tr><td>STID/11</td><td>RTR</td><td>IDE</td><td>R0</td><td>DLC/4</td><td colspan="8">Data (8Byte)</td><td>CRC</td><td>ACK</td><td>EOF</td></tr><tr><td>ID</td><td>0</td><td>0</td><td>0</td><td>3</td><td>CMD_RD</td><td>Index</td><td>D1</td><td>　</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr></table>
ID: 关节ID<br>
Length: 数据域字节数，为3<br>
CMD\_RD: 0x01<br>
Index: 需要读取的起始索引<br>
D1: 需要读取的数据字节数

CMD\_WR/ CMD\_WR\_NR指令包：

<table><tr><td colspan="2"> <b>仲裁域</b></td><td colspan="3"> <b>控制域</b></td><td colspan="8"> <b>数据域</b></td><td colspan="3"> <b>CRC及其他域</b></td></tr><tr><td>STID/11</td><td>RTR</td><td>IDE</td><td>R0</td><td>DLC/4</td><td colspan="8">Data (8Byte)</td><td>CRC</td><td>ACK</td><td>EOF</td></tr><tr><td>ID</td><td>0</td><td>0</td><td>0</td><td>Length</td><td>CMD_WR</td><td>Index</td><td>D1</td><td>D2</td><td>D3</td><td>D4</td><td>D5</td><td>D6</td><td></td><td></td><td></td></tr></table>

ID: 关节ID<br>
Length: 数据域字节数(需要写入的数据字节数+2，可以为4,6,8)<br>
CMD\_RD: 0x01<br>
Index: 需要写入的起始索引<br>
D1~D6: 需要写入的数据，对应关系如下：

| Index | D1 + D2 &lt;&lt; 8 |
| --- | --- |
| Index+1 | D3 + D4 &lt;&lt; 8 |
| Index+2 | D5 + D6 &lt;&lt; 8 |



### 1.3.2  应答包

应答包格式如下：
<table><tr><td colspan="2"> <b>仲裁域</b></td><td colspan="3"> <b>控制域</b></td><td colspan="8"> <b>数据域</b></td><td colspan="3"> <b>CRC及其他域</b></td></tr><tr><td>STID/11</td><td>RTR</td><td>IDE</td><td>R0</td><td>DLC/4</td><td colspan="8">Data (8Byte)</td><td>CRC</td><td>ACK</td><td>EOF</td></tr><tr><td>ID+0x100</td><td>0</td><td>0</td><td>0</td><td>Length</td><td>CMD</td><td>Index</td><td>D1</td><td>D2</td><td>D3</td><td>D4</td><td>D5</td><td>D6</td><td></td><td></td><td></td></tr></table>

　　应答包的格式与指令包基本一致，其区别在于：

若模块收到的是读指令，则返回的数据段为指定长度的数据。

　　特别注意的是，如果控制器发送的读指令所指定读取的长度大于6，则模块会将所要返回的数据自动拆分成数个应答包，每个应答包的Index字段值会自动递增，表征该帧数据对应的控制表起始索引。

　　例如，若收到控制器读取地址0x10开始的16个字节数据，则模块将连续返回三帧数据，第一帧Index段为0x10，后面跟随6个字节数据；第二帧Index段为0x13（内存表以16位数据为基本单位），后面跟随6个字节数据；第三帧Index段为0x16，后面跟随4个字节数据。

　　若模块收到的是需要返回应答的写指令，则返回的数据段长度为1Byte，1表示写入成功，0表示写入失败。

### 1.3.3  伺服指令包

　　模块进入周期性伺服控制阶段，控制器向各个关节模块发送指令帧，各个关节模块在收到指令帧之后进行相应的位置伺服控制，并将当前的位置信息和电流信息通过反馈帧发送给控制器。如果一个总线上有7个模块参与周期性伺服控制，那么一个控制周期之内，总线上将会产生14帧的通讯，以此降低总线负载。

指令帧：

<table><tr><td colspan="2"> <b>仲裁域</b></td><td colspan="3"> <b>控制域</b></td><td colspan="8"> <b>数据域</b></td><td colspan="3"> <b>CRC及其他域</b></td></tr><tr><td>STID/11</td><td>RTR</td><td>IDE</td><td>R0</td><td>DLC/4</td><td colspan="8">Data (8Byte)</td><td>CRC</td><td>ACK</td><td>EOF</td></tr><tr><td>ID+0x200</td><td>0</td><td>0</td><td>0</td><td>Length</td><td>D1</td><td>D2</td><td>D3</td><td>D4</td><td>D5</td><td>D6</td><td>D7</td><td>D8</td><td></td><td></td><td></td></tr><tr><td>ID+0x200</td><td>0</td><td>0</td><td>0</td><td>8</td><td colspan="4">target position</td><td colspan="4">target speed</td><td></td><td></td><td></td></tr></table>


反馈帧：

<table><tr><td colspan="2"> <b>仲裁域</b></td><td colspan="3"> <b>控制域</b></td><td colspan="8"> <b>数据域</b></td><td colspan="3"> <b>CRC及其他域</b></td></tr><tr><td>STID/11</td><td>RTR</td><td>IDE</td><td>R0</td><td>DLC/4</td><td colspan="8">Data (8Byte)</td><td>CRC</td><td>ACK</td><td>EOF</td></tr><tr><td>ID+0x300</td><td>0</td><td>0</td><td>0</td><td>Length</td><td>D1</td><td>D2</td><td>D3</td><td>D4</td><td>D5</td><td>D6</td><td>D7</td><td>D8</td><td></td><td></td><td></td></tr><tr><td>ID+0x200</td><td>0</td><td>0</td><td>0</td><td>8</td><td colspan="4">real position</td><td colspan="4">real current</td><td></td><td></td><td></td></tr></table>
<a id="head1.4"/>
## 1.4 内存控制表

　　模块本身的信息和控制参数形成了一张一维控制表，在控制芯片的RAM和Flash中各保存一份。在系统上电时，控制芯片从Flash读取控制表到RAM中，在模块运行过程中用户对RAM中的控制表进行读写操作，获取模块状态或对模块进行控制。

若用户对表中的&quot;保存数据到Flash标志&quot;写1，则控制芯片将当前RAM表的内容复制到Flash中，实现断电保存。

控制表内容如下：

| **地址** | **名称** | **命令项** | **权限** | **范围** | **初始值** |
| --- | --- | --- | --- | --- | --- |
| **0x00** | SYS\_HW\_VERSION | 驱动器硬件版本 | R | 303 | 303 |
| **0x01** | SYS\_ID | 驱动器ID | R/W | 0x01~0xFE | -- |
| **0x02** | SYS\_MODEL\_TYPE | 驱动器型号：0x10-M14，0x20-M17,0x21-M17E,0x30-M20 | R |   | -- |
| **0x03** | SYS\_FW\_VERSION | 驱动器固件版本 | R |   |   |
| **0x04** | SYS\_ERROR | 错误代码 | R |   | -- |
| **0x05** | SYS\_VOLTAGE | 系统电压（0.01V） | R |   | -- |
| **0x06** | SYS\_TEMP | 系统温度（0.1℃） | R |   | -- |
| **0x07** | SYS\_REDU\_RATIO | 模块减速比：M14-100，M17-10,M17E-120,M20-160 | R |   | -- |
| **0x08** | reserved |   |   |   | 1 |
| **0x09** | SYS\_BAUDRATE\_CAN | CAN总线波特率：0-250k，1-500k，2-1M | R/W |   | 2 |
| **0x0a** | SYS\_ENABLE\_DRIVER | 驱动器使能标志 | R/W |   | 0 |
| **0x0b** | SYS\_ENABLE\_ON\_POWER | 上电使能驱动器标志 | R/W |   | 1 |
| **0x0c** | SYS\_SAVE\_TO\_FLASH | 保存数据到Flash标志 | R/W |   | 0 |
| **0x0d** | SYS\_IAP | 置1，下次上电进入bootloader模式；置2，立即进入bootloader模式 | R/W |   | 0 |
| **0x0e** | SYS\_SET\_ZERO\_POS | 将当前位置设置为零点标志 | R/W |   | 0 |
| **0x0f** | SYS\_CLEAR\_ERROR | 清除错误标志 | R/W |   | 0 |
| **0x10** | SYS\_CURRENT\_L | 当前电流低16位（mA） | R |   | -- |
| **0x11** | SYS\_CURRENT\_H | 当前电流高16位（mA） | R |   | -- |
| **0x12** | SYS\_SPEED\_L | 当前速度低16位（units/s） | R |   | -- |
| **0x13** | SYS\_SPEED\_H | 当前速度高16位（units/s） | R |   | -- |
| **0x14** | SYS\_POSITION\_L | 当前位置低16位（units） | R |   | -- |
| **0x15** | SYS\_POSITION\_H | 当前位置高16位（units） | R |   | -- |
| **0x16** | reserved |   | R |   | -- |
| **0x17** | SYS\_ZERO\_POS\_OFFSET\_L | 零点位置偏移量低16位（units） | R/W |   | 0 |
| **0x18** | SYS\_ZERO\_POS\_OFFSET\_H | 零点位置偏移量高16位（units） | R/W |   | 0 |
| **0x19~** **0x1f** | 保留 | 保留 | -- |   | -- |
| **0x20** | MOT\_RES | 电机内阻（mΩ） | R |   | -- |
| **0x21** | MOT\_INDUC | 电机电感（mH） | R |   | -- |
| **0x22** | MOT\_RATED\_VOL | 电机额定电压（0.1V） | R |   | -- |
| **0x23** | MOT\_RATED\_CUR | 电机额定电流（mA） | R |   | -- |
| **0x24** | reserved |   | R |   | 1024 |
| **0x25** | reserved |   | R |   | -- |
| **0x26** | MOT\_ST\_DAT | 绝对编码器单圈数据 | R |   | -- |
| **0x27** | MOT\_MT\_DAT | 绝对编码器多圈数据 | R |   | -- |
| **0x28~0x2f** | 保留 | 保留 | -- |   | -- |
| **0x30** | TAG\_WORK\_MODE | 工作模式：0-开环，1-电流模式，2-速度模式PV，3-位置模式PP，4-位置模式IP | R/W |   | 3 |
| **0x31** | TAG\_OPEN\_PWM | 开环模式下占空比（0~100） | R/W |   | 0 |
| **0x32** | TAG\_CURRENT\_L | 目标电流低16位（mA） | R/W |   | 0 |
| **0x33** | TAG\_CURRENT\_H | 目标电流高16位（mA） | R/W |   | 0 |
| **0x34** | TAG\_SPEED\_L | 目标速度低16位（units/s） | R/W |   | 0 |
| **0x35** | TAG\_SPEED\_H | 目标速度高16位（units/s） | R/W |   | 0 |
| **0x36** | TAG\_POSITION\_L | 目标位置低16位（units） | R/W |   | 0 |
| **0x37** | TAG\_POSITION\_H | 目标位置高16位（units） | R/W |   | 0 |
| **0x38~0x3f** | 保留 | 保留 | -- |   | -- |
| **0x40** | LIT\_MAX\_CURRENT | 最大电流（mA） | R/W |   | -- |
| **0x41** | LIT\_MAX\_SPEED | 最大速度（rpm） | R/W | 0~2000 | -- |
| **0x42** | LIT\_MAX\_ACC | 最大加/减速度（rpm/s） | R/W | 0~5000 | -- |
| **0x43** | LIT\_MIN\_POSITION\_L | 最小位置低16位（units） | R/W |   | -- |
| **0x44** | LIT\_MIN\_POSITION\_H | 最小位置高16位（units） | R/W |   | -- |
| **0x45** | LIT\_MAX\_POSITION\_L | 最大位置低16位（units） | R/W |   | -- |
| **0x46** | LIT\_MAX\_POSITION\_H | 最大位置高16位（units） | R/W |   | -- |
| **0x47~0x4f** | 保留 | 保留 | -- |   | -- |
| **0x50** | SEV\_PARAME\_LOCKED | 三闭环参数锁定标志：0-自动切换，1-低载PID参数，2-中载PID参数，3-重载PID参数 | R/W |   | 0 |
| **0x51** | S\_CURRENT\_P | S电流环P参数 | R/W |   | -- |
| **0x52** | S\_CURRENT\_I | S电流环I参数 | R/W |   | -- |
| **0x53** | S\_CURRENT\_D | S电流环D参数 | R/W |   | -- |
| **0x54** | S\_SPEED\_P | S速度环P参数 | R/W |   | -- |
| **0x55** | S\_SPEED\_I | S速度环I参数 | R/W |   | -- |
| **0x56** | S\_SPEED\_D | S速度环D参数 | R/W |   | -- |
| **0x57** | S\_SPEED\_DS | S速度P死区 | R/W |   | -- |
| **0x58** | S\_POSITION\_P | S位置环P参数 | R/W |   | -- |
| **0x59** | S\_POSITION\_I | S位置环I参数 | R/W |   | -- |
| **0x5a** | S\_POSITION\_D | S位置环D参数 | R/W |   | -- |
| **0x5b** | S\_POSITION\_DS | S位置P死区 | R/W |   | -- |
| **0x5c~0x5f** | 保留 | 保留 | -- |   | -- |
| **0x60** | 保留 | 保留 | -- |   | -- |
| **0x61** | M\_CURRENT\_P | M电流环P参数 | R/W |   | -- |
| **0x62** | M\_CURRENT\_I | M电流环I参数 | R/W |   | -- |
| **0x63** | M\_CURRENT\_D | M电流环D参数 | R/W |   | -- |
| **0x64** | M\_SPEED\_P | M速度环P参数 | R/W |   | -- |
| **0x65** | M\_SPEED\_I | M速度环I参数 | R/W |   | -- |
| **0x66** | M\_SPEED\_D | M速度环D参数 | R/W |   | -- |
| **0x67** | M\_SPEED\_DS | M速度P死区 | R/W |   | -- |
| **0x68** | M\_POSITION\_P | M位置环P参数 | R/W |   | -- |
| **0x69** | M\_POSITION\_I | M位置环I参数 | R/W |   | -- |
| **0x6a** | M\_POSITION\_D | M位置环D参数 | R/W |   | -- |
| **0x6b** | M\_POSITION\_DS | M位置P死区 | R/W |   | -- |
| **0x6c~0x6f** | 保留 | 保留 | -- |   | -- |
| **0x70** | 保留 | 保留 | -- |   | -- |
| **0x71** | L\_CURRENT\_P | L电流环P参数 | R/W |   | -- |
| **0x72** | L\_CURRENT\_I | L电流环I参数 | R/W |   | -- |
| **0x73** | L\_CURRENT\_D | L电流环D参数 | R/W |   | -- |
| **0x74** | L\_SPEED\_P | L速度环P参数 | R/W |   | -- |
| **0x75** | L\_SPEED\_I | L速度环I参数 | R/W |   | -- |
| **0x76** | L\_SPEED\_D | L速度环D参数 | R/W |   | -- |
| **0x77** | L\_SPEED\_DS | L速度P死区 | R/W |   | -- |
| **0x78** | L\_POSITION\_P | L位置环P参数 | R/W |   | -- |
| **0x79** | L\_POSITION\_I | L位置环I参数 | R/W |   | -- |
| **0x7a** | L\_POSITION\_D | L位置环D参数 | R/W |   | -- |
| **0x7b** | L\_POSITION\_DS | L位置P死区 | R/W |   | -- |
| **0x7c~0x7f** | 保留 | 保留 | -- |   | -- |
| **0x80** | BRAKE\_RELEASE\_CMD | 刹车释放命令，0-保持制动，1-释放刹车 | R/W |   | 0 |
| **0x81** | BRAKE\_STATE | 刹车状态，0-保持制动，1-释放刹车 | R |   | 0 |
| **0x82~0x8f** | 保留 | 保留 | -- |   | -- |
| **0x90** | SCP\_MASK | 记录对象标志MASK：0x0001-记录目标电流MASK0x0002-记录实际电流MASK0x0004-记录目标速度MASK0x0008-记录实际速度MASK0x0010-记录目标位置MASK0x0020-记录实际位置MASK | R/W |   | 0 |
| **0x91** | SCP\_TRI\_SOC | 触发源：0-开环触发，1-电流触发，2-速度触发，3-位置触发 | R/W |   | 0 |
| **0x92** | SCP\_TRI\_MOD | 触发方式：0-上升沿，1-下降沿，2-连续采样，3-用户触发 | R/W |   | 0 |
| **0x93** | SCP\_TRI\_FLG | 用户触发标志:0-不触发，1-触发 | R/W |   | 0 |
| **0x94** | SCP\_REC\_TIM | 记录时间间隔（对10kHZ的分频值） | R/W |   | 1 |
| **0x95** | SCP\_REC\_OFS | 记录时间偏置（默认以信号过零点时刻±60次数据） | R/W |   |   |
| **0x96** | SCP\_TAGCUR | 目标电流数据集 | R/W |   | 0 |
| **0x97** | SCP\_MEACUR | 实际电流数据集 | R/W |   | 0 |
| **0x98** | SCP\_TAGSPD | 目标速度数据集 | R/W |   | 0 |
| **0x99** | SCP\_MEASPD | 实际速度数据集 | R/W |   | 0 |
| **0x9a** | SCP\_TAGPOS | 目标位置数据集 | R/W |   | 0 |
| **0x9b** | SCP\_MEAPOS | 实际位置数据集 | R/W |   | 0 |
| **0x9c~0x9f** | 保留 | 保留 | -- |   | -- |

　　需要注意的地方有：

　　ID设置成功后将立即起作用，模块将以新ID进行通讯。

　　固件版本的定义为：低5位代表日，第6位到第9位代表月，第10位到第16位代表年。其定义示例为：

`#define _SW_VERSION                (u16)((13<<9)|(3<<5)|(14))                //固件版本13年3月14日`

### 1.4.1  错误代码定义

　　错误代码是16位数据，每一位代表一种错误，当相应错误产生时，相应位将置1，出现不可恢复的错误后，模块将自动进入驱动禁能状态，用户需要往&quot;清除错误标志&quot;字段（0x0f）写1来清除错误，清除错误成功后系统自动进入驱动使能状态。

错误位代表的含义如下：

| **错误位** | **定义** | **说明** |
| --- | --- | --- |
| 0x0001 | 过流 | 电机电流超过安全范围 |
| 0x0002 | 过压 | 系统电压超过安全范围 |
| 0x0004 | 欠压 | 系统电压低于安全范围 |
| 0x0008 | 过温 | 温度过高 |
| 0x0010 | 霍尔错误 | 霍尔传感器出错 |
| 0x0020 | 码盘错误 | 编码器出错 |
| 0x0040 | 数字电位器错误 | 绝对位置传感器出错 |
| 0x0080 | 电流检测错误 | 上电时电流传感器检测错误 |
| 0x0100 | 保险丝断开错误 | 检测到保险丝已断开 |

### 1.4.2  标志说明

共有6个标志类变量，其值只能为1或者0，其他值无效。具体说明如下表所示。

| **地址** | **定义** | **说明** |
| --- | --- | --- |
| 0x0a | 驱动器使能标志 | 写1使能驱动器，写0禁能驱动器并打开锁相继电器 |
| 0x0b | 上电使能驱动器标志 | 1代表上电即使能驱动器并工作在指定模式，0代表上电不使能 |
| 0x0c | 保存数据到Flash标志 | 写入1后芯片将当前RAM中的控制表写入Flash中，系统自动清零 |
| 0x0e | 将当前位置设置为零点标志 | 写1后将当前的位置保存为零点位置，并立即起作用，数据保存到Flash中后下次上电后将使用保存的位置作为零点位置。系统自动清零 |
| 0x0f | 清除错误标志 | 写1尝试清除当前错误。系统自动清零 |
| 0x50 | 三闭环参数锁定标志 | 上电自动设置为0，PID参数将根据载荷大小（速度）自动切换；不为0时，PID锁定为某一组参数，不进行自动切换 |

### 1.4.3  相关单位说明

在内存控制表中，电流的单位为mA。

电压的单位为0.01V。

位置的单位为unit，即编码器读数，用户需要换算成最终输出轴的角度公式如下：

Angle = 360.0f\*units/65536/ SYS\_REDU\_RATIO

其中SYS\_REDU\_RATIO为模块减速比。

速度的单位为units/s，即编码器读数/s。

加速度的单位为units/s2。

### 1.4.4  工作模式说明

　　模块共有五种工作模式：开环模式、电流模式、速度模式、位置模式和伺服控制模式。

　　五种模式的切换通过改变控制表中的TAG\_WORK\_MODE字段（0x30）实现切换。

　　其中，开环模式直接控制驱动器输出到电机的占空比，容易发生危险，不建议使用。电流模式控制电机输出的目标电流（力矩）。速度模式控制电机输出转速，除非模块能够实现整周转动，否则不建议使用。位置模块控制电机伺服位置。

　　0x3\*字段中的目标占空比、电流、速度和位置只有在模块处于相应工作模式下才起作用。

　　0x40~0x46字段中的最大电流、最大速度和最大加/减速度对电流、速度和位置模式均有效，最小和最大位置只有工作在位置伺服模式有效，其指定了位置伺服的目标值不能够设定在该范围以外。若设定的目标位置小于最小值或大于最大值，则模块将只运动到最小值或最大值处。若驱动器使能时模块已经处于范围之外，则将自动运动到最小/最大值处。

　　伺服控制模式为机械臂进入周期性伺服控制阶段时所在的控制模式，详见1.3.3伺服指令包的讲解。

### 1.4.5  三闭环控制参数说明

三闭环参数出厂时已调好，如非必要无需再调整，否则将造成不可预测的后果。

<a id="head1.5"/>
## 1.5 示例

### 1.5.1 CAN协议示例

- **例1： 读取ID为1的模块温度**

指令 = CMD\_RD；地址 = 0x06; 数据 = 0x02

指令包：

| **CAN-ID** | **DLC** | **Byte1** | **Byte2** | **Byte3** | **Byte4** | **Byte5** | **Byte6** | **Byte7** | **Byte8** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x01 | 0x03 | 0x01 | 0x06 | 0x02 | NULL | NULL | NULL | NULL | NULL |

应答包：

| **CAN-ID** | **DLC** | **Byte1** | **Byte2** | **Byte3** | **Byte4** | **Byte5** | **Byte6** | **Byte7** | **Byte8** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x101 | 0x04 | 0x01 | 0x06 | 0xF6 | 0x00 | NULL | NULL | NULL | NULL |

- **例2：设置ID为1的模块目标位置为10000**

指令 = CMD\_WR；地址 = 0x36; 数据 = 10000 = 0x00002710

指令包：

| **CAN-ID** | **DLC** | **Byte1** | **Byte2** | **Byte3** | **Byte4** | **Byte5** | **Byte6** | **Byte7** | **Byte8** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x01 | 0x06 | 0x02 | 0x36 | 0x10 | 0x27 | 0x00 | 0x00 | NULL | NULL |

应答包：

| **CAN-ID** | **DLC** | **Byte1** | **Byte2** | **Byte3** | **Byte4** | **Byte5** | **Byte6** | **Byte7** | **Byte8** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x101 | 0x03 | 0x02 | 0x36 | 0x01 | NULL | NULL | NULL | NULL | NULL |

- **例3：以无返回方式设置ID为1的模块目标位置为10000**

指令 = CMD\_WR\_NR；地址 = 0x36; 数据 = 1000 = 0x00002710

指令包：

| **CAN-ID** | **DLC** | **Byte1** | **Byte2** | **Byte3** | **Byte4** | **Byte5** | **Byte6** | **Byte7** | **Byte8** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x01 | 0x06 | 0x03 | 0x36 | 0x10 | 0x27 | 0x00 | 0x00 | NULL | NULL |

无应答包：
<a id="headx"/>
## 附录: