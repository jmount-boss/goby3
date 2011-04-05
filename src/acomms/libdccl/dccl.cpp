// copyright 2009-2011 t. schneider tes@mit.edu
// 
// this file is part of the Dynamic Compact Control Language (DCCL),
// the goby-acomms codec. goby-acomms is a collection of libraries 
// for acoustic underwater networking
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

#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/assign.hpp>
#include <crypto++/filters.h>
#include <crypto++/sha.h>
#include <crypto++/modes.h>
#include <crypto++/aes.h>

#include "dccl.h"
#include "dccl_field_codec_default.h"

#include "goby/util/logger.h"
#include "goby/util/string.h"
#include "goby/protobuf/acomms_proto_helpers.h"
#include "goby/protobuf/dccl_option_extensions.pb.h"
#include "goby/protobuf/header.pb.h"

using goby::util::goby_time;
using goby::util::as;
using google::protobuf::FieldDescriptor;
using google::protobuf::Descriptor;
using google::protobuf::Reflection;


const char* goby::acomms::DCCLCodec::DEFAULT_CODEC_NAME = "_default";
std::ostream*  goby::acomms::DCCLCodec::log_ = 0;

goby::acomms::DCCLCodec::FieldCodecManager goby::acomms::DCCLCodec::codec_manager_;
goby::acomms::DCCLCodec::CppTypeHelper goby::acomms::DCCLCodec::cpptype_helper_;
goby::acomms::protobuf::DCCLConfig goby::acomms::DCCLCodec::cfg_;
std::string goby::acomms::DCCLCodec::crypto_key_;
bool goby::acomms::DCCLCodec::default_codecs_set_ = false;
std::map<google::protobuf::int32, const google::protobuf::Descriptor*> goby::acomms::DCCLCodec::id2desc_;
google::protobuf::DynamicMessageFactory goby::acomms::DCCLCodec::message_factory_;

//
// DCCLCodec
//

void goby::acomms::DCCLCodec::set_default_codecs()
{    
    codec_manager_.add<double, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);
    codec_manager_.add<float, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);
    codec_manager_.add<bool, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);
    codec_manager_.add<int32, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);
    codec_manager_.add<int64, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);
    codec_manager_.add<uint32, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);
    codec_manager_.add<uint64, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);
    codec_manager_.add<std::string, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);
    codec_manager_.add<google::protobuf::EnumDescriptor, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);
    codec_manager_.add<google::protobuf::Message, DCCLDefaultFieldCodec>(DEFAULT_CODEC_NAME);

    //    add_field_codec<FieldDescriptor::CPPTYPE_STRING, DCCLTimeCodec>("_time");
//    add_field_codec<FieldDescriptor::CPPTYPE_STRING, DCCLModemIdConverterCodec>(
    //"_platform<->modem_id");

    default_codecs_set_ = true;
}

void goby::acomms::DCCLCodec::add_flex_groups(util::FlexOstream* tout)
{
    tout->add_group("dccl_enc", util::Colors::lt_magenta, "encoder messages (goby_dccl)");
    tout->add_group("dccl_dec", util::Colors::lt_blue, "decoder messages (goby_dccl)");
}


