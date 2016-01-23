/*

TCP DTMF decoder

Copyright (C) 2016 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Revision: 3251 $ $Date:: 2016-01-22 #$ $Author: serge $

#include "tcp_dtmf_detector.h"      // self

#include <vector>

#include "../tcp_data_receiver/i_data_receiver.h"       // IDataReceiver
#include "../tcp_data_receiver/tcp_data_receiver.h"     // TcpDataReceiver
#include "../dtmf_detector/IDtmfDetectorCallback.hpp"   // dtmf::IDtmfDetectorCallback
#include "../dtmf_detector/DtmfDetector.hpp"            // dtmf::DtmfDetector
#include "../utils/dummy_logger.h"      // dummy_log

#define MODULENAME      "TcpDtmfDetector"

namespace tcp_dtmf_detector
{

static const int BUFLEN = 512;

class TcpDtmfDetectorImpl: public tcp_data_receiver::IDataReceiver
{
public:
    TcpDtmfDetectorImpl():
        detector_( BUFLEN, 16000 )
    {
    }

    bool init( dtmf::IDtmfDetectorCallback * callback, unsigned short port )
    {
        auto b = serv_.init( this, port );

        if( b == false )
        {
            dummy_log_fatal( MODULENAME, "cannot init server" );
        }

        detector_.init_callback( callback );

        dummy_log_info( MODULENAME, "inited: OK, port %u", port );

        return true;
    }

    void shutdown()
    {
        serv_.shutdown();
    }

    void worker()
    {
        serv_.worker();
    }

    // tcp_data_receiver::IDataReceiver
    virtual void on_received( uint32_t size, const char *data )
    {
        //std::cout << "receiving ... got " << buff_.size() << std::endl;

        if( size > 0 )
        {
            buff_.insert( buff_.end(), data, data + size );

            auto buf_size = buff_.size();

            if( buf_size >= BUFLEN * 2 )
            {
                detector_.process( reinterpret_cast<int16_t*>( &buff_[0] ) );

                auto rest_size = buf_size - BUFLEN;

                if( rest_size > 0 )
                {
                    // move unprocessed elements to the beginning
                    buff_.erase( buff_.begin(), buff_.begin() + rest_size );
                }

                buff_.resize( rest_size );
            }
        }
        else
        {
            buff_.resize( 0 );

            //detector_.reset();    // TODO reset() not implemented yet
        }
    }

private:

    std::vector<char>           buff_;

    tcp_data_receiver::Server   serv_;

    dtmf::DtmfDetector          detector_;

};


TcpDtmfDetector::TcpDtmfDetector()
{
    impl_   = new TcpDtmfDetectorImpl;
}

TcpDtmfDetector::~TcpDtmfDetector()
{
    delete impl_;
}

bool TcpDtmfDetector::init( dtmf::IDtmfDetectorCallback * callback, unsigned short port )
{
    return impl_->init( callback, port );
}

void TcpDtmfDetector::shutdown()
{
    impl_->shutdown();
}

void TcpDtmfDetector::worker()
{
    impl_->worker();
}

} // namespace tcp_dtmf_detector
