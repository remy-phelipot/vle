/**
 * @file vle/gvle/CutCopyPaste.hpp
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


#ifndef GUI_CUTCOPYPAST_HPP
#define GUI_CUTCOPYPAST_HPP

#include <vle/graph/CoupledModel.hpp>
#include <vle/vpz/Model.hpp>
#include <list>
#include <string>

namespace vle
{
namespace gvle {

/**
 * A class to manage cut/copy/paste action on GVLE views.
 *
 */
class CutCopyPaste
{
public:
    /** define type of paste : cut or copy */
    enum Type { CUT, COPY, NOTHING };

    /** create a cut copy paste object with no type */
    CutCopyPaste();

    /** delete list of GModel if present */
    ~CutCopyPaste();

    /**
    * detach the list of GModel of graph::CoupledModel parent.
    *
    * @param l list of GModel to detach.
    * @param gc graph::CoupledModel where list will be cut.
    */
    void cut(graph::ModelList & l, graph::CoupledModel* gc,
             vpz::AtomicModelList& src);

    /**
     * clone the graph::ModelList if has no connection with external
     * model.
     *
     * @param l list of GModel to copy.
     * @param gc graph::CoupledModel where list will be copied.
     */
    void copy(graph::ModelList& l, graph::CoupledModel* gc,
              vpz::AtomicModelList& src);

    /**
     * paste the current list GModel into graph::CoupledModel gc. Rename
     * models to eliminate duplicated name.
     *
     * @param gc graph::CoupledModel parent to attach list of model.
     * passed with function copy or cut.
     */
    void paste(graph::CoupledModel* gc, vpz::AtomicModelList& list);

    void state();

private:
    /** delete list of GModel if present */
    void clear();

    /**
     * clone the internal graph::ModelList mList. If num == -1 then
     * list is purely clone (parent is set to 0 and position move 5px).
     * Else, name is modified to add num
     *
     * @param lst_clone clone result
     * @param num number of clonage
     */
    void clone(graph::ModelList& lst_graph_clone,
               vpz::AtomicModelList& lst_vpz_clone, int num);

    void state(vpz::AtomicModelList& list);

    void cut_atomic(graph::Model* model, vpz::AtomicModelList& src);
    void cut_coupled(graph::Model* model, vpz::AtomicModelList& src);

    void copy_atomic(graph::Model* model, graph::Model* clone, vpz::AtomicModelList& src);
    void copy_coupled(graph::Model* model, graph::Model* clone, vpz::AtomicModelList& src);

    void clone_atomic(graph::Model* model, graph::Model* clone, vpz::AtomicModelList& vpz_list);
    void clone_coupled(graph::Model* model, graph::Model* clone, vpz::AtomicModelList& vpz_list);

    graph::ModelList           mList_graph;
    vpz::AtomicModelList       mList_vpz;
    int                        mNumero;
    Type                       mType;

    std::vector <std::string> mCnts;

};

}
} // namespace vle gvle

#endif