std::string goby::acomms::DCCLCodec::encode(const google::protobuf::Message& msg)
{
    const Descriptor* desc = msg.GetDescriptor();

    try
    {
        if(!id2desc_.count(desc->options().GetExtension(dccl::id)))
            throw(DCCLException("Message id " +
                                as<std::string>(desc->options().GetExtension(dccl::id))+
                                " has not been validated. Call validate() before encoding this type."));
    

        //fixed header
        Bitset ccl_id_bits(fixed_head_size(), DCCL_CCL_HEADER);
        Bitset dccl_id_bits(fixed_head_size(),
                            msg.GetDescriptor()->options().GetExtension(dccl::id));
        ccl_id_bits <<= HEAD_DCCL_ID_SIZE;    
        Bitset fixed_head_bits = ccl_id_bits | dccl_id_bits;
        Bitset head_bits, body_bits;

        boost::shared_ptr<DCCLFieldCodec> codec =
            codec_manager_.find(FieldDescriptor::CPPTYPE_MESSAGE,
                                desc->options().GetExtension(dccl::message_codec));
        if(codec)
        {
            codec->encode(&head_bits, msg, DCCLFieldCodec::HEAD);
            codec->encode(&body_bits, msg, DCCLFieldCodec::BODY);
        }
        else
        {
            throw(DCCLException("Failed to find dccl.message_codec `" + desc->options().GetExtension(dccl::message_codec) + "`"));
        }
        
        // given header of not even byte size (e.g. 01011), make even byte size (e.g. 00001011)
        // and shift bytes to MSB (e.g. 01011000).
        unsigned head_byte_size = ceil_bits2bytes(head_bits.size() + fixed_head_bits.size());
        unsigned head_bits_diff = head_byte_size * BITS_IN_BYTE - (head_bits.size() + + fixed_head_bits.size());
        head_bits.resize(head_bits.size() + head_bits_diff);
        for(int i = 0, n = fixed_head_bits.size(); i < n; ++i)
            head_bits.push_back(fixed_head_bits[i]);
        
        std::string head_bytes, body_bytes;
        bitset2string(head_bits, &head_bytes);
        bitset2string(body_bits, &body_bytes);
    
        if(!crypto_key_.empty())
            encrypt(&body_bytes, head_bytes);

        // reverse so the body reads LSB->MSB such that extra chars at
        // the end of the string get tacked on to the MSB, not the LSB where they would cause trouble
        std::reverse(body_bytes.begin(), body_bytes.end());

        std::cout << "head: " << head_bits << std::endl;
        std::cout << "body: " << body_bits << std::endl;
        return head_bytes + body_bytes;
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        
        ss << "Message " << desc->full_name() << " failed to encode. Reason: " << e.what() << std::endl;

        if(log_) *log_ << warn <<  ss.str();        
        throw(DCCLException(ss.str()));
    }
}

