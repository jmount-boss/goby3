#include "goby/middleware/single-thread-application.h"
#include <boost/units/io.hpp>
#include <sys/types.h>
#include <sys/wait.h>

#include "test.pb.h"
using goby::glog;
using namespace goby::common::logger;

extern constexpr goby::Group widget1{"Widget1"};

using Base = goby::SingleThreadApplication<TestConfig>;
class TestApp : public Base
{
public:
    TestApp() : Base(10)
        {
            transporter().subscribe<widget1, Widget>([this](const Widget& w) { post(w); });
        }

    void loop() override
        {
            static int i = 0;
            ++i;
            if(i < 2)
            {
                return;
            }
            else if(i > (std::max(5, 2+(int)loop_frequency_hertz())))
            {
                quit();
            }
            else
            {
                assert(rx_count_ == tx_count_);
                std::cout << goby::common::goby_time() << std::endl;
                Widget w;
                w.set_b(++tx_count_);
                std::cout << "Tx: " << w.DebugString() << std::flush;
                transporter().publish<widget1>(w);
            }
            
        }

    void post(const Widget& widget)
        {
            std::cout << "Rx: " << widget.DebugString() << std::flush;
            assert(widget.b() == tx_count_);
            ++rx_count_;
        }
    
    
private:
    int tx_count_ { 0 };
    int rx_count_ { 0 };
    
};
    


int main(int argc, char* argv[])
{
    int child_pid = fork();


    std::unique_ptr<std::thread> t2, t3;
    std::unique_ptr<zmq::context_t> manager_context;
    std::unique_ptr<zmq::context_t> router_context;

    if(child_pid != 0)
    {
        goby::protobuf::InterProcessPortalConfig cfg;
        manager_context.reset(new zmq::context_t(1));
        router_context.reset(new zmq::context_t(1));
        goby::ZMQRouter router(*router_context, cfg);
        t2.reset(new std::thread([&] { router.run(); }));
        goby::ZMQManager manager(*manager_context, cfg, router);
        t3.reset(new std::thread([&] { manager.run(); }));
        int wstatus;
        wait(&wstatus);
        router_context.reset();
        manager_context.reset();
        t2->join();
        t3->join();
        if(wstatus != 0) exit(EXIT_FAILURE);
    }
    else
    {
        sleep(1);
        return goby::run<TestApp>(argc, argv);
    }
}
    
