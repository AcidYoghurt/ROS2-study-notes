#include <rclcpp/rclcpp.hpp>
#include <recognizer/recognizer.hpp>

class UseRecognize : public rclcpp::Node
{
public:
    UseRecognize():Node("UseRecognizeNode")
    {
        *recognizer = recognize_pkg::Recognizer();
        recognizer->run();
    }
private:
    recognize_pkg::Recognizer* recognizer;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<UseRecognize>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
