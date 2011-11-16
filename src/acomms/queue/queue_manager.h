// copyright 2009 t. schneider tes@mit.edu 
//
// this file is part of the Queue Library (libqueue),
// the goby-acomms message queue manager. goby-acomms is a collection of 
// libraries for acoustic underwater networking
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


#ifndef QueueManager20091204_H
#define QueueManager20091204_H

#include <limits>
#include <set>
#include <boost/bind.hpp>
#include <boost/signal.hpp>

#include "goby/acomms/dccl.h"
#include "goby/acomms/protobuf/queue.pb.h"

#include <map>
#include <deque>

#include "queue_exception.h"
#include "queue.h"


namespace goby
{
    namespace acomms
    {
        /// \class QueueManager queue.h goby/acomms/queue.h
        /// \brief provides an API to the goby-acomms Queuing Library.
        /// \ingroup acomms_api
        /// \sa acomms_queue.proto and acomms_modem_message.proto for definition of Google Protocol Buffers messages (namespace goby::acomms::protobuf).
         class QueueManager
         {
           public:
             /// constructor
             QueueManager();
             /// destructor
             ~QueueManager()
             { }

             /// \name Initialization Methods
             ///
             /// These methods are intended to be called before doing any work with the class.
             //@{

             /// \brief Set (and overwrite completely if present) the current configuration. (protobuf::QueueManagerConfig defined in acomms_queue.proto)
             void set_cfg(const protobuf::QueueManagerConfig& cfg);

             /// \brief Set (and merge "repeat" fields) the current configuration. (protobuf::QueueManagerConfig defined in acomms_queue.proto)
             void merge_cfg(const protobuf::QueueManagerConfig& cfg);

             /// \brief Add a DCCL queue for use with QueueManager. Note that the queue must be added before receiving messages with QueueManager.
             ///
             /// \tparam ProtobufMessage Any Google Protobuf Message generated by protoc (i.e. subclass of google::protobuf::Message)
             template<typename ProtobufMessage>
                 void add_queue()
             {
                 add_queue(ProtobufMessage::descriptor());
             }

             /// \brief Alternative method for adding Queues when using Dynamic Protobuf Messages
             void add_queue(const google::protobuf::Descriptor* desc);
             //@}

             /// \name Application level Push/Receive Methods
             ///
             /// These methods are the primary higher level interface to the QueueManager. From here you can push messages
             /// and set the callbacks to use on received messages.
             //@{

             /// \brief Push a message (and add the queue if it does not exist)
             ///
             /// \param new_message DCCL message to push.
             void push_message(const google::protobuf::Message& new_message);

             /// \brief Flush (delete all messages in) a queue
             ///
             /// \param flush QueueFlush object containing details about queue to flush
             void flush_queue(const protobuf::QueueFlush& flush);
             //@}

             /// \name Modem Slots
             ///
             /// These methods are the interface to the QueueManager from the %modem driver.
             //@{

             /// \brief Finds data to send to the %modem.
             /// 
             /// Data from the highest priority %queue(s) will be combined to form a message equal or less than the size requested in ModemMessage message_in. If using one of the classes inheriting ModemDriverBase, this method should be connected to ModemDriverBase::signal_data_request.
             /// \param msg The ModemTransmission containing information about the data request and is the place where the request data will be stored (in the repeated field ModemTransmission::frame).
             /// \return true if successful in finding data to send, false if no data is available
             void handle_modem_data_request(protobuf::ModemTransmission* msg);

             /// \brief Receive incoming data from the %modem.
             ///
             /// If using one of the classes inheriting ModemDriverBase, this method should be bound and passed to ModemDriverBase::signal_receive.
             /// \param message The received ModemMessage.
             void handle_modem_receive(const protobuf::ModemTransmission& message);

             //@}


             /// \name Control
             ///
             /// Call these methods when you want the QueueManager to perform time sensitive tasks (such as expiring old messages)
             //@{
             /// \brief Calculates which messages have expired and emits the goby::acomms::QueueManager::signal_expire as necessary.
             void do_work();

             //@}

             /// \name Informational Methods
             ///
             //@{        

             /// \brief Writes a human readable summary (including DCCLCodec info) of all loaded queues.
             ///
             /// \param os Pointer to a stream to write this information
             void info_all(std::ostream* os) const;


             /// \brief Writes a human readable summary (including DCCLCodec info) of the queue for the provided DCCL type to the stream provided.
             ///
             /// \tparam ProtobufMessage Any Google Protobuf Message generated by protoc (i.e. subclass of google::protobuf::Message)
             /// \param os Pointer to a stream to write this information
             template<typename ProtobufMessage>
                 void info(std::ostream* os) const 
             {
                 info(ProtobufMessage::descriptor(), os);
             }

