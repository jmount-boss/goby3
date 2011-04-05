// copyright 2011 t. schneider tes@mit.edu
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this software.  If not, see <http://www.gnu.org/licenses/>.


#include "goby/acomms/dccl.h"
#include "test.pb.h"
#include "goby/util/string.h"
#include "goby/util/time.h"

using goby::acomms::operator<<;

int main()
{
    goby::acomms::DCCLCodec::set_log(&std::cerr);    
    goby::acomms::protobuf::DCCLConfig cfg;
    cfg.set_crypto_passphrase("my_passphrase!");
    goby::acomms::DCCLCodec::set_cfg(cfg);

    TestMsg msg_in;
    int i = 0;
    msg_in.set_double_default(++i + 0.1);
    msg_in.set_float_default(++i + 0.2);

    msg_in.set_int32_default(++i);
    msg_in.set_int64_default(-++i);
    msg_in.set_uint32_default(++i);
    msg_in.set_uint64_default(++i);
    msg_in.set_sint32_default(-++i);
    msg_in.set_sint64_default(++i);
    msg_in.set_fixed32_default(++i);
    msg_in.set_fixed64_default(++i);
    msg_in.set_sfixed32_default(++i);
    msg_in.set_sfixed64_default(-++i);

    msg_in.set_bool_default(true);

    msg_in.set_string_default("abc123");
    msg_in.set_bytes_default(goby::acomms::hex_decode("aabbcc1234"));
    
    msg_in.set_enum_default(ENUM_C);
    msg_in.mutable_msg_default()->set_val(++i + 0.3);
    msg_in.mutable_msg_default()->mutable_msg()->set_val(++i);

    for(int j = 0; j < 2; ++j)
    {
        msg_in.add_double_default_repeat(++i + 0.1);
        msg_in.add_float_default_repeat(++i + 0.2);
        
        msg_in.add_int32_default_repeat(++i);
        msg_in.add_int64_default_repeat(-++i);
        msg_in.add_uint32_default_repeat(++i);
        msg_in.add_uint64_default_repeat(++i);
        msg_in.add_sint32_default_repeat(-++i);
        msg_in.add_sint64_default_repeat(++i);
        msg_in.add_fixed32_default_repeat(++i);
        msg_in.add_fixed64_default_repeat(++i);
        msg_in.add_sfixed32_default_repeat(++i);
        msg_in.add_sfixed64_default_repeat(-++i);
        
        msg_in.add_bool_default_repeat(true);
        
        msg_in.add_string_default_repeat("abc123");
        msg_in.add_bytes_default_repeat(goby::acomms::hex_decode("aabbcc1234"));
        
        msg_in.add_enum_default_repeat(static_cast<Enum1>((++i % 3) + 1));
        EmbeddedMsg1* em_msg = msg_in.add_msg_default_repeat();
        em_msg->set_val(++i + 0.3);
        em_msg->mutable_msg()->set_val(++i);
    }


    goby::acomms::DCCLCodec::info(msg_in.GetDescriptor(), &std::cout);    
    
    std::cout << "Message in:\n" << msg_in.DebugString() << std::endl;
    
    assert(goby::acomms::DCCLCodec::validate(msg_in.GetDescriptor()));

    std::cout << "Try encode..." << std::endl;
    std::string bytes = goby::acomms::DCCLCodec::encode(msg_in);
    std::cout << "... got bytes (hex): " << goby::acomms::hex_encode(bytes) << std::endl;

    std::cout << "Try decode..." << std::endl;

    boost::shared_ptr<google::protobuf::Message> msg_out = goby::acomms::DCCLCodec::decode(bytes);

    std::cout << "... got Message out:\n" << msg_out->DebugString() << std::endl;
    
    assert(msg_in.SerializeAsString() == msg_out->SerializeAsString());
    
    std::cout << "all tests passed" << std::endl;
}

