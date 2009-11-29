/**
 * @file vle/oov/plugins/cairo/gauge/CairoGauge.cpp
 * @author The VLE Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment
 * http://www.sourceforge.net/projects/vle
 *
 * Copyright (C) 2003 - 2007 Gauthier Quesnel <quesnel@users.sourceforge.net>
 * Copyright (C) 2003 - 2009 ULCO http://www.univ-littoral.fr
 * Copyright (C) 2007 - 2009 INRA http://www.inra.fr
 * Copyright (C) 2007 - 2009 Cirad http://www.cirad.fr
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


#include <vle/oov/plugins/cairo/gauge/CairoGauge.hpp>
#include <vle/utils/Debug.hpp>
#include <vle/value/Double.hpp>

namespace vle { namespace oov { namespace plugin {

CairoGauge::CairoGauge(const std::string& location) :
    CairoPlugin(location),
    mLine(1.),
    mMin(0.0),
    mMax(1.0),
    mValue(0.0),
    mScale(1.8),
    mWindowWidth(175),
    mWindowHeight(120),
    mTime(-1.0)
{
}

CairoGauge::~CairoGauge()
{
}

void CairoGauge::onParameter(const std::string& /* plugin */,
                             const std::string& /* location */,
                             const std::string& /* file */,
                             value::Value* parameters,
                             const double& /* time */)
{
    if (parameters) {
        if (not parameters->isMap()) {
            throw utils::ArgError(
                _("Gauge: initialization failed, bad parameters"));
        }
        value::Map* init = dynamic_cast < value::Map* >(parameters);

	mMin = value::toDouble(init->get("min"));
	mMax = value::toDouble(init->get("max"));

        delete parameters;
    }
}

void CairoGauge::onNewObservable(const std::string& simulator,
                                 const std::string& /* parent */,
                                 const std::string& port,
                                 const std::string& /* view */,
                                 const double& /* time */)
{
    std::string name(buildname(simulator, port));

    if (not mName.empty()) {
        throw utils::InternalError(fmt(
                _("CairoGauge: observable '%1%' already exists")) % name);
    }

    mName = name;
}

void CairoGauge::onDelObservable(const std::string& /* simulator */,
                                 const std::string& /* parent */,
                                 const std::string& /* port */,
                                 const std::string& /* view */,
                                 const double& /* time */)
{
}

void CairoGauge::onValue(const std::string& simulator,
                         const std::string& /* parent */,
                         const std::string& port,
                         const std::string& /* view */,
                         const double& time,
                         value::Value* value)
{
    mTime = time;

    std::string name(buildname(simulator, port));

    if (name == mName) {
        mValue = value->toDouble().value();
        delete value;
    } else {
        throw utils::InternalError(fmt(
                _("CairoGauge: columns %1% does not exist")) % name);
    }

    draw();
    copy();
}

void CairoGauge::close(const double& /* time */)
{
}

void CairoGauge::preferredSize(int& width, int& height)
{
    width = mWindowWidth;
    height = mWindowHeight;
}

void CairoGauge::draw()
{
    draw_background(m_ctx);
    draw_text(m_ctx);
    draw_line(m_ctx);
}

void CairoGauge::draw_background(Cairo::RefPtr < Cairo::Context > m_ctx)
{
    double sy = mWindowHeight * mScale;

    m_ctx->rectangle(0, 0, mWindowWidth, mWindowHeight);
    m_ctx->set_source_rgb(1.,1.,1.);
    m_ctx->fill();

    m_ctx->begin_new_path();
    m_ctx->set_source_rgb(0.,0.,0.);
    m_ctx->arc(mWindowWidth/2, mWindowHeight,
               (mWindowWidth>sy) ? sy/2 : mWindowWidth / 2, M_PI, 0);
    m_ctx->close_path();
    m_ctx->stroke();
}

void CairoGauge::draw_line(Cairo::RefPtr < Cairo::Context > m_ctx)
{
    double nx = 0.;
    double ny = 0.;
    double mx = mWindowWidth / 2;
    double my = mWindowHeight - mLine ;

    double sy = mWindowHeight * mScale;
    double rayon = (mWindowWidth > sy) ? sy/2 : mWindowWidth/2;

    if (mValue <= mMin) {
	nx = mx - rayon  + mLine ;
	ny = mWindowHeight;
    } else if (mValue >= mMax) {
	nx = mx + rayon - mLine ;
	ny = mWindowHeight;
    } else {
	double angle = M_PI + (M_PI * mValue / (mMax - mMin));
	nx = mx + (std::cos(angle) * rayon) - mLine;
	ny = my + (std::sin(angle) * rayon) - mLine;
    }

    m_ctx->set_line_width(mLine);
    m_ctx->set_source_rgb(0.,0.,1.);
    m_ctx->begin_new_path();
    m_ctx->move_to(mx, my);
    m_ctx->line_to(nx, ny);
    m_ctx->close_path();
    m_ctx->stroke();
}

void CairoGauge::draw_text(Cairo::RefPtr < Cairo::Context > m_ctx)
{
    double sy = mWindowHeight * mScale;
    double rayon = (mWindowWidth>sy)?sy/2:mWindowWidth/2;

    m_ctx->set_source_rgb(0.,0.,0.);
    m_ctx->move_to( mWindowWidth / 2 - rayon + 5 , mWindowHeight - 3*mLine);
    m_ctx->show_text((fmt("min: %1%") % mMin).str());

    m_ctx->move_to(mWindowWidth / 2 - 15, mWindowHeight - 3*mLine ) ;
    m_ctx->show_text(vle::utils::toString(mValue));

    m_ctx->move_to(mWindowWidth / 2 + rayon - 40, mWindowHeight - 3*mLine );
    m_ctx->show_text( (fmt("max: %1%") % mMax).str() );
}

}}} // namespace vle oov plugin

