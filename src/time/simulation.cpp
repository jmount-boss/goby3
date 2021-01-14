// Copyright 2011-2020:
//   GobySoft, LLC (2013-)
//   Massachusetts Institute of Technology (2007-2014)
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

#include <chrono> // for system_clock::tim...

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>    // for ptime
#include <boost/date_time/time.hpp>                // for base_time<>::date...
#include <boost/date_time/time_system_counted.hpp> // for counted_time_syst...

#include "goby/time/convert.h"      // for SystemClock::now
#include "goby/time/simulation.h"   // for SimulatorSettings
#include "goby/time/system_clock.h" // for SystemClock

bool goby::time::SimulatorSettings::using_sim_time = false;
int goby::time::SimulatorSettings::warp_factor = 1;

// creates the default reference time, which is Jan 1 of the current year
std::chrono::system_clock::time_point create_reference_time()
{
    using namespace goby::time;
    using boost::posix_time::ptime;

    auto now_ptime = SystemClock::now<ptime>();
    ptime last_year_start(boost::gregorian::date(now_ptime.date().year(), 1, 1));
    return convert<std::chrono::system_clock::time_point>(last_year_start);
}

std::chrono::system_clock::time_point
    goby::time::SimulatorSettings::reference_time(create_reference_time());
