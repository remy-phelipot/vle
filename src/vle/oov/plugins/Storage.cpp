/**
 * @file vle/oov/plugins/Storage.cpp
 * @author The VLE Development Team
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment (http://vle.univ-littoral.fr)
 * Copyright (C) 2003 - 2008 The VLE Development Team
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


#include <vle/oov/plugins/Storage.hpp>
#include <vle/utils/Debug.hpp>
#include <vle/value/Set.hpp>
#include <vle/value/Double.hpp>
#include <vle/value/Integer.hpp>
#include <vle/value/String.hpp>
#include <vle/value/Null.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace vle { namespace oov { namespace plugin {

Storage::Storage(const std::string& location) :
    Plugin(location),
    m_matrix(2, 2, 1, 1),
    m_time(-1.0),
    m_isstart(false)
{
}

Storage::~Storage()
{
}

bool Storage::isSerializable() const
{
    return true;
}

value::Value* Storage::serialize() const
{
    return m_matrix.serialize();
}

void Storage::deserialize(const value::Value& vals)
{
    m_matrix.deserialize(vals);
}

std::string Storage::name() const
{
    return std::string("storage");
}

void Storage::onParameter(const vpz::ParameterTrame& trame)
{
    std::istringstream param(trame.data());
    int columns = -1, rows = -1, rzcolumns = -1, rzrows = -1;

    if (param) {
        param >> columns;
        if (param) {
            param >> rows;
            if (param) {
                param >> rzcolumns;
                if (param) {
                    param >> rzrows;
                }
            }
        }
    }
    m_matrix.resize(columns > 1 ? columns : 10, rows > 1 ? rows : 10);
    m_matrix.updateStep(rzcolumns > 0 ? rzcolumns : 1, rzrows > 0 ? rzrows : 1);
}

void Storage::onNewObservable(const vpz::NewObservableTrame& trame)
{
    std::string name(trame.parent());
    name += trame.name();
    m_matrix.addModel(name, trame.port());
}

void Storage::onDelObservable(const vpz::DelObservableTrame& /* trame */)
{
}

void Storage::onValue(const vpz::ValueTrame& trame)
{
    if (m_isstart) {
        nextTime(utils::to_double(trame.time()));
    } else {
        if (m_time < .0) {
            m_time = utils::to_double(trame.time());
        } else {
            nextTime(utils::to_double(trame.time()));
            m_isstart = true;
        }
    }

    for (vpz::ModelTrameList::const_iterator it = trame.trames().begin();
         it != trame.trames().end(); ++it) {
        std::string name(it->parent());
        name += it->simulator();
        if (not it->value()->isNull()) {
            m_matrix.addValue(name, it->port(), *it->value());
        }
    }
}

void Storage::close(const vpz::EndTrame& /*trame*/)
{
    m_matrix.setLastTime(m_time);
}

value::VectorView Storage::getTime()
{
    return m_matrix.getTime();
}

void Storage::nextTime(double trame_time)
{
    if (trame_time != m_time) {
        m_matrix.setLastTime(m_time);
	m_time = trame_time;
    }
}

}}} // namespace vle oov plugin
