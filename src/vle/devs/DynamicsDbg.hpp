/*
 * @file vle/devs/DynamicsDbg.hpp
 *
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems
 * http://www.vle-project.org
 *
 * Copyright (c) 2003-2007 Gauthier Quesnel <quesnel@users.sourceforge.net>
 * Copyright (c) 2003-2011 ULCO http://www.univ-littoral.fr
 * Copyright (c) 2007-2011 INRA http://www.inra.fr
 *
 * See the AUTHORS or Authors.txt file for copyright owners and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef VLE_DEVS_DYNAMICSDBG_HPP
#define VLE_DEVS_DYNAMICSDBG_HPP

#include <vle/devs/DllDefines.hpp>
#include <vle/devs/Dynamics.hpp>
#include <vle/version.hpp>

#define DECLARE_DYNAMICS_DBG(mdl)                                     \
    extern "C" {                                                      \
        VLE_DEVS_EXPORT vle::devs::Dynamics*                          \
        vle_make_new_dynamics(const vle::devs::DynamicsInit& init,    \
                              const vle::devs::InitEventList& events) \
        {                                                             \
            vle::devs::DynamicsDbg* x__;                              \
            x__ = new vle::devs::DynamicsDbg( init, events);          \
            x__->set(new mdl(init, events));                          \
            return x__;                                               \
        }                                                             \
                                                                      \
        VLE_DEVS_EXPORT void                                          \
        vle_api_level(vle::uint32_t* major,                           \
                      vle::uint32_t* minor,                           \
                      vle::uint32_t* patch)                           \
        {                                                             \
            *major = VLE_MAJOR_VERSION;                               \
            *minor = VLE_MINOR_VERSION;                               \
            *patch = VLE_PATCH_VERSION;                               \
        }                                                             \
    }

namespace vle { namespace devs {

    /**
     * @brief A Dynamics debug class which wrap an another Dynamics to show
     * debug information. This class inherits the DEVS standard API.
     */
    class VLE_DEVS_EXPORT DynamicsDbg : public Dynamics
    {
    public:
        /**
         * @brief Constructor of Dynamics for an atomic model.
         *
         * @param init The initialiser of Dynamics.
         * @param events The parameter from the experimental frame.
         */
        DynamicsDbg(const DynamicsInit& init,
                    const InitEventList& events);

        /**
         * @brief Destructor.
         */
        virtual ~DynamicsDbg() { delete mDynamics; }

        /**
         * @brief Assign a Dynamics to debug for this DynamicsDbg class.
         *
         * @param dyn The Dynamics to attach.
         */
        void set(Dynamics* dyn) { mDynamics = dyn; }

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
          * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
         * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        /**
         * @brief Process the initialization of the model by initializing the
         * initial state and return the duration (or timeAdvance) of the initial
         * state.
         * @param time the time of the creation of this model.
         * @return duration of the initial state.
         */
        virtual Time init(const Time& time);

        /**
         * @brief Process the output function: compute the output function.
         * @param time the time of the occurrence of output function.
         * @param output the list of external events (output parameter).
         */
        virtual void output(const Time& time, ExternalEventList& output) const;

        /**
         * @brief Process the time advance function: compute the duration of the
         * current state.
         * @return duration of the current state.
         */
        virtual Time timeAdvance() const;

        /**
         * @brief Process an internal transition: compute the new state of the
         * model with the internal transition function.
         * @param time the date of occurence of this event.
         */
        virtual void internalTransition(const Time& time);

        /**
         * @brief Process an external transition: compute the new state of the
         * model when an external event occurs.
         * @param event the external event with of the port.
         * @param time the date of occurrence of this event.
         */
        virtual void
            externalTransition(const ExternalEventList& event,
                               const Time& time);

        /**
         * @brief Process the confluent transition: select the transition to
         * call when an internal and one or more external event appear in the
         * same time.
         * @param internal the internal event.
         * @param extEventlist the external events list.
         */
        virtual void
            confluentTransitions(const Time& time,
                                 const ExternalEventList& extEventlist);

        /**
         * @brief Process a request event: these functions occurs when an
         * RequestEvent is push into an ExternalEventList by the
         * output function.
         * @param event RequestEvent to process.
         * @param time the date of occurrence of this event.
         * @param output the list of external events (output parameter).
         * @return a response to the model. This bag can include External and
         * Request event.
         */
        virtual void
            request(const RequestEvent& event,
                    const Time& time,
                    ExternalEventList& output) const;

        /**
         * @brief Process an observation event: compute the current state of the
         * model at a specified time and for a specified port.
         * @param event the state event with of the port
         * @return the value of state variable
         */
        virtual vle::value::Value*
            observation(const ObservationEvent& event) const;

        /**
         * @brief When the simulation of the atomic model is finished, the
         * finish method is invoked.
         */
        virtual void finish();

    private:
        Dynamics* mDynamics;
        std::string mName;
    };

}} // namespace vle devs

#endif