boost::shared_ptr<google::protobuf::Message> goby::acomms::DCCLCodec::decode(const std::string& bytes)   
{
    try
    {
        unsigned fixed_header_bytes = ceil_bits2bytes(fixed_head_size());

        if(bytes.length() < fixed_header_bytes)
            throw(DCCLException("Bytes passed (hex: " + hex_encode(bytes) + ") is too small to be a valid DCCL message"));
        
        Bitset fixed_header_bits;
        string2bitset(&fixed_header_bits, bytes.substr(0, fixed_header_bytes));

        std::cout << fixed_header_bits << std::endl;

        unsigned ccl_id = (fixed_header_bits >> (fixed_header_bits.size()-HEAD_CCL_ID_SIZE)).to_ulong();
        unsigned id = (fixed_header_bits >> (fixed_header_bits.size()-fixed_head_size())).to_ulong() & Bitset(std::string(HEAD_DCCL_ID_SIZE, '1')).to_ulong();

        if(ccl_id != DCCL_CCL_HEADER)
            throw(DCCLException("CCL ID (left-most byte in bytes string) must be " +
                                as<std::string>(DCCL_CCL_HEADER) + " but was given " + as<std::string>(ccl_id)));
        
        if(!id2desc_.count(id))
            throw(DCCLException("Message id " + as<std::string>(id) + " has not been validated. Call validate() before decoding this type."));

        // ownership of this object goes to the caller of decode()
        boost::shared_ptr<google::protobuf::Message> msg(message_factory_.GetPrototype(id2desc_.find(id)->second)->New());
        
        const Descriptor* desc = msg->GetDescriptor();
        
    
        boost::shared_ptr<DCCLFieldCodec> codec =
            codec_manager_.find(FieldDescriptor::CPPTYPE_MESSAGE,
                                desc->options().GetExtension(dccl::message_codec));
    
        unsigned head_size_bits = codec->max_size(desc, DCCLFieldCodec::HEAD) + fixed_head_size();
        unsigned body_size_bits = codec->max_size(desc, DCCLFieldCodec::BODY);

        unsigned head_size_bytes = ceil_bits2bytes(head_size_bits);
        unsigned body_size_bytes = ceil_bits2bytes(body_size_bits);
    
        std::cout << "head is " << head_size_bits << " bits" << std::endl;
        std::cout << "body is " << body_size_bits << " bits" << std::endl;
        std::cout << "head is " << head_size_bytes << " bytes" << std::endl;
        std::cout << "body is " << body_size_bytes << " bytes" << std::endl;

        std::string head_bytes = bytes.substr(0, head_size_bytes);
        std::string body_bytes = bytes.substr(head_size_bytes);
        // we had reversed the bytes so extraneous zeros will not cause trouble. undo this reversal.
        std::reverse(body_bytes.begin(), body_bytes.end());
        
        std::cout << "head is " << hex_encode(head_bytes) << std::endl;
        std::cout << "body is " << hex_encode(body_bytes) << std::endl;

        if(!crypto_key_.empty())
            decrypt(&body_bytes, head_bytes);
    
        Bitset head_bits, body_bits;
        string2bitset(&head_bits, head_bytes);
        string2bitset(&body_bits, body_bytes);

    
        std::cout << "head: " << head_bits << std::endl;
        std::cout << "body: " << body_bits << std::endl;    

        
        head_bits.resize(head_size_bits - fixed_head_size());

        std::cout << "head after removing fixed portion: " << head_bits << std::endl;

        if(codec)
        {
            codec->decode(&head_bits, msg, DCCLFieldCodec::HEAD);
            std::cout << "after header decode, message is: " << *msg << std::endl;
            codec->decode(&body_bits, msg, DCCLFieldCodec::BODY);
            std::cout << "after header & body decode, message is: " << *msg << std::endl;
        }
        else
        {
            throw(DCCLException("Failed to find dccl.message_codec `" + desc->options().GetExtension(dccl::message_codec) + "`"));
        }
        return msg;
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        
        ss << "Message " << hex_encode(bytes) <<  " failed to decode. Reason: " << e.what() << std::endl;

        if(log_) *log_ << warn <<  ss.str();        
        throw(DCCLException(ss.str()));
    }    
}

// makes sure we can actual encode / decode a message of this descriptor given the loaded FieldCodecs
// checks all bounds on the message
bool goby::acomms::DCCLCodec::validate(const google::protobuf::Descriptor* desc)
{
    if(!default_codecs_set_)
        set_default_codecs();
    
    try
    {
        if(!desc->options().HasExtension(dccl::id))
            throw(DCCLException("Missing message option `dccl.id`. Specify a unique id (e.g. 3) in the body of your .proto message using \"option (dccl.id) = 3\""));
        else if(!desc->options().HasExtension(dccl::max_bytes))
            throw(DCCLException("Missing message option `dccl.max_bytes`. Specify a maximum (encoded) message size in bytes (e.g. 32) in the body of your .proto message using \"option (dccl.max_bytes) = 32\""));
        
        boost::shared_ptr<DCCLFieldCodec> codec =
            codec_manager_.find(FieldDescriptor::CPPTYPE_MESSAGE,
                                desc->options().GetExtension(dccl::message_codec));

        
        const unsigned head_bit_size = codec->max_size(desc, DCCLFieldCodec::HEAD) + fixed_head_size();
        const unsigned body_bit_size = codec->max_size(desc, DCCLFieldCodec::BODY);
        
        const unsigned byte_size = ceil_bits2bytes(head_bit_size) + ceil_bits2bytes(body_bit_size);

        if(byte_size > desc->options().GetExtension(dccl::max_bytes))
            throw(DCCLException("Actual maximum size of message exceeds allowed maximum (dccl.max_bytes). Tighten bounds, remove fields, improve codecs, or increase the allowed dccl.max_bytes"));
        
        codec->validate(desc, DCCLFieldCodec::HEAD);
        codec->validate(desc, DCCLFieldCodec::BODY);

        unsigned dccl_id = desc->options().GetExtension(dccl::id);
        if(id2desc_.count(dccl_id) && desc != id2desc_.find(dccl_id)->second)
            throw(DCCLException("`dccl.id` " + as<std::string>(dccl_id) + " is already in use by Message " + id2desc_.find(dccl_id)->second->full_name()));
        else
            id2desc_.insert(std::make_pair(desc->options().GetExtension(dccl::id), desc));
    }
    catch(DCCLException& e)
    {
        try
        {
            if(log_) info(desc, log_);
        }
        catch(DCCLException& e)
        { }
        
        if(log_) *log_ << warn << "Message " << desc->full_name() << " failed validation. Reason: "
                       << e.what() <<  "\n"
                       << "If possible, information about the Message are printed above. " << std::endl;
        return false;
    }

    return true;
}

