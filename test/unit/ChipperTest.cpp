/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>

#include <libpc/chipper.hpp>
#include <libpc/drivers/liblas/writer.hpp>
#include <libpc/drivers/liblas/reader.hpp>

#include "support.hpp"

using namespace libpc;
using namespace libpc::drivers::liblas;

extern std::string g_data_path;

BOOST_AUTO_TEST_SUITE(ChipperTest)

BOOST_AUTO_TEST_CASE(test_construction)
{

    std::istream* ifs = Utils::openFile(g_data_path + "1.2-with-color.las");
    LiblasReader reader(*ifs);
    

    {
        const boost::uint64_t num_points = reader.getHeader().getNumPoints();

        // need to scope the writer, so that's it dtor can use the stream
        libpc::chipper::Chipper chipper(reader, 15);

        chipper.Chip();
        boost::uint32_t num_blocks = chipper.GetBlockCount();
        BOOST_CHECK(num_points == 1065);
        BOOST_CHECK(num_blocks==71);
        
        
        libpc::Bounds<double> const& bounds = chipper.GetBlock(0).GetBounds();

        
        std::vector< Range<double> > ranges = bounds.dimensions();
        
        libpc::Range<double> x = ranges[0];
        libpc::Range<double> y = ranges[1];
        
        BOOST_CHECK(Utils::compare_approx(x.getMinimum(), (double)635674, (double) 1) == true);
        BOOST_CHECK(Utils::compare_approx(x.getMaximum(), (double)635994, (double) 1) == true);
        BOOST_CHECK(Utils::compare_approx(y.getMinimum(), (double)848992, (double) 1) == true);
        BOOST_CHECK(Utils::compare_approx(y.getMaximum(), (double)849427, (double) 1) == true);

        std::vector<boost::uint32_t> ids = chipper.GetBlock(70).GetIDs();

        BOOST_CHECK(ids.size() == 15);
        BOOST_CHECK(ids[14] == 1050 );

    }


    Utils::closeFile(ifs);


    return;
}



BOOST_AUTO_TEST_SUITE_END()
