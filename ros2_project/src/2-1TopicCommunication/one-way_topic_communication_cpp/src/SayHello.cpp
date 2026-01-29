#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <rclcpp/node.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/timer.hpp>

class SayHello : public rclcpp::Node{
public:
    // 构造函数
    SayHello(std::string name):rclcpp::Node(name){
        // 创建发布者
        say_hello = this->create_publisher<std_msgs::msg::String>("/HelloWorld",10);

        // 创建定时器，用于定时发布信息
        say_hello_timer = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&SayHello::PubHello,this));
    }

private:
    /*
        函数
    */
    void PubHello(){
        // 创建消息
        std_msgs::msg::String msg;
        msg.data = "你好世界";      // 我们可以发现std_msgs::msg::String有一个属性data，而data刚好是C++的std::string类型

        // 发布者发布消息
        say_hello->publish(msg);

        // DEBUG，输出一些东西到终端，让我们知道这个PubHello函数确实被执行了
        RCLCPP_INFO(this->get_logger(),"发布者发布了：%s",msg.data.c_str());    // c_str()表示转换字符串为c语言风格，这是ros2中输出日志的函数需要的
    }

    /*
        声明变量
    */
    // 声明一个发布者指针（其实我们理解为发布者就可以了）
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr say_hello;
    
    // 声明一个定时器指针（其实我们理解为定时器就可以了）
    // 定时器用于根据一定的频率来执行某一个回调函数，我们通过这个回调函数来使Publisher发布消息，就实现发布者按某个频率一直发布消息
    rclcpp::TimerBase::SharedPtr say_hello_timer;

};


int main(int argc, char ** argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SayHello>("PubHello");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}