void goby::acomms::DCCLCodec::info(const google::protobuf::Descriptor* desc, std::ostream* os)
{
    if(!default_codecs_set_)
        set_default_codecs();
    
    try
    {   
        boost::shared_ptr<DCCLFieldCodec> codec =
            codec_manager_.find(FieldDescriptor::CPPTYPE_MESSAGE,
                                desc->options().GetExtension(dccl::message_codec));

        const unsigned config_head_bit_size = codec->max_size(desc, DCCLFieldCodec::HEAD);
        const unsigned body_bit_size = codec->max_size(desc, DCCLFieldCodec::BODY);
        const unsigned bit_size = fixed_head_size() + config_head_bit_size + body_bit_size;
        
        const unsigned byte_size = ceil_bits2bytes(config_head_bit_size + fixed_head_size())
            + ceil_bits2bytes(body_bit_size);
        
        const unsigned allowed_byte_size = desc->options().GetExtension(dccl::max_bytes);
        const unsigned allowed_bit_size = allowed_byte_size * BITS_IN_BYTE;
        
        *os << "== Begin " << desc->full_name() << " ==\n"
            << "Actual maximum size of message: " << byte_size << " bytes / "
            << byte_size*BITS_IN_BYTE  << " bits [fixed head: " << fixed_head_size()
            << ", user head: " << config_head_bit_size << ", body: "
            << body_bit_size << ", padding: " << byte_size * BITS_IN_BYTE - bit_size << "]\n"
            << "Allowed maximum size of message: " << allowed_byte_size << " bytes / "
            << allowed_bit_size << " bits\n";

        *os << "= Header =" << std::endl;
        codec->info(desc, os, DCCLFieldCodec::HEAD);
        *os << "= Body =" << std::endl;
        codec->info(desc, os, DCCLFieldCodec::BODY);
        
        *os << "== End " << desc->full_name() << " ==" << std::endl;
    }
    catch(DCCLException& e)
    {
        if(log_) *log_ << warn << "Message " << desc->full_name() << " cannot provide information due to invalid configuration. Reason: " << e.what() << std::endl;
    }
        
}


void goby::acomms::DCCLCodec::encrypt(std::string* s, const std::string& nonce /* message head */)
{
    using namespace CryptoPP;

    std::string iv;
    SHA256 hash;
    StringSource unused(nonce, true, new HashFilter(hash, new StringSink(iv)));
    
    CTR_Mode<AES>::Encryption encryptor;
    encryptor.SetKeyWithIV((byte*)crypto_key_.c_str(), crypto_key_.size(), (byte*)iv.c_str());

    std::string cipher;
    StreamTransformationFilter in(encryptor, new StringSink(cipher));
    in.Put((byte*)s->c_str(), s->size());
    in.MessageEnd();
    *s = cipher;
}

void goby::acomms::DCCLCodec::decrypt(std::string* s, const std::string& nonce)
{
    using namespace CryptoPP;

    std::string iv;
    SHA256 hash;
    StringSource unused(nonce, true, new HashFilter(hash, new StringSink(iv)));
    
    CTR_Mode<AES>::Decryption decryptor;    
    decryptor.SetKeyWithIV((byte*)crypto_key_.c_str(), crypto_key_.size(), (byte*)iv.c_str());
    
    std::string recovered;
    StreamTransformationFilter out(decryptor, new StringSink(recovered));
    out.Put((byte*)s->c_str(), s->size());
    out.MessageEnd();
    *s = recovered;
}

