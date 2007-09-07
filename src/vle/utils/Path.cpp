/**
 * @file utils/Path.cpp
 * @author The VLE Development Team.
 * @brief Portable way, i.e. Linux/Unix/Windows to get VLE paths.
 */

/*
 * Copyright (c) 2004, 2005 The vle Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include <list>
#include <vle/utils/Path.hpp>
#include <vle/utils/Exception.hpp>

#ifdef G_OS_WIN32
#	include <Windows.h>
#	include <winreg.h>
#endif



namespace vle { namespace utils {

Path* Path::mPath = 0;

const std::string& Path::getPrefixDir() const
{
    return m_prefix;
}

std::string Path::getPixmapsDir() const
{
    return buildPrefixSharePath(m_prefix, std::string("pixmaps"));
}

std::string Path::getGladeDir() const
{
    return buildPrefixSharePath(m_prefix, "glade");
}

std::string Path::getPixmapsFile(const std::string& file) const
{
    return buildPrefixSharePath(m_prefix, "pixmaps", file);
}

std::string Path::getGladeFile(const std::string& file) const
{
    return buildPrefixSharePath(m_prefix, "glade", file);
}


std::string Path::getHomeDir() const
{
    std::list < std::string > arb;
    arb.push_back(Glib::get_home_dir());
#ifdef G_OS_WIN32
    arb.push_back("vle");
#else
    arb.push_back(".vle");
#endif
    return Glib::build_path(G_DIR_SEPARATOR_S, arb);
}

void Path::addSimulatorDir(const std::string& dirname)
{
    m_simulator.push_back(dirname);
}

void Path::addTranslatorDir(const std::string& dirname)
{
    m_translator.push_back(dirname);
}

void Path::addStreamDir(const std::string& dirname)
{
    m_stream.push_back(dirname);
}

void Path::addModelDir(const std::string& dirname)
{
    m_model.push_back(dirname);
}

void Path::addPluginDir(const std::string& dirname)
{
    addSimulatorDir(dirname);
    addTranslatorDir(dirname);
    addStreamDir(dirname);
    addModelDir(dirname);
}

std::string Path::buildPrefixPath(const std::string& buf)
{
    std::list < std::string > lst;
    lst.push_front(VLE_PREFIX_DIR);
    lst.push_back(buf);
    return Glib::build_path(G_DIR_SEPARATOR_S, lst);
}

std::string Path::buildUserPath(const std::string& dir)
{
    if (dir.empty()) {
        std::list < std::string > arb;
        arb.push_back(Glib::get_home_dir());
#ifdef G_OS_WIN32
        arb.push_back("vle");
#else
        arb.push_back(".vle");
#endif
        return Glib::build_path(G_DIR_SEPARATOR_S, arb);
    } else {
        std::list < std::string > arb;
        arb.push_back(Glib::get_home_dir());
#ifdef G_OS_WIN32
        arb.push_back("vle");
#else
        arb.push_back(".vle");
#endif
        arb.push_back(dir);
        return Glib::build_path(G_DIR_SEPARATOR_S, arb);
    }
}

std::string Path::buildPrefixLibrariesPath(const std::string& prefix,
                                           const std::string& name)
{
    std::list < std::string > lst;
    lst.push_back(prefix);
    lst.push_back(VLE_LIBRARY_DIRS);
    lst.push_back(name);
    return Glib::build_path(G_DIR_SEPARATOR_S, lst);
}

std::string Path::buildPrefixSharePath(const std::string& prefix,
                                       const std::string& prg,
                                       const std::string& name)
{
    std::list < std::string > lst;
    lst.push_front(prefix);
    lst.push_back(VLE_SHARE_DIRS);
    lst.push_back(prg);
    if (not name.empty()) {
        lst.push_back(name);
    }
    return Glib::build_path(G_DIR_SEPARATOR_S, lst);
}

std::string Path::buildPath(const std::string& left, const std::string& right)
{
    std::list < std::string > lst;
    lst.push_front(left);
    lst.push_back(right);
    return Glib::build_path(G_DIR_SEPARATOR_S, lst);
}

#ifdef G_OS_WIN32
bool Path::initPath()
{
    LONG result;
    HKEY hkey;

    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\VLE", 0,
			  KEY_QUERY_VALUE, &hkey);

    if (result == ERROR_SUCCESS) {
	char* buf = new char[256];
	DWORD sz = 256;

	if (RegQueryValueEx(hkey, TEXT("system"), NULL, NULL,
			    (LPBYTE)buf, &sz) != ERROR_SUCCESS) {
	    delete[] buf;
	    return false;
	}
	m_prefix.assign(buf);
	delete[] buf;

        addSimulatorDir(buildPrefixLibrariesPath(m_prefix, "simulator"));
        addSimulatorDir(buildUserPath("simulator"));
        addSimulatorDir(".");

        addTranslatorDir(buildPrefixLibrariesPath(m_prefix, "translator"));
        addTranslatorDir(buildUserPath("translator"));
        addTranslatorDir(".");

        addStreamDir(buildPrefixLibrariesPath(m_prefix, "stream"));
        addStreamDir(buildUserPath("stream"));
        addStreamDir(".");

        addModeldir(buildPrefixLibrariesPath(m_prefix, "model"));
        addModeldir(buildUserPath("model"));
        addModeldir(".");
	return true;
    }
    return false;
}
#else // On Unix platform
bool Path::initPath()
{
    m_prefix.assign(VLE_PREFIX_DIR);

    addSimulatorDir(buildPrefixLibrariesPath(m_prefix, "simulator"));
    addSimulatorDir(buildUserPath("simulator"));
    addSimulatorDir(".");

    addTranslatorDir(buildPrefixLibrariesPath(m_prefix, "translator"));
    addTranslatorDir(buildUserPath("translator"));
    addTranslatorDir(".");

    addStreamDir(buildPrefixLibrariesPath(m_prefix, "stream"));
    addStreamDir(buildUserPath("stream"));
    addStreamDir(".");

    addModelDir(buildPrefixLibrariesPath(m_prefix, "model"));
    addModelDir(buildUserPath("model"));
    addModelDir(".");

    return true;
}

#endif // Win32 specific needs.

Path::Path()
{
    if (initPath() == false) {
        throw InternalError("Path initialization failed.");
    }
}

}} // namespace vle utils