             /// \brief An alterative form for getting information for Queues for message types <i>not</i> known at compile-time ("dynamic").
             void info(const google::protobuf::Descriptor* desc, std::ostream* os) const;

             const std::string& glog_push_group() { return glog_push_group_; }
             const std::string& glog_pop_group() { return glog_pop_group_; }            
             const std::string& glog_priority_group() { return glog_priority_group_; }            
             const std::string& glog_out_group() { return glog_out_group_; }
             const std::string& glog_in_group(){ return glog_in_group_; }


             /// \brief The current modem ID (MAC address) of this node.
            int modem_id() { return modem_id_; }
            
            //@}
        
            /// \name Application Signals
            //@{
            /// \brief Signals when acknowledgment of proper message receipt has been received. This is only sent for queues with queue.ack == true with an explicit destination (ModemMessageBase::dest() != 0)
            ///
            /// \param ack_msg a message containing details of the acknowledgment and the acknowledged transmission. (protobuf::ModemMsgAck is defined in acomms_modem_message.proto)        
            boost::signal<void (const protobuf::ModemTransmission& ack_msg,
                                const google::protobuf::Message& orig_msg)> signal_ack;

            /// \brief Signals when a DCCL message is received.
            ///
            /// \param msg the received transmission.
            boost::signal<void (const google::protobuf::Message& msg) > signal_receive;
            
            /// \brief Signals when a message is expires (exceeds its time-to-live or ttl) before being sent (if queue.ack == false) or before being acknowledged (if queue.ack == true).
            ///
            /// \param expire_msg the expired transmission. (protobuf::ModemDataExpire is defined in acomms_modem_message.proto)
            boost::signal<void (const google::protobuf::Message& orig_msg)> signal_expire;
            
            /// \brief Forwards the data request to the application layer. This advanced feature is used when queue.encode_on_demand == true and allows for the application to provide data immediately before it is actually sent (for highly time sensitive data)
            ///
            /// \param request_msg the details of the requested data. (protobuf::ModemDataRequest is defined in acomms_modem_message.proto)
            /// \param data_msg pointer to store the supplied data. The message is of the type for this queue.
            boost::signal<void (const protobuf::ModemTransmission& request_msg,
                                google::protobuf::Message* data_msg)> signal_data_on_demand;

            /// \brief Signals when any queue changes size (message is popped or pushed)
            ///
            /// \param size message containing the queue that changed size and its new size (protobuf::QueueSize is defined in acomms_queue.proto).
            boost::signal<void (protobuf::QueueSize size)> signal_queue_size_change;
            //@}

            /// \example acomms/queue/queue_simple/queue_simple.cpp
            /// simple.proto
            /// \verbinclude simple.proto
            /// queue_simple.cpp
            /// \example acomms/chat/chat.cpp
            

            
          private:
            QueueManager(const QueueManager&);
            QueueManager& operator= (const QueueManager&);
            //@}
            
            void qsize(Queue* q);
        
            // finds the %queue with the highest priority
            Queue* find_next_sender(const protobuf::ModemTransmission& message,
                                    const std::string& data,
                                    bool first_user_frame);
        
            // clears the destination and ack values for the packet to reset for next $CADRQ
            void clear_packet(); 
            void process_cfg();

            void process_modem_ack(const protobuf::ModemTransmission& ack_msg);

            void set_latest_metadata(const boost::any& field_value,
                                     const boost::any& wire_value,
                                     const boost::any& extension_value);

            
          private:
            static boost::shared_ptr<QueueManager> inst_;
            protobuf::QueuedMessageMeta latest_meta_;

            friend class Queue;
            int modem_id_;
            std::map<unsigned, Queue> queues_;
            
            // map frame number onto %queue pointer that contains
            // the data for this ack
            std::multimap<unsigned, Queue*> waiting_for_ack_;

            // the first *user* frame sets the tone (dest & ack) for the entire packet (all %modem frames)
            unsigned packet_ack_;
            int packet_dest_;
 
            protobuf::QueueManagerConfig cfg_;            

            goby::acomms::DCCLCodec* codec_;

            std::string glog_push_group_;
            std::string glog_pop_group_;
            std::string glog_priority_group_;
            std::string glog_out_group_;
            std::string glog_in_group_;            

            static int count_;
        };

        /// outputs information about all available messages (same as info_all)
        std::ostream& operator<< (std::ostream& out, const QueueManager& d);

        
    }

}


#endif
