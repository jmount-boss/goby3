// copyright 2009 t. schneider tes@mit.edu
// 
// this file is part of flex-ostream, a terminal display library
// that provides an ostream with both terminal display and file logging
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

#ifndef FlexOstream20091211H
#define FlexOstream20091211H

#include <iostream>
#include <string>
#include <iomanip>


#include <google/protobuf/text_format.h>
#include <google/protobuf/io/tokenizer.h>

#include <boost/static_assert.hpp>
#include <boost/static_assert.hpp>

#include "flex_ostreambuf.h"
#include "logger_manipulators.h"

namespace goby
{

    namespace util
    {
        
        namespace logger_lock
        {
            /// Mutex actions available to the Goby logger (glogger)
            enum LockAction { none, lock };
        }

        /// Forms the basis of the Goby logger: std::ostream derived class for holding the FlexOStreamBuf
        class FlexOstream : public std::ostream
        {
          public:
          FlexOstream()
              : std::ostream(&sb_)
            {
                ++instances_;
                if(instances_ > 1)
                {
                    std::cerr << "Fatal error: cannot create more than one instance of FlexOstream. Use global object goby::glog to access the Goby Logger. Do not instantiate the FlexOstream directly." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            ~FlexOstream() { }
            FlexOstream(const FlexOstream&);
            FlexOstream& operator = (const FlexOstream&);            


            /// \name Initialization
            //@{
            /// Add another group to the logger. A group provides related manipulator for categorizing log messages.
            /// For thread safe use use boost::scoped_lock on Logger::mutex
            void add_group(const std::string & name,
                           Colors::Color color = Colors::nocolor,
                           const std::string & description = "");
            
            /// Set the name of the application that the logger is serving.
            void set_name(const std::string & s)
            {
                sb_.name(s);
            }
            
            bool is(std::ostream& (*pf) (std::ostream&), logger_lock::LockAction lock_action = logger_lock::none); 
            
            void add_stream(const std::string& verbosity, std::ostream* os = 0)
            {
                if(verbosity == "scope" || verbosity == "gui")        
                    add_stream(goby::util::Logger::GUI, os);
                else if(verbosity == "quiet")        
                    add_stream(goby::util::Logger::QUIET, os);
                else if(verbosity == "terse" || verbosity == "warn")        
                    add_stream(goby::util::Logger::WARN, os);
                else if(verbosity == "debug")        
                    add_stream(goby::util::Logger::DEBUG1, os);
                else
                    add_stream(goby::util::Logger::VERBOSE, os);
            }
            
            
            /// Attach a stream object (e.g. std::cout, std::ofstream, ...) to the logger with desired verbosity
            void add_stream(Logger::Verbosity verbosity = Logger::VERBOSE, std::ostream* os = 0)
            {
                sb_.add_stream(verbosity, os);
            }            
            //@}

            /// \name Overloaded insert stream operator<<
            //@{
            // overload this function so we can look for the die manipulator
            // and set the die_flag
            // to exit after this line
            std::ostream& operator<<(FlexOstream& (*pf) (FlexOstream&));
            std::ostream& operator<<(std::ostream & (*pf) (std::ostream &));
            
            //provide interfaces to the rest of the types
            std::ostream& operator<< (bool& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (const short& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (const unsigned short& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (const int& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (const unsigned int& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (const long& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (const unsigned long& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (const float& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (const double& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (const long double& val )
            { return std::ostream::operator<<(val); }
            std::ostream& operator<< (std::streambuf* sb )
            { return std::ostream::operator<<(sb); }
            std::ostream& operator<< (std::ios& ( *pf )(std::ios&))
            { return std::ostream::operator<<(pf); }
            std::ostream& operator<< (std::ios_base& ( *pf )(std::ios_base&))
            { return std::ostream::operator<<(pf); }
            //@}

            /// \name Thread safety related
            //@{
            /// Get a reference to the Goby logger mutex for scoped locking
            boost::mutex& mutex()
            { return Logger::mutex; }
            //@}

            void refresh()
            {
                sb_.refresh();
            }
            void set_group(const std::string & s)
            {
                sb_.group_name(s);
            }            
            
          private:            
            bool quiet()
            { return (sb_.is_quiet()); }
            
            
            template<typename T>
                friend void boost::checked_delete(T*);
            
            friend std::ostream& operator<< (FlexOstream& out, char c );
            friend std::ostream& operator<< (FlexOstream& out, signed char c );
            friend std::ostream& operator<< (FlexOstream& out, unsigned char c );
            friend std::ostream& operator<< (FlexOstream& out, const char *s );
            friend std::ostream& operator<< (FlexOstream& out, const signed char* s );
            friend std::ostream& operator<< (FlexOstream& out, const unsigned char* s );

          private:
            static int instances_;

          private:
            FlexOStreamBuf sb_;
        };        
        
        inline std::ostream& operator<< (FlexOstream& out, char c )
        { return std::operator<<(out, c); }
        inline std::ostream& operator<< (FlexOstream& out, signed char c )
        { return std::operator<<(out, c); }
        inline std::ostream& operator<< (FlexOstream& out, unsigned char c )
        { return std::operator<<(out, c); }        
        inline std::ostream& operator<< (FlexOstream& out, const char* s )
        { return std::operator<<(out, s); }        
        inline std::ostream& operator<< (FlexOstream& out, const signed char* s )
        { return std::operator<<(out, s); }        
        inline std::ostream& operator<< (FlexOstream& out, const unsigned char* s )
        { return std::operator<<(out, s); }                    
        //@}

    }

    /// \name Logger
    //@{
    /// \brief Access the Goby logger through this object. 
    extern util::FlexOstream glog;
    //@}
    namespace util
    {
        // for compatibility with Goby1
        inline FlexOstream& glogger()
        {
            return goby::glog;
        }
    }
}


/// Unlock the Goby logger after a call to glogger(lock)
inline std::ostream& unlock(std::ostream & os)
{
    goby::util::Logger::mutex.unlock();    
    return os;
}

class FlexOStreamErrorCollector : public google::protobuf::io::ErrorCollector
{
  public:
  FlexOStreamErrorCollector(const std::string& original)
      : original_(original),
        has_warnings_(false),
        has_errors_(false)
        { }
    
    void AddError(int line, int column, const std::string& message)
    {
        print_original(line, column);
        goby::glog << warn << "line: " << line << " col: " << column << " " << message << std::endl;
        has_errors_ = true;
    }
    void AddWarning(int line, int column, const std::string& message)
    {
        print_original(line, column);
        goby::glog << warn << "line: " << line << " col: " << column << " " << message << std::endl;
        has_warnings_ = true;
    }
    
    void print_original(int line, int column)
    {
        std::stringstream ss(original_ + "\n");
        std::string line_str;

        //for(int i = 0; i <= line; ++i)
        //    getline(ss, line_str);

        int i = 0;
        while(!getline(ss, line_str).eof())
        {
            if(i == line)
                goby::glog << goby::util::tcolor::lt_red << "[line " << std::setw(3) << i++ << "]" << line_str << goby::util::tcolor::nocolor << std::endl;
            else
                goby::glog << "[line " << std::setw(3) << i++ << "]" << line_str << std::endl;       
        }
    }


    bool has_errors() { return has_errors_; }
    bool has_warnings() { return has_warnings_; }
    
    
  private:
    const std::string& original_;
    bool has_warnings_;
    bool has_errors_;
};


#endif
