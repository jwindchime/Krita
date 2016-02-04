/*
 *  Copyright (c) 2007 Adrian Page <adrian@pagenet.plus.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "opengl/kis_opengl.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QDesktopServices>
#include <QMessageBox>

#include <klocalizedstring.h>

#include <kis_debug.h>
#include <kis_config.h>

namespace
{
    bool NeedsFenceWorkaround = false;
    int glVersion = 0;
    QString Renderer;
}

void KisOpenGL::initialize()
{
    dbgOpenGL << "OpenGL: initializing";
    KisConfig cfg;

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);

    if (cfg.openGLVersion() == 0) {
        format.setVersion(2, 1);
    }
    else {
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(0); // Disable vertical refresh syncing

    QSurfaceFormat::setDefaultFormat(format);

    glVersion = 100 * QSurfaceFormat::defaultFormat().majorVersion() + QSurfaceFormat::defaultFormat().minorVersion();
    dbgOpenGL << "GL Version:" << glVersion << QSurfaceFormat::defaultFormat().swapInterval() << QSurfaceFormat::defaultFormat().swapBehavior();

}

int KisOpenGL::initializeContext(QOpenGLContext* ctx)
{
    KisConfig cfg;
    dbgOpenGL << "OpenGL: Opening new context";

    // Double check we were given the version we requested
    QSurfaceFormat format = ctx->format();
    glVersion = 100 * format.majorVersion() + format.minorVersion();

    dbgOpenGL << "GL Version:" << glVersion << format.swapInterval() << format.swapBehavior();

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

#ifndef GL_RENDERER
#  define GL_RENDERER 0x1F01
#endif
    Renderer = QString((const char*)f->glGetString(GL_RENDERER));
    QString vendor((const char*)f->glGetString(GL_VENDOR));
    QString version((const char*)f->glGetString(GL_VERSION));

    dbgOpenGL << "Vendor:" << vendor << "Renderer" << Renderer << "version" << version;

    QFile log(QDesktopServices::storageLocation(QDesktopServices::TempLocation) + "/krita-opengl.txt");
    dbgOpenGL << "Writing OpenGL log to" << log.fileName();
    log.open(QFile::WriteOnly);
    log.write(vendor.toLatin1());
    log.write(", ");
    log.write(Renderer.toLatin1());
    log.write(", ");
    log.write(version.toLatin1());

    // Check if we have a bugged driver that needs fence workaround
    bool isOnX11 = false;
#ifdef HAVE_X11
    isOnX11 = true;
#endif

    if ((isOnX11 && Renderer.startsWith("AMD")) || cfg.forceOpenGLFenceWorkaround()) {
        NeedsFenceWorkaround = true;
    }

    return glVersion;
}

bool KisOpenGL::supportsFenceSync()
{
    KisConfig cfg;
    // 0: 2.1
    // 1: 3.2
    // ... undecides
    return cfg.openGLVersion() > 0;
}

bool KisOpenGL::needsFenceWorkaround()
{
    return NeedsFenceWorkaround;
}