void goby::acomms::DCCLCodec::merge_cfg(const protobuf::DCCLConfig& cfg)
{
    cfg_.MergeFrom(cfg);
    process_cfg();
}

void goby::acomms::DCCLCodec::set_cfg(const protobuf::DCCLConfig& cfg)
{
    cfg_.CopyFrom(cfg);
    process_cfg();
}

void goby::acomms::DCCLCodec::process_cfg()
{
    if(cfg_.has_crypto_passphrase())
    {
        using namespace CryptoPP;
        
        SHA256 hash;
        StringSource unused(cfg_.crypto_passphrase(), true, new HashFilter(hash, new StringSink(crypto_key_)));
        
        if(log_) *log_ << group("dccl_enc") << "cryptography enabled with given passphrase" << std::endl;
    }
    else
    {
        if(log_) *log_ << group("dccl_enc") << "cryptography disabled, set crypto_passphrase to enable." << std::endl;
    }
    
}

//
// FieldCodecManager
//


const boost::shared_ptr<goby::acomms::DCCLFieldCodec>
goby::acomms::DCCLCodec::FieldCodecManager::find(
    google::protobuf::FieldDescriptor::CppType cpp_type, const std::string& name) const
{
    typedef InsideMap::const_iterator InsideIterator;
    typedef std::map<google::protobuf::FieldDescriptor::CppType, InsideMap>::const_iterator Iterator;
                    
    Iterator it = codecs_.find(cpp_type);
    if(it != codecs_.end())
    {
        InsideIterator inside_it = it->second.find(name);
        if(inside_it != it->second.end())
        {
            return inside_it->second;
        }
    }                    
    throw(DCCLException("No codec by the name `" + name + "` found for type: " + DCCLCodec::cpptype_helper().find(cpp_type)->as_str()));
}



//
// CppTypeHelper
//

goby::acomms::DCCLCodec::CppTypeHelper::CppTypeHelper()                
{
    using namespace google::protobuf;    
    using boost::shared_ptr;
    boost::assign::insert (map_)
        (static_cast<FieldDescriptor::CppType>(0),
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppTypeBase))
        (FieldDescriptor::CPPTYPE_DOUBLE,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_DOUBLE>))
        (FieldDescriptor::CPPTYPE_FLOAT,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_FLOAT>))
        (FieldDescriptor::CPPTYPE_UINT64,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_UINT64>))
        (FieldDescriptor::CPPTYPE_UINT32,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_UINT32>))
        (FieldDescriptor::CPPTYPE_INT32,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_INT32>))
        (FieldDescriptor::CPPTYPE_INT64,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_INT64>))
        (FieldDescriptor::CPPTYPE_BOOL,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_BOOL>))
        (FieldDescriptor::CPPTYPE_STRING,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_STRING>))
        (FieldDescriptor::CPPTYPE_MESSAGE,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_MESSAGE>))
        (FieldDescriptor::CPPTYPE_ENUM,
         shared_ptr<FromProtoCppTypeBase>(new FromProtoCppType<FieldDescriptor::CPPTYPE_ENUM>));
}

boost::shared_ptr<goby::acomms::FromProtoCppTypeBase> goby::acomms::DCCLCodec::CppTypeHelper::find(google::protobuf::FieldDescriptor::CppType cpptype)
{
    Map::iterator it = map_.find(cpptype);
    if(it != map_.end())
        return it->second;
    else
        return boost::shared_ptr<FromProtoCppTypeBase>();
}

const boost::shared_ptr<goby::acomms::FromProtoCppTypeBase> goby::acomms::DCCLCodec::CppTypeHelper::find(google::protobuf::FieldDescriptor::CppType cpptype) const
{
    Map::const_iterator it = map_.find(cpptype);
    if(it != map_.end())
        return it->second;
    else
        return boost::shared_ptr<FromProtoCppTypeBase>();
}
