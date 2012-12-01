/*
 *  Copyright (c) 2012 Dmitry Kazakov <dimula73@gmail.com>
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

#ifndef __KIS_UI_ACTION_FACTORY_H
#define __KIS_UI_ACTION_FACTORY_H

#include <QString>
#include <krita_export.h>
#include "kis_properties_configuration.h"

class KisView2;

class KisUiActionConfiguration : public KisPropertiesConfiguration
{
public:
    KisUiActionConfiguration();
    KisUiActionConfiguration(const QString &id);

    QString id() const;
};


class KRITAUI_EXPORT KisUiActionFactory
{
public:
    KisUiActionFactory(const QString &id);
    virtual ~KisUiActionFactory();

    QString id() const;

    virtual void runFromXML(KisView2 *view, const KisUiActionConfiguration &config);

private:
    const QString m_id;
};

#endif /* __KIS_UI_ACTION_FACTORY_H */