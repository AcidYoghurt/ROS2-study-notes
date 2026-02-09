# 1 ROS2系统架构

![ROS2架构图](./assets/image-20220602204152352-1770179594002-3-1770179596676-5.png)

## 1.1 操作系统层(Operating System Layer)

操作系统层比较好理解，ROS2本身就是基于Linux、Windows或者macOS系统建立的，驱动计算机硬件、底层网络通信等实现都是交由操作系统来实现的。



## 1.2 DDS实现层(DDS Implementation Layer)

### DDS定义

- `DDS`，全称`Data Distribution Service (数据分发服务)`，是新一代分布式实时通信中间件协议，采用发布/订阅体系架构，强调以数据为中心，提供丰富的QoS服务质量策略，以保障数据进行实时、高效、灵活地分发，可满足各种分布式实时通信应用需求。

DDS规范是由`OMG`（Object Management Group）对象管理组织发布的。

- `DDS`的相关标准包括核心协议([DDSI-RTPS](https://zhida.zhihu.com/search?content_id=131018066&content_type=Article&match_order=1&q=DDSI-RTPS&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NzAzNTQ4MjEsInEiOiJERFNJLVJUUFMiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxMzEwMTgwNjYsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.RPDcg8AnnbOaDZwoyZbmVs4hSz37b87RqphhtO03PWw&zhida_source=entity)，[DDS-XTypes](https://zhida.zhihu.com/search?content_id=131018066&content_type=Article&match_order=1&q=DDS-XTypes&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NzAzNTQ4MjEsInEiOiJERFMtWFR5cGVzIiwiemhpZGFfc291cmNlIjoiZW50aXR5IiwiY29udGVudF9pZCI6MTMxMDE4MDY2LCJjb250ZW50X3R5cGUiOiJBcnRpY2xlIiwibWF0Y2hfb3JkZXIiOjEsInpkX3Rva2VuIjpudWxsfQ.mGWSt4JX1g2BzyFt5di8fWHdhlKcAmrl2p1qLGZlIwQ&zhida_source=entity)，[DDS-Security](https://zhida.zhihu.com/search?content_id=131018066&content_type=Article&match_order=1&q=DDS-Security&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NzAzNTQ4MjEsInEiOiJERFMtU2VjdXJpdHkiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxMzEwMTgwNjYsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.mig5mEzZ_q0mUuRXy_Ekdq7uOVBYkiLiTuwxjtd58Iw&zhida_source=entity)，Interface Definition Language (IDL)…)，API(DDS C++ API，DDS Java API)，拓展协议(DDS-RPC，DDS-XML…)等13份协议集合。

  <img src="./assets/v2-83ec29d3d590582cb8d07f8ca1a8a4fa_r.jpg" alt="img" style="zoom: 67%;" />

  > [!NOTE]
  >
  > 该图中
  >
  > - `DDS`：一整块 Middleware 的规范 + 实现
  >   - `API层`：DDS 暴露给应用开发者的接口层
  >   - `Presentation 层`**（DDS 的核心抽象）**：这里定义了 DDS“是什么样的通信模型”
  >     - Topic / Type（IDL、数据模型）
  >     - 数据序列化（CDR、XCDR）
  >     - QoS（Reliability、Deadline、History、LatencyBudget…）
  >     - Content Filtered Topic
  >     - 本地缓存（DataReader history）
  >   -  `Protocol 层`（DDS 怎么跑起来）：DDS 的网络协议
  >     - 自动发现
  >     - 心跳
  >     - 重传、可靠性、liveliness
  >
  > - DDS使用操作系统提供的API，制作出更易用的API提供给应用程序



### DDS发布订阅模型DCPS

讲具体协议之前，我们先熟悉下几个专有名词：

- **Domain：**代表一个通信平面，由Domain ID唯一标识，只有在**同一个域内的通信实体**才可以通信；
- **Domain Participant：**代表域内通信的应用程序的本地成员身份，简单来说，就是说明同一数据域内的通信成员；
- **Topic：**是数据的抽象概念，由TopicName标识，关联相应数据的数据类型(DataType)，如果把车内所涉及的所有Topic集合在一起，这样就形成一个虚拟的全局数据空间“Global Data Space”，进一步弱化了节点的概念，所以域参与者已经不是节点的概念了；
- **DataWriter：**数据写入者，类似缓存，把需要发布的主题数据从应用层写入到DataWriter中；
- **DataReader：**数据读取者，同样可以理解为一种缓存，从订阅者得到主题数据，随之传给应用层；
- **Publisher：**发布者，发布主题数据，至少与1个DataWriter关联，通过调用DataWriter的相关函数将数据发出去；
- **Subscriber：**订阅者，订阅主题数据，至少与1个DataReader关联。当数据到达时，应用程序可能忙于执行其他操作或应用程序只是等待该消息时，这样就会存在两种情况，同步访问和异步通知。

根据前面介绍，我们清楚了DDS是一个以数据为中心的中间件协议和API标准，意为

- 用户只关心自己想要的数据，数据通过`Topic`进行标识

- **发布者 **根据**主题**发布数据，

- **订阅者** 根据自己感兴趣的**主题**订阅数据。

这便是DDS的核心：`以数据为中心的发布-订阅模型DCPS`

![img](./assets/v2-ff84b83ecac6b84f96d3631827080188_r.jpg)



### QoS服务质量

我们在ROS2中最常用的：

- `RELIABILITY（可靠性）`：表示数据能不能丢
  - `BEST_EFFORT`：可能丢，不重传（快）
  - `RELIABLE`：不丢，丢了重传（慢一点）

- `HISTORY（历史深度）`：缓存多少条历史数据

  - `KEEP_LAST(N)`：只保留最近 N 条
  - `KEEP_ALL`：全都保留（直到资源耗尽）

- `DEPTH（历史队列多深）`：最多允许落后多少条消息

  > [!NOTE]
  >
  > HISTORY = KEEP_LAST 时才有意义

  - depth = 1  → 只要最新，适用于控制回路
  - depth = 10 → 允许短暂卡顿，适用于感知处理链

- `DURABILITY（后来的人能不能看到过去）`：订阅者晚来，还能不能拿到关键数据

  - `VOLATILE`：只管现在
  - `TRANSIENT_LOCAL`：保存最后几条给新人

- `DEADLINE（你多久不来我就报警）`：检测“该来的数据没来”

  - 例如：

    camera 应该 33ms 来一帧
    设置 deadline = 40ms
    如果40ms后camera没有信息，就认为camera节点死了

- `LIVELINESS（你是不是还活着）`：发布者还在不在
  - `AUTOMATIC`：DDS 自动维护
  - `MANUAL_BY_TOPIC`：发布者必须“定期报平安”



### DDS实现层是什么

> DDS实现层 就是原始的 DDS Middleware

`DDS实现层` 为 `RMW层` 提供接口，其中DDS的具体实现有**三大流派**：

- `eProsima Fast DDS` 对应了 rmw中的 **rmw_fastrtps_cpp**
  - **定位**：ROS2的默认引擎
  - **优点**：上手成本低
  - **缺点**：Discovery偏慢（尤其多节点），实时性一般
- `Eclipse Cyclone DDS` 对应了 rmw中的 **rmw_cyclonedds_cpp**
  - **定位**：实时性 / 确定性优先
  - **优点**：Discovery快，内存管理克制
  - **缺点**：配置偏底层
- `RTI Connext DDS` 对应了 rmw中的 **rmw_connextdds**
  - **定位**：工业 / 军工 / 航天级 DDS
  - **优点**：实时性和可靠性天花板，QoS 实现最完整
  - **缺点**：要钱

| 维度           | Fast DDS | Cyclone DDS | Connext DDS |
| -------------- | -------- | ----------- | ----------- |
| ROS2 默认      | ✅        | ❌           | ❌           |
| Discovery 速度 | 中       | 快          | 快          |
| 延迟           | 中       | 低          | 很低        |
| 抖动           | 中       | 低          | 极低        |
| 配置复杂度     | 低       | 中          | 高          |
| 工具链         | 一般     | 一般        | 极强        |
| 商用支持       | 可选     | 社区        | 企业级      |



## 1.3 抽象DDS层-RMW中间件(Abstract DDS Layer)

> [!NOTE]
>
> ROS 2 通过 rmw 定义了一套统一的中间件接口，不同的 rmw 实现对各自的 DDS 实现进行适配和封装，从而为 ROS 2 上层提供一致的通信 API。

这一层将DDS实现层进一步的封装，使得DDS更容易使用。原因在于DDS需要大量的设置和配置（分区，主题名称，发现模式，消息创建,...），这些设置都是在ROS2的抽象层中完成的。



## 1.4 ROS2客户端库 RCL

> `RCL（ROS Client Library）`ROS客户端库，其实就是ROS的一种API，提供了对ROS话题、服务、参数、Action等接口。

- 不同的语言对应着不同的rcl，但基本功能都是相同的。

- 比如Python语言提供了rclpy来操作ROS2的节点话题服务等，而C++则使用rclcpp提供API操作ROS2的节点话题和服务等。

- 所以后面我们使用Python和C++来编写ROS2节点实现通讯等功能时，我们就会引入rclpy和rclcpp的库。

<img src="./assets/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBA6bG86aaZUk9T,size_20,color_FFFFFF,t_70,g_se,x_16.png" alt="rcl与rmw" style="zoom: 50%;" />

上面这张图是`ROS2的API`的实现层级，

- 最下面的是第三方的DDS

- rmw（中间件接口）层是对各家DDS的抽象层

- 基于rmw实现了rclc，有了rclc，我们就可以实现各个语言的库，大家都知道C语言是各个语言的鼻祖（汇编除外）

- 所以基于rclc，ROS2官方实现了rclpy和rclcpp

- 基于rclpy和rclcpp我们就可以实现上层的应用了



## 1.5 应用层

应用层就是我们写代码以及ROS2开发的各种常用的机器人相关开发工具所在的层了。后面我们写的所有代码其实都是属于这一层的。



