/**
 * @file vle/gvle/SimpleTypeBox.hpp
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


#ifndef VLE_GVLE_SIMPLETYPEBOX_HPP
#define VLE_GVLE_SIMPLETYPEBOX_HPP

#include <gtkmm.h>
#include <vle/value/Value.hpp>
#include <vle/value/Integer.hpp>
#include <vle/value/Double.hpp>
#include <vle/value/String.hpp>
#include <vle/value/Tuple.hpp>

namespace vle { namespace gvle {

    class SimpleTypeBox : public Gtk::Dialog
    {
    public:
        SimpleTypeBox(value::ValueBase* base);
        SimpleTypeBox(const std::string& title);

        virtual ~SimpleTypeBox();

        std::string run();

        /**
         * @brief Return true if the user have selected the Gtk::Button valid.
         * False otherwise.
         *
         * @return true if Gtk::Dialog is true, false otherwise.
         */
        bool valid() const { return mValid; }

    private:
        value::ValueBase* mBase;
        Gtk::Entry* mEntry;
        bool mValid;

        void makeDialog();
    };

}} // namespace vle gvle

#endif
