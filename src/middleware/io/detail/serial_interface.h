// Copyright 2019-2020:
//   GobySoft, LLC (2013-)
//   Community contributors (see AUTHORS file)
// File authors:
//   Toby Schneider <toby@gobysoft.org>
//
//
// This file is part of the Goby Underwater Autonomy Project Libraries
// ("The Goby Libraries").
//
// The Goby Libraries are free software: you can redistribute them and/or modify
// them under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 2.1 of the License, or
// (at your option) any later version.
//
// The Goby Libraries are distributed in the hope that they will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Goby.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SerialInterface20190718H
#define SerialInterface20190718H

#include <memory>      // for shared_ptr
#include <ostream>     // for endl
#include <string>      // for operator+
#include <sys/ioctl.h> // for ioctl, TIOCM...

#include <boost/asio/serial_port.hpp> // for serial_port

#include "goby/middleware/io/detail/io_interface.h"     // for PubSubLayer
#include "goby/middleware/protobuf/io.pb.h"             // for SerialCommand
#include "goby/middleware/protobuf/serial_config.pb.h"  // for SerialConfig
#include "goby/util/debug_logger/flex_ostream.h"        // for glog, FlexOs...
#include "goby/util/debug_logger/logger_manipulators.h" // for group
namespace goby
{
namespace middleware
{
class Group;
}
} // namespace goby

namespace goby
{
namespace middleware
{
namespace io
{
namespace detail
{
template <const goby::middleware::Group& line_in_group,
          const goby::middleware::Group& line_out_group, PubSubLayer publish_layer,
          PubSubLayer subscribe_layer, template <class> class ThreadType>
class SerialThread : public IOThread<line_in_group, line_out_group, publish_layer, subscribe_layer,
                                     goby::middleware::protobuf::SerialConfig,
                                     boost::asio::serial_port, ThreadType>
{
    using Base =
        IOThread<line_in_group, line_out_group, publish_layer, subscribe_layer,
                 goby::middleware::protobuf::SerialConfig, boost::asio::serial_port, ThreadType>;

  public:
    /// \brief Constructs the thread.
    /// \param config A reference to the Protocol Buffers config read by the main application at launch
    /// \param index Thread index for multiple instances in a given application (-1 indicates a single instance)
    SerialThread(const goby::middleware::protobuf::SerialConfig& config, int index = -1)
        : Base(config, index, std::string("serial: ") + config.port())
    {
        auto command_out_callback =
            [this](std::shared_ptr<const goby::middleware::protobuf::SerialCommand> cmd) {
                goby::glog.is_debug2() && goby::glog << group(this->glog_group()) << "< [Command] "
                                                     << cmd->ShortDebugString() << std::endl;
                switch (cmd->command())
                {
                    case protobuf::SerialCommand::SEND_BREAK:
                        if (this->socket_is_open())
                            this->mutable_serial_port().send_break();
                        break;

                        // sets RTS high, needed for PHSEN and PCO2W comms
                    case protobuf::SerialCommand::RTS_HIGH:
                        if (this->socket_is_open())
                        {
                            int fd = this->mutable_serial_port().native_handle();
                            int RTS_flag = TIOCM_RTS;
                            // TIOCMBIS - set bit
                            ioctl(fd, TIOCMBIS, &RTS_flag);
                        }
                        break;

                    case protobuf::SerialCommand::RTS_LOW:
                        if (this->socket_is_open())
                        {
                            int fd = this->mutable_serial_port().native_handle();
                            int RTS_flag = TIOCM_RTS;
                            // TIOCMBIC - clear bit
                            ioctl(fd, TIOCMBIC, &RTS_flag);
                        }
                        break;
                }
            };

        this->subscribe_transporter()
            .template subscribe<line_out_group, goby::middleware::protobuf::SerialCommand>(
                command_out_callback);
    }

    ~SerialThread()
    {
        this->subscribe_transporter()
            .template unsubscribe<line_out_group, goby::middleware::protobuf::SerialCommand>();
    }

  protected:
    /// \brief Access the (mutable) serial_port object
    boost::asio::serial_port& mutable_serial_port() { return this->mutable_socket(); }

  private:
    void async_write(std::shared_ptr<const goby::middleware::protobuf::IOData> io_msg) override
    {
        basic_async_write(this, io_msg);
    }

    void open_socket() override;
};
} // namespace detail
} // namespace io
} // namespace middleware
} // namespace goby

template <const goby::middleware::Group& line_in_group,
          const goby::middleware::Group& line_out_group,
          goby::middleware::io::PubSubLayer publish_layer,
          goby::middleware::io::PubSubLayer subscribe_layer, template <class> class ThreadType>
void goby::middleware::io::detail::SerialThread<line_in_group, line_out_group, publish_layer,
                                                subscribe_layer, ThreadType>::open_socket()
{
    this->mutable_serial_port().open(this->cfg().port());
    using boost::asio::serial_port_base;
    this->mutable_serial_port().set_option(serial_port_base::baud_rate(this->cfg().baud()));

    switch (this->cfg().flow_control())
    {
        case goby::middleware::protobuf::SerialConfig::NONE:
            this->mutable_serial_port().set_option(
                serial_port_base::flow_control(serial_port_base::flow_control::none));
            break;
        case goby::middleware::protobuf::SerialConfig::SOFTWARE:
            this->mutable_serial_port().set_option(
                serial_port_base::flow_control(serial_port_base::flow_control::software));
            break;
        case goby::middleware::protobuf::SerialConfig::HARDWARE:
            this->mutable_serial_port().set_option(
                serial_port_base::flow_control(serial_port_base::flow_control::hardware));
            break;
    }

    // 8N1
    this->mutable_serial_port().set_option(serial_port_base::character_size(8));
    this->mutable_serial_port().set_option(
        serial_port_base::parity(serial_port_base::parity::none));
    this->mutable_serial_port().set_option(
        serial_port_base::stop_bits(serial_port_base::stop_bits::one));
}

#endif
