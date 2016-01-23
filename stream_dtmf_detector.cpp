/*

Stream DTMF decoder

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

// $Revision: 3259 $ $Date:: 2016-01-23 #$ $Author: serge $

#include "stream_dtmf_detector.h"      // self

#include <vector>
//#include <iostream>

#include "../dtmf_detector/IDtmfDetectorCallback.hpp"   // dtmf::IDtmfDetectorCallback
#include "../dtmf_detector/DtmfDetector.hpp"            // dtmf::DtmfDetector
#include "../utils/dummy_logger.h"      // dummy_log
#include "../utils/mutex_helper.h"      // MUTEX_SCOPE_LOCK

#define MODULENAME      "StreamDtmfDetector"

namespace tcp_dtmf_detector
{

static const int BUFLEN = 512;

StreamDtmfDetector::StreamDtmfDetector( uint32_t sampling_rate ):
    detector_( BUFLEN, sampling_rate )
{
}

bool StreamDtmfDetector::init( dtmf::IDtmfDetectorCallback * callback )
{
    detector_.init_callback( callback );

    dummy_log_info( MODULENAME, "inited: OK" );

    return true;
}

void StreamDtmfDetector::process( uint32_t size, const char *data )
{
    if( size > 0 )
    {
        MUTEX_SCOPE_LOCK( mutex_ );

        buff_.insert( buff_.end(), data, data + size );

        auto buf_size = buff_.size();

        if( buf_size >= BUFLEN * 2 )
        {
            process_buffer();
        }
    }
}

void StreamDtmfDetector::reset()
{
    MUTEX_SCOPE_LOCK( mutex_ );

    buff_.resize( 0 );

    //detector_.reset();    // TODO reset() not implemented yet
}

void StreamDtmfDetector::process_buffer()
{
    // no mutex lock: called from locked area

    auto buf_size = buff_.size();

    std::size_t i = 0;

    for( ; i < ( buf_size - BUFLEN * 2); i+= BUFLEN * 2 )
    {
        detector_.process( reinterpret_cast<int16_t*>( &buff_[ i ] ) );
    }

    auto rest_size = buf_size - i;

    if( rest_size > 0 )
    {
        // move unprocessed elements to the beginning
        buff_.erase( buff_.begin(), buff_.begin() + rest_size );

        //std::cout << "rest size ... " << rest_size << std::endl;
    }

    buff_.resize( rest_size );
}

} // namespace tcp_dtmf_detector